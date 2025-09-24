/*
** EPITECH PROJECT, 2025
** GUI
** File description:
** Player
*/

#ifndef PLAYER_HPP_
#define PLAYER_HPP_

#include <iostream>
#include <vector>
#include <memory>
#include <queue>
#include <set>
#include "Resource.hpp"
#include "raylib.h"
#include "ResourceManager.hpp"

enum Direction
{
    NORTH = 0,
    EAST = 1,
    SOUTH = 2,
    WEST = 3
};

class Player {
public:
    Player(int id, std::string teamName, Vector2 gridPos, Vector2 mapSize, float tileSize, Vector3 modelCenter, const char* modelPath, float scale, float timeUnit);
    ~Player();

    void Draw();
    void animPlayer(int anim);
    bool resourceDrop(Type type);
    void addInventory(Type type, Vector2 pos);

    void setPosition(Vector2 pos);
    void setLevel(int lvl);
    void setOrientation(int direction);
    void setInventory(Type rtype, int quantity);
    void setIncantation(bool incantation);
    bool checkPosition(int x, int y);
    void Update(float deltaTime);
    void setRotation(Direction dir);
    void setDuration(float duration);
    float getDuration() const;
    std::string getModel() const;
    int ressourceCount(Type type) const;
    std::vector<Type> getCleanInventory() const;
    int getInventorySize() const;
    void playPickupAnimation();
    void setMoveDuration(float duration);


    Vector2 wrapPosition(Vector2 pos) const;
    std::string getTeamName() const;
    int getOrientation() const;
    int getId() const;
    Vector2 getPosition() const;
    Vector3 get3DPos() const;

    std::string get2DModel() const;

    void setBroadcast(std::string msg);
    std::string getBroadcast() const;

private:
    // Logical grid position
    Vector2 _grid;

    // World position (for drawing)
    Vector3 pos;

    // Context for conversion
    Vector2 _mapSize;
    float _tileSize;
    Vector3 _modelCenter;
    std::string _model;

    std::string _teamName;
    int _id;
    int level = 1;
    int food = 0;
    int linemate = 0;
    int deraumere = 0;
    int sibur = 0;
    int mendiane = 0;
    int phiras = 0;
    int thystame = 0;
    bool isAlive = true;
    
    // Model& model;
    std::shared_ptr<Zappy::ModelGui> model;
    // Texture2D texture;
    float _scale;
    std::vector<std::unique_ptr<Resource>> _inventory;
    Vector3 _rotationAxis;
    float _rotationAngle;
    Vector3 _scaleVector;
    Direction _direction;
    std::string _broadcast;

    // Animation
    Vector2 _startGrid;
    Vector2 _targetGrid;
    float _moveProgress;
    float _moveDuration;
    float _moveElapsed;
    bool _isMoving;
    int _moveAnim;
    std::queue<Vector2> _moveQueue;
    
    int _pendingAnim = -1;
    int _pendingAnimFrame = 0;
    float _pendingAnimTimer = 0.0f;
    const float _pendingAnimDuration = 2.0f;
    bool _playingPendingAnim = false;
    
    bool _incantation = false;
    ModelAnimation *anims = nullptr;
    int animCounts = 0;
    int animFrameCounter = 0;
    
    bool _isWrapping = false;
    int _wrapPhase = 0;
    Vector2 _wrapTarget = {0, 0};
    Vector2 _preWrapTarget = {0, 0};
    ResourceManager *_resourceManager;

    void updateWorldPosition();
};

#endif /* !PLAYER_HPP_ */