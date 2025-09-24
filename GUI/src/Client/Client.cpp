/*
** EPITECH PROJECT, 2025
** B-YEP-400-TLS-4-1-zappy-dytoma.batogouma
** File description:
** Client.cpp
*/

#include "Client.hpp"
#include "Utils.hpp"
#include "Window.hpp"
#include "Resource.hpp"
#include <iostream>
#include <chrono>
#include <filesystem>

std::mutex clientMutex;

Client::Client(const std::string &addr, const int port, const bool debugMode)
    : _fd(-1), _isConnected(false), _isRunning(true), _debugMode(debugMode)
{
    _fd = socket(AF_INET, SOCK_STREAM, 0);
    const int rand_port = std::experimental::randint(1024, 65535);
    if (addr.empty())
        std::cerr << "No address provided" << std::endl;
    _addr = {AF_INET, htons(rand_port),
        (in_addr){inet_addr(addr.c_str())}, {}};
    _serv_addr = {AF_INET, htons(port), (in_addr){inet_addr(addr.c_str())}, {}};
    if (_fd != -1)
        _pfds.push_back({_fd, POLLIN, {}});
    _map = nullptr;
    _grid = std::vector<std::unique_ptr<Tile>>();
    _players = std::vector<std::shared_ptr<Player>>();
    setModelsPaths();
    _teams = {};
    _broadcasting = false;
    _isGameOver = false;
    _map = nullptr;
    _selectedPlayer = nullptr;
    _incBtn = {30, 30, 40, 40};
    _decBtn = {80, 30, 40, 40};
    _showChat = false;
    _cursorEnabled = false;
    _mapSize = {0, 0};
    timeUnit = 100;
    showInfo = false;
}

void Client::setModelsPaths()
{
    std::vector<std::string> paths = {"Cedric.glb", "Eleejah.glb", "Lebron.glb", "Leon.glb",
        "Melissa.glb", "Sofiane.glb", "Steve.glb"};
    std::string currentDir = std::filesystem::current_path().string();
    _assetsPath = getAssetsPath();

    _skins.clear();
    for (const auto& path : paths)
        _skins.push_back(_assetsPath + "models/" + path);
}

Client::~Client() {
    stop();
}

void Client::connect_to_server()
{
    if (_fd == -1) {
        perror("Can't open connection port.");
        return;
    }

    {
        std::unique_lock<std::mutex> lock(clientMutex);
        if (connect(_fd, reinterpret_cast<sockaddr *>(&_serv_addr), sizeof(_serv_addr)) == -1) {
            perror("Can't connect to se_broadcasting = false;rver.");
            _isConnected = false;
            return;
        }
        _isConnected = true;
    }

    handleServerWelcome();
    initDataToSendToServer("GRAPHIC\n");
    initDataToSendToServer("msz\n");
    while (_isRunning) {
        if (std::cin.eof()) {
            initDataToSendToServer("QUIT\n");
            _isRunning = false;
            break;
        }
        int ret = poll(_pfds.data(), _pfds.size(), 10);
        if (ret == -1) {
            perror("poll");
            break;
        }
        if (ret > 0)
            processMessageFromServer();
        processMessageToServer();
    }
    if (_fd != -1) {
        close(_fd);
        _fd = -1;
    }
}

void Client::handleServerWelcome()
{
    char buffer[1024] = {0};
    ssize_t bytesRead = recv(_fd, buffer, sizeof(buffer) - 1, 0);
    if (bytesRead > 0) {
        buffer[bytesRead] = '\0';
        std::string msg(buffer);
        std::cout << msg << std::endl;
        if (msg.find("WELCOME") == std::string::npos) {
            std::cerr << "Did not receive expected WELCOME from server. Aborting." << std::endl;
            _isRunning = false;
        }
    } else {
        std::cerr << "Failed to receive WELCOME from server." << std::endl;
        _isRunning = false;
    }
}

void Client::processMessageFromServer()
{
    for (size_t i = 0; i < _pfds.size(); ++i) {
        if (_pfds[i].fd == _fd && (_pfds[i].revents & POLLIN)) {
            char buffer[BUFFER_SIZE] = {0};
            ssize_t bytesRead = recv(_fd, buffer, sizeof(buffer) - 1, 0);
            if (bytesRead > 0) {
                buffer[bytesRead] = '\0';
                _buffer += std::string(buffer);
                if (_debugMode)
                    std::cout << "\033[31m[SERVER]\033[0m " << _buffer;

                while (true) {
                    size_t end = _buffer.find('\n');
                    if (end == std::string::npos)
                        break;
                    std::string singleMessage = _buffer.substr(0, end);
                    if (!singleMessage.empty())
                        processServerMessages(singleMessage);
                    _buffer.erase(0, end + 1);
                }
            } else if (bytesRead == 0) {
                std::cout << "Server closed the connection." << std::endl;
                _isRunning = false;
                if (_fd != -1) {
                    close(_fd);
                    _fd = -1;
                }
            } else if (errno != EAGAIN && errno != EWOULDBLOCK) {
                perror("recv");
                _isRunning = false;
            }
        }
    }
}

void Client::processMessageToServer()
{
    std::unique_lock<std::mutex> lock(clientMutex);
    if (_fd == -1 || !_isRunning)
        return;
    while (!_queueMessages.empty()) {
        const std::string &msg = _queueMessages.front();
        ssize_t sent = send(_fd, msg.c_str(), msg.length(), 0);
        while (sent != -1) {
            if (static_cast<int>(sent) < static_cast<int>(msg.length()))
                sent += send(_fd, msg.c_str() + sent, msg.length() - sent, 0);
            if (static_cast<int>(sent) == static_cast<int>(msg.length())) {
                _queueMessages.pop();
                return;
            }
        }
        if (sent == -1) {
            perror("send");
            _isRunning = false;
            if (_fd != -1) {
                close(_fd);
                _fd = -1;
            }
            break;
        }
    }
}

void Client::processServerMessages(const std::string& message)
{
    std::unique_lock<std::mutex> lock(clientMutex);
    std::vector<std::string> words = str_to_word_array(message, " ,\n#");

    if (words[0] == "msz") {
        _mapSize.x = atoi(words[1].c_str());
        _mapSize.y = atoi(words[2].c_str());
    } else {
        _words.push(words);
        reload = true;
    }
}

void Client::initDataToSendToServer(const std::string &message)
{
    std::unique_lock<std::mutex> lock(clientMutex);
    _queueMessages.push(message);
}

Type Client::whichType(int i)
{
    switch (i) {
        case 0: return Type::FOOD;
        case 1: return Type::LINEMATE;
        case 2: return Type::DERAUMERE;
        case 3: return Type::SIBUR;
        case 4: return Type::MENDIANE;
        case 5: return Type::PHIRAS;
        case 6: return Type::THYSTAME;
        default: return Type::NONE;
    }
}

void Client::start()
{
    _isRunning = true;
    _serverThread = std::thread(&Client::connect_to_server, this);
    start_gui();
    if (_serverThread.joinable()) {
        _serverThread.join();
    }
}

void Client::stop()
{
    std::unique_lock<std::mutex> lock(clientMutex);

    if (_isConnected && _fd != -1) {
        std::string quitCmd = "QUIT\n";
        send(_fd, quitCmd.c_str(), quitCmd.size(), 0);
        shutdown(_fd, SHUT_RDWR);
        close(_fd);
        _fd = -1;
        _isConnected = false;
    }
    _isRunning = false;
}

void Client::reloadData()
{
    if (reload || !_words.empty()) {
        reload = false;
        if (_words.empty())
            return;
        while (!_words.empty()) {
            const std::vector<std::string> &commands = _words.front();
            _broadcasting = false;
            bct_command(commands);
            tna_command(commands);
            pnw_command(commands);
            ppo_command(commands);
            plv_command(commands);
            pin_command(commands);
            pdi_command(commands);
            pdr_command(commands);
            pgt_command(commands);
            enw_command(commands);
            sst_command(commands);
            seg_command(commands);
            pic_command(commands);
            pie_command(commands);
            sgt_command(commands);
            pbc_command(commands);
            pfk_command(commands);
            edi_command(commands);
            suc_command(commands);
            sbp_command(commands);
            pex_command(commands);
            _words.pop();
        }
    }
}

void Client::setCameras()
{
    Camera cam1;
    cam1.position = {0.0f, 4.0f, 10.5f};
    cam1.target = {0.0f, 0.0f, 0.0f};
    cam1.up = {0.0f, 1.0f, 0.0f};
    cam1.fovy = 45.0f;
    cam1.projection = CAMERA_PERSPECTIVE;
    _cameras.push_back(cam1);

    Camera cam2 = cam1;
    cam2.position = {5.0f, 4.0f, 10.5f};
    _cameras.push_back(cam2);

    Camera cam3 = cam1;
    cam3.position = {-5.0f, 4.0f, 10.5f};
    _cameras.push_back(cam3);
}

// Initilizers
void Client::setupCamera(Camera& camera)
{
    camera.position = {0.0f, 4.0f, 5.5f};
    camera.target = {0.0f, 0.0f, 0.0f};
    camera.up = {0.0f, 1.0f, 0.0f};
    camera.fovy = 45;
    camera.projection = CAMERA_PERSPECTIVE;
}


void Client::start_gui()
{
    SetTraceLogLevel(LOG_WARNING);
    InitWindow(1920, 1080, "Zappy - Graphics");
    DisableCursor();
    SetTargetFPS(60);
    setCameras();
    _resourceManager = ResourceManager::getInstance();
    int activeCameraIndex = 0;
    Resource::LoadModels(_resourceManager);
    Resource::LoadTextures(_resourceManager);

    Camera camera;
    setupCamera(camera);
    _teamsInfo = std::make_unique<TeamsInfo>(&_teams, _resourceManager);
    Model& map = _resourceManager->getModel(_assetsPath + "models/map.glb")->_model;
    std::shared_ptr<Player> selectedPlayer = nullptr;
    _map = std::make_unique<Map>(_assetsPath, _mapSize);

    initDataToSendToServer("mct\n");
    reloadData();
    initDataToSendToServer("tna\n");
    initDataToSendToServer("sgt\n");
    while (_isRunning && !WindowShouldClose()) {
        float deltaTime = GetFrameTime();
        if (!_isGameOver) {
            if (!_cursorEnabled && activeCameraIndex == 0)
                UpdateCamera(&_cameras[activeCameraIndex], CAMERA_FREE);
        }
        BeginDrawing();
        ClearBackground(Color{123, 169, 255, 100});
        BeginMode3D(_cameras[activeCameraIndex]);
        DrawModel(map, _map->getMapPos(), _mapSize.x + _mapSize.y, WHITE);
        _map->Draw();
        DrawPlayerBBox();

        if (_cursorEnabled && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            Vector2 mouse = GetMousePosition();
            Ray ray = GetMouseRay(mouse, _cameras[activeCameraIndex]);
            _map->setSelectedTile(-1);
            _selectedPlayer = nullptr;
            setSelectedPlayer(ray);
        }
        for (const auto &player : _players) {
            player->Update(deltaTime);
            player->Draw();
        }
        reloadData();
        EndMode3D();
        handleGameOver(activeCameraIndex);
        handleButtonEvents();
        if (_map->getSelectedTile() != -1) {
            Tile* tile = _map->_grid[_map->getSelectedTile()].get();
            DrawHUD(tile, Resource::resourceTextures);
        }
        handlePlayerInventory(activeCameraIndex);
        handleKeyEvents(activeCameraIndex);
        EndDrawing();
    }
    UnloadModel(map);
    Resource::UnloadTextures();
    Resource::UnloadModels();
    CloseWindow();
}
