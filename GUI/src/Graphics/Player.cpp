/*
** EPITECH PROJECT, 2025
** GUI
** File description:
** Player
*/

#include "Player.hpp"
#include <iostream>
#include <random>
#include <ctime>

#include "Utils.hpp"

Player::Player(int id, std::string teamName, Vector2 gridPos, Vector2 mapSize, float tileSize, Vector3 modelCenter, const char* modelPath, float scale, float timeUnit)
    : _grid(gridPos),
      pos{},
      _mapSize(mapSize),
      _tileSize(tileSize),
      _modelCenter(modelCenter),
      _model(modelPath),
      _teamName(teamName),
      _id(id),
      _scale(scale)
{
    std::cout << "Player from " << teamName << " with id " << _id << " created" << std::endl;
    std::cout << "Model selected: " << modelPath << std::endl;
    animCounts = animFrameCounter = 0;
    _resourceManager = ResourceManager::getInstance();
    anims = LoadModelAnimations(modelPath, &animCounts);
    model = _resourceManager->getModel(modelPath);
    _scaleVector = {scale, scale, scale};
    _rotationAxis = {1.0f, 0.0f, 0.0f};
    _rotationAngle = 90.0f;
    _moveProgress = 1.0f;
    _moveDuration = timeUnit;
    if (_moveDuration < 0.01f)
        _moveDuration = 0.01f;
    _moveElapsed = 0.0f;
    _isMoving = false;
    _moveAnim = 0;
    static std::mt19937 rng(static_cast<unsigned int>(std::time(nullptr)));
    std::uniform_int_distribution<int> dist(0, 3);
    _direction = static_cast<Direction>(dist(rng));
    setDuration(7.0f / std::max(1.0f, timeUnit));
    updateWorldPosition();
    std::cout << "Position: (" << pos.x << ", " << pos.y << ", " << pos.z << ")" << std::endl;
}

Player::~Player() {
    UnloadModelAnimations(anims, animCounts);
}

void Player::animPlayer(int anim)
{
    animFrameCounter++;
    if (anims && animCounts > 0)
        UpdateModelAnimation(model->_model, anims[anim], animFrameCounter);
    if (animFrameCounter >= anims[anim].frameCount)
        animFrameCounter = 0;
}

void Player::Draw() {
    DrawModelEx(model->_model,
        pos,
        {0,1,0},
        _rotationAngle,
        {_scale, _scale, _scale},
        WHITE);
}

int Player::getId() const
{
    return _id;
}

void Player::setPosition(Vector2 newPos)
{
    Vector2 from = wrapPosition(_grid);
    Vector2 to = wrapPosition(newPos);
    int mapWidth = static_cast<int>(_mapSize.x);
    int mapHeight = static_cast<int>(_mapSize.y);

    bool wrapX = (std::abs(from.x - to.x) > 1) &&
                 ((from.x == 0 && to.x == mapWidth - 1) || (from.x == mapWidth - 1 && to.x == 0));
    bool wrapY = (std::abs(from.y - to.y) > 1) &&
                 ((from.y == 0 && to.y == mapHeight - 1) || (from.y == mapHeight - 1 && to.y == 0));

    if (wrapX || wrapY) {
        _isWrapping = true;
        _wrapPhase = 0;
        _wrapTarget = to;
        _startGrid = from;
        if (wrapX) {
            float edge = (from.x == mapWidth - 1 && to.x == 0) ? from.x + 0.5f : from.x - 0.5f;
            _targetGrid = {edge, from.y};
            float outside = (from.x == mapWidth - 1 && to.x == 0) ? -0.5f : mapWidth - 0.5f;
            _preWrapTarget = {outside, to.y};
        } else {
            float edge = (from.y == mapHeight - 1 && to.y == 0) ? from.y + 0.5f : from.y - 0.5f;
            _targetGrid = {from.x, edge};
            float outside = (from.y == mapHeight - 1 && to.y == 0) ? -0.5f : mapHeight - 0.5f;
            _preWrapTarget = {to.x, outside};
        }
        _moveProgress = 0.0f;
        _moveElapsed = 0.0f;
        _isMoving = true;
        _moveAnim = 0;
        return;
    }

    _isWrapping = false;
    _wrapPhase = 0;
    if (_grid.x == to.x && _grid.y == to.y) {
        _isMoving = false;
        _moveProgress = 1.0f;
        _moveElapsed = 0.0f;
        updateWorldPosition();
        return;
    }
    _startGrid = _grid;
    _targetGrid = to;
    _moveProgress = 0.0f;
    _moveElapsed = 0.0f;
    _isMoving = true;
    _moveAnim = 0;
}

Vector2 Player::wrapPosition(Vector2 pos) const
{
    float wrappedX = fmodf(pos.x + _mapSize.x, _mapSize.x);
    float wrappedY = fmodf(pos.y + _mapSize.y, _mapSize.y);
    return {wrappedX, wrappedY};
}

void Player::setLevel(int lvl)
{
    level = lvl;
}

void Player::setOrientation(int direction)
{
    switch (direction)
    {
        case 1:
            _direction = NORTH; break;
        case 2:
            _direction = EAST; break;
        case 3:
            _direction = SOUTH; break;
        case 4:
            _direction = WEST; break;
        default:
            break;
    }
    setRotation(_direction);
}

void Player::setInventory(Type rType, const int quantity)
{
    for (int i = 0; i < quantity; ++i) {
        _inventory.push_back(std::make_unique<Resource>(Vector2{0, 0}, rType, 1.0f));
    }
    
    switch (rType) {
        case FOOD: food += quantity; break;
        case LINEMATE: linemate += quantity; break;
        case DERAUMERE: deraumere += quantity; break;
        case SIBUR: sibur += quantity; break;
        case MENDIANE: mendiane += quantity; break;
        case THYSTAME: thystame += quantity; break;
        default: break;
    }
}

bool Player::resourceDrop(Type type)
{
    for (size_t i = 0; i < _inventory.size(); ++i) {
        if (_inventory[i] && _inventory[i]->getType() == type) {
            _inventory.erase(_inventory.begin() + i);
            return true;
        }
    }
    return false;
}

Vector2 Player::getPosition() const
{
    return _grid;
}

std::string Player::getTeamName() const
{
    return _teamName;
}

void Player::addInventory(Type type, Vector2 pos)
{
    _inventory.push_back(std::make_unique<Resource>(pos, type, 1.0f));
}

void Player::setIncantation(const bool incantation)
{
    _incantation = incantation;
}

bool Player::checkPosition(int x, int y)
{
    if (_grid.x == static_cast<float>(x) &&
        _grid.y == static_cast<float>(y))
        return true;
    return false;
}

void Player::updateWorldPosition() {
    float gridWidth = _mapSize.x * _tileSize;
    float gridHeight = _mapSize.y * _tileSize;
    float gridCenterX = gridWidth / 2.0f;
    float gridCenterZ = gridHeight / 2.0f;
    float offsetX = _modelCenter.x - gridCenterX + _tileSize / 2.0f;
    float offsetZ = _modelCenter.z - gridCenterZ + _tileSize / 2.0f;

    pos.x = _grid.x * _tileSize + offsetX;
    pos.z = _grid.y * _tileSize + offsetZ;

    BoundingBox box = GetModelBoundingBox(model->_model);
    float tileTopY = _tileSize / 2.0f;
    float modelMinY = box.min.y * _scale;
    pos.y = tileTopY - modelMinY - 0.5f;
}

void Player::Update(float deltaTime)
{
    if (_playingPendingAnim && _pendingAnim >= 0 && anims && animCounts > _pendingAnim) {
        _pendingAnimTimer += deltaTime;
        _pendingAnimFrame++;
        UpdateModelAnimation(model->_model, anims[_pendingAnim], _pendingAnimFrame);

        if (_pendingAnimFrame >= anims[_pendingAnim].frameCount ||
            _pendingAnimTimer >= _pendingAnimDuration) {
            _pendingAnim = -1;
            _pendingAnimFrame = 0;
            _pendingAnimTimer = 0.0f;
            _playingPendingAnim = false;
        }
        return;
    }

    if (_isMoving) {
        _moveElapsed += deltaTime;
        _moveProgress = std::min(_moveElapsed / _moveDuration, 1.0f);

        float x = _startGrid.x + (_targetGrid.x - _startGrid.x) * _moveProgress;
        float y = _startGrid.y + (_targetGrid.y - _startGrid.y) * _moveProgress;
        _grid = {x, y};
        updateWorldPosition();
        animPlayer(_moveAnim);

        if (_moveProgress >= 1.0f) {
            if (_isWrapping) {
                if (_wrapPhase == 0) {
                    _wrapPhase = 1;
                    _startGrid = _preWrapTarget;
                    _targetGrid = _wrapTarget;
                    _grid = _preWrapTarget;
                    updateWorldPosition();
                    _moveProgress = 0.0f;
                    _moveElapsed = 0.0f;
                    return;
                } else if (_wrapPhase == 1) {
                    _grid = _wrapTarget;
                    updateWorldPosition();
                    _isMoving = false;
                    _isWrapping = false;
                    _wrapPhase = 0;
                    _moveProgress = 1.0f;
                    _moveElapsed = 0.0f;
                    animFrameCounter = 0;
                    return;
                }
            }
            _grid = _targetGrid;
            updateWorldPosition();
            _isMoving = false;
            animFrameCounter = 0;
        }
        return;
    }
    animPlayer(2);
}

void Player::setRotation(Direction dir)
{
    _direction = dir;
    switch (_direction) {
        case NORTH: _rotationAngle = 90.0f; break;
        case EAST:  _rotationAngle = 00.0f; break;
        case SOUTH: _rotationAngle = 270.0f; break;
        case WEST:  _rotationAngle = 180.0f; break;
    }
}

void Player::setDuration(float duration) {
    _moveDuration = duration;
}

float Player::getDuration() const
{
    return _moveDuration;
}

std::string Player::getModel() const
{
    return _model;
}

int Player::ressourceCount(Type type) const
{
    int count = 0;
    for (const auto& res : _inventory) {
        if (res->getType() == type)
            count++;
    }
    return count;
}

std::vector<Type> Player::getCleanInventory() const
{
    std::vector<Type> uniqueTypes;
    std::set<Type> seenTypes;

    for (const auto& res : _inventory) {
        Type type = res->getType();
        if (seenTypes.insert(type).second) {
            uniqueTypes.push_back(type);
        }
    }

    return uniqueTypes;
}

int Player::getInventorySize() const
{
    return _inventory.size();
}

Vector3 Player::get3DPos() const
{
    return pos;
}

std::string Player::get2DModel() const
{
    size_t lastSlash = _model.find_last_of("/\\");
    std::string filename = (lastSlash != std::string::npos) ? _model.substr(lastSlash + 1) : _model;

    std::string lowercase;
    for (char c : filename) {
        lowercase += std::tolower(c);
    }

    size_t dotPos = lowercase.find_last_of('.');
    if (dotPos != std::string::npos) {
        lowercase = lowercase.substr(0, dotPos);
    }

    return getAssetsPath() + "images/" + lowercase + ".png";
}

void Player::setBroadcast(std::string msg)
{
    _broadcast = msg;
}

std::string Player::getBroadcast() const
{
    return _broadcast;
}

void Player::playPickupAnimation()
{
    _pendingAnim = 3;
    _pendingAnimFrame = 0;
    _pendingAnimTimer = 0.0f;
    _playingPendingAnim = true;
}

int Player::getOrientation() const
{
    return static_cast<int>(_direction) + 1;
}

void Player::setMoveDuration(float duration)
{
    _moveDuration = std::max(0.01f, duration);
}
