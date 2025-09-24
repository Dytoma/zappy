/*
** EPITECH PROJECT, 2025
** B-YEP-400-TLS-4-1-zappy-dytoma.batogouma
** File description:
** Map
*/

#ifndef MAP_HPP_
    #define MAP_HPP_

#include <raylib.h>
#include <string>
#include <vector>
#include <memory>
#include "Tile.hpp"

class Map
{
    public:
    Map(const std::string &assetsPath, Vector2 mapSize);
    ~Map() = default;
    Vector3 &getMapPos();
    void Draw();
    void setSelectedTile(int tile);
    int getSelectedTile();
    float _offsetX, _offsetZ;
    std::vector<std::unique_ptr<Tile>> _grid;

private:
    float _gridCenterX, _gridCenterZ;
    float _gridWidth, _gridHeight;
    std::string _assetsPath;
    Vector3 mapPos;
    int selectedTile;
};


#endif /* !MAP_HPP_ */
