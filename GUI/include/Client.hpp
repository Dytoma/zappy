/*
** EPITECH PROJECT, 2025
** B-YEP-400-TLS-4-1-zappy-dytoma.batogouma
** File description:
** Client.hpp
*/

#ifndef CLIENT_H
    #define CLIENT_H
    #include <iostream>
    #include <vector>
    #include <poll.h>
    #include <arpa/inet.h>
    #include <netinet/in.h>
    #include <sys/socket.h>
    #include <experimental/random>
    #include <mutex>
    #include <unistd.h>
    #include <queue>
    #include <string>
    #include <cstring>
    #include <thread>
    #include <string>
    #include <vector>
    #include <atomic>
    #include <memory>
    #include <unordered_map>
    #include <algorithm>
    #include <utility>
    #include <chrono>
    #include "TeamsInfo.hpp"
    #include "Tile.hpp"
    #include "Player.hpp"
    #include "Egg.hpp"
#include "Map.hpp"
#include "ResourceManager.hpp"
#define BUFFER_SIZE 20480

class Client {
    public:
        Client(const std::string &addr, int port, bool debugMode);
        ~Client();

        void start();
        void stop();

    private:
        // Communication to the server
        void initDataToSendToServer(const std::string &message);
        void connect_to_server();
        void processMessageFromServer();
        void processMessageToServer();
        void handleServerWelcome();
        void processServerMessages(const std::string &message);

        // GUI
        void start_gui();
        void DrawHUD(Tile* tile, const std::vector<std::shared_ptr<Zappy::Texture>>& resourceTextures);
        void setCameras();
        void showInventory(std::shared_ptr<Player> player, const Vector2& position,
            const std::vector<std::shared_ptr<Zappy::Texture>>& resourceTextures);
        void displayChat();
        Rectangle endWindow(int width, int height);
        void DrawPlayerBBox();
        void setSelectedPlayer(const Ray &ray);
        void handleGameOver(int activeCameraIndex);
        void handleButtonEvents();
        void handlePlayerInventory(int activeCameraIndex);
        void handleKeyEvents(int &activeCameraIndex);

        // Initilizers
        void setupCamera(Camera &camera);
        // void initGrid();

        // Commands processed
        void bct_command(const std::vector<std::string> &words);
        void tna_command(const std::vector<std::string> &words);
        void pnw_command(const std::vector<std::string> &words);
        void ppo_command(const std::vector<std::string> &words);
        void plv_command(const std::vector<std::string> &words);
        void pin_command(const std::vector<std::string> &words);
        void pdi_command(const std::vector<std::string> &words);
        void pdr_command(const std::vector<std::string> &words);
        void pgt_command(const std::vector<std::string> &words);
        void enw_command(const std::vector<std::string> &words); //egg
        void sst_command(const std::vector<std::string> &words);
        void sgt_command(const std::vector<std::string> &words);
        void pic_command(const std::vector<std::string> &words);
        void pie_command(const std::vector<std::string> &words);
        void pbc_command(const std::vector<std::string> &words);
        void pfk_command(const std::vector<std::string> &words); //egg
        void edi_command(const std::vector<std::string> &words); //egg
        void ebo_command(const std::vector<std::string> &words); //egg
        void suc_command(const std::vector<std::string> &words);
        void sbp_command(const std::vector<std::string> &words);
        void seg_command(const std::vector<std::string> &words);
        void pex_command(const std::vector<std::string> &words);

        // Utils
        Type whichType(int i);
        void reloadData();
        void setModelsPaths();

        // Server communication
        int _fd;
        sockaddr_in _addr;
        sockaddr_in _serv_addr;
        bool _isConnected;
        std::atomic<bool> _isRunning;
        std::vector<pollfd> _pfds;
        std::queue<std::string> _queueMessages;
        std::string _buffer;
        std::vector<std::string> _skins;
        std::unordered_map<std::string, std::string> _teamSkins;
        Vector3 mapPos = {0.0f, 0.0f, 0.0f};
        std::unique_ptr<Map> _map;

        // Threading
        std::thread _serverThread;

        // Data needed for GUI
        std::vector<std::unique_ptr<Tile>> _grid;
        std::vector<std::shared_ptr<Player>> _players;
        std::shared_ptr<Player> _selectedPlayer;
        std::vector<Team> _teams;
        Vector2 _mapSize;
        int _ids;
        int timeUnit;
        bool isRunning;
        std::vector<Camera> _cameras;
        bool _broadcasting;
        std::vector<std::pair<int, std::string>> _messages;
        std::chrono::steady_clock::time_point _broadcastingStartTime;
        std::string _winningTeam;
        bool _isGameOver;
        bool showInfo;

        // adding a new player
        std::queue<std::vector<std::string>> _words;
        bool reload = false;
        std::unique_ptr<TeamsInfo> _teamsInfo;
        std::string _assetsPath;
        ResourceManager *_resourceManager;
        bool _debugMode;

        // Buttons
        Rectangle _incBtn;
        Rectangle _decBtn;

        // controls
        bool _cursorEnabled;
        bool _showChat;
};

#endif