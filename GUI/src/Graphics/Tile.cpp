/*
** EPITECH PROJECT, 2025
** GUI [WSL: Ubuntu-24.04]
** File description:
** Tile
*/

#include "Tile.hpp"
#include "raylib.h"

Tile::Tile(Vector2 pos, float size, const std::string &textPath)
    : position(pos), size(size), color(WHITE), selected(false)
{
    _resourceManager = ResourceManager::getInstance();
    texture = _resourceManager->getTexture(textPath);
    if (textPath.find("oak") != std::string::npos)
        model = _resourceManager->getModel("Cube_oak");
    else
        model = _resourceManager->getModel("Cube_spruce");
    model->_model.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = texture->_texture;
    _resources = std::vector<std::unique_ptr<Resource>>();
}

void Tile::Draw() const
{
    DrawModel(model->_model, (Vector3){position.x, 0.0f, position.y}, 1.0f, WHITE);
    for (auto &resource : _resources) {
        resource->Draw();
    }
    for (auto &egg : _eggs) {
        egg->Draw();
    }
}

Vector2 Tile::GetPosition() const
{
    return position;
}

float Tile::GetSize() const
{
    return size;
}

Color Tile::GetColor() const
{
    return color;
}

void Tile::SetPosition(Vector2 pos)
{
    position = pos;
}

// void Tile::SetSize(float s)
// {
//     size = s;
//     UnloadModel(model);
//     Mesh cubeMesh = GenMeshCube(size, 1.0f, size);
//     model = LoadModelFromMesh(cubeMesh);
//     model.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = texture->_texture;
// }

void Tile::SetColor(Color c)
{
    color = c;
}

void Tile::reloadResource(int q0, int q1, int q2, int q3, int q4, int q5, int q6)
{
    _resources.clear();
    for (int i = 0; i < q0; i++)
        _resources.push_back(std::make_unique<Resource>(position, FOOD, 0.2f));
    for (int i = 0; i < q1; i++)
        _resources.push_back(std::make_unique<Resource>(position, LINEMATE, 0.08f)); //potion
    for (int i = 0; i < q2; i++)
        _resources.push_back(std::make_unique<Resource>(position, DERAUMERE, 0.2f)); // torch
    for (int i = 0; i < q3; i++)
        _resources.push_back(std::make_unique<Resource>(position, SIBUR, 0.08f)); //iron
    for (int i = 0; i < q4; i++)
        _resources.push_back(std::make_unique<Resource>(position, MENDIANE, 0.08f)); //gold
    for (int i = 0; i < q5; i++)
        _resources.push_back(std::make_unique<Resource>(position, PHIRAS, 1.2f)); //emerald
    for (int i = 0; i < q6; i++)
        _resources.push_back(std::make_unique<Resource>(position, THYSTAME, 0.03f)); //diamond
}

void Tile::addResource(Type type, Vector2 position)
{
    if (type == FOOD)
        _resources.push_back(std::make_unique<Resource>(position, FOOD, 0.2f));
    else if (type == LINEMATE)
        _resources.push_back(std::make_unique<Resource>(position, LINEMATE, 0.08f)); //potion
    else if (type == DERAUMERE)
        _resources.push_back(std::make_unique<Resource>(position, DERAUMERE, 0.2f)); // torch
    else if (type == SIBUR)
        _resources.push_back(std::make_unique<Resource>(position, SIBUR, 0.08f)); //iron
    else if (type == MENDIANE)
        _resources.push_back(std::make_unique<Resource>(position, MENDIANE, 0.08f)); //gold
    else if (type == PHIRAS)
        _resources.push_back(std::make_unique<Resource>(position, PHIRAS, 1.2f)); //emerald
    else if (type == THYSTAME)
        _resources.push_back(std::make_unique<Resource>(position, THYSTAME, 0.03f)); //diamond
}

void Tile::popResource(Type type)
{
    for (auto it = _resources.begin(); it != _resources.end(); ++it) {
        if ((*it)->getType() == type) {
            _resources.erase(it);
            return;
        }
    }
}

void Tile::addEgg(std::unique_ptr<Egg> egg) {
    _eggs.push_back(std::move(egg));
}

void Tile::printResources() const {
    std::cout << "Resources on tile at position (" << position.x << ", " << position.y << "):\n";
    for (const auto& res : _resources) {
        if (res) {
            std::cout << "- Type: " << static_cast<int>(res->getType())
                      << ", Pos: (" << res->getPosition().x << ", " << res->getPosition().y << ")\n";
        }
    }
}

int Tile::getResourceCount(Type type) const
{
    int count = 0;
    for (const auto& res : _resources) {
        if (res->getType() == type)
            count++;
    }
    return count;
}

std::string Tile::getResourceName(Type type) const
{
    switch(type) {
        case FOOD: return "Cake";
        case LINEMATE: return "Potion";
        case DERAUMERE: return "Torch";
        case SIBUR: return "Iron";
        case MENDIANE: return "Gold";
        case PHIRAS: return "Emerald";
        case THYSTAME: return "Diamond";
        default: return "";
    }
}

std::vector<std::unique_ptr<Egg>>& Tile::getEggs()
{
    return _eggs;
}

Tile::~Tile()
{
}

