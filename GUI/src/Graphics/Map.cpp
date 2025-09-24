/*
** EPITECH PROJECT, 2025
** B-YEP-400-TLS-4-1-zappy-dytoma.batogouma
** File description:
** Map
*/

#include "Map.hpp"


Map::Map(const std::string& assetsPath, Vector2 mapSize) : _assetsPath(assetsPath)
{
    _gridWidth  = mapSize.x * TILESIZE;
    _gridHeight = mapSize.y * TILESIZE;
    _gridCenterX = _gridWidth  / 2.0f;
    _gridCenterZ = _gridHeight / 2.0f;
    selectedTile = -1;
    mapPos = {0.0f, 0.0f, 0.0f};

    float modelCenterX = mapPos.x;
    float modelCenterZ = mapPos.z;

    _offsetX = modelCenterX - _gridCenterX + TILESIZE / 2.0f;
    _offsetZ = modelCenterZ - _gridCenterZ + TILESIZE / 2.0f;

    for (size_t i = 0; i < mapSize.x; ++i) {
        for (size_t j = 0; j < mapSize.y; ++j) {
            std::string textPath = ((i + j) % 2 == 0)
                ? _assetsPath + "textures/oak_planks.png"
                : _assetsPath + "textures/spruce_planks.png";
            Vector2 pos = {
                i * TILESIZE + _offsetX,
                j * TILESIZE + _offsetZ
            };
            _grid.push_back(std::make_unique<Tile>(pos, TILESIZE, textPath));
        }
    }
}

Vector3 &Map::getMapPos()
{
    return mapPos;
}

void Map::Draw()
{
    for (size_t i = 0; i < _grid.size(); i++) {
        _grid[i]->Draw();
        if (static_cast<int>(i) == selectedTile) {
            Vector3 cubeCenter = {_grid[i]->GetPosition().x, 0.0f, _grid[i]->GetPosition().y};
            DrawCubeWires(cubeCenter, _grid[i]->GetSize(), 1.0f, _grid[i]->GetSize(), GREEN);
        }
    }
}

void Map::setSelectedTile(const int tile)
{
    selectedTile = tile;
}

int Map::getSelectedTile()
{
    return selectedTile;
}

