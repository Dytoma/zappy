/*
** EPITECH PROJECT, 2025
** GUI
** File description:
** Resource
*/

#include "Resource.hpp"
#include "Utils.hpp"
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <filesystem>


std::unordered_map<Type, std::shared_ptr<Zappy::ModelGui>> Resource::models;
std::unordered_map<Type, BoundingBox> Resource::modelBoxes;

std::vector<std::shared_ptr<Zappy::Texture>> Resource::resourceTextures;
std::vector<std::string> Resource::resourceImagePaths;

void Resource::LoadModels(ResourceManager *resourceManager)
{
    std::string currentDir = std::filesystem::current_path().string();
    const std::string modelLocation = getAssetsPath() + "models/";

    models[FOOD]      = resourceManager->getModel(std::string(modelLocation + "cake.glb"));
    models[LINEMATE]  = resourceManager->getModel(std::string(modelLocation + "potion.glb"));
    models[DERAUMERE] = resourceManager->getModel(std::string(modelLocation + "torch.glb"));
    models[SIBUR]     = resourceManager->getModel(std::string(modelLocation + "iron.glb"));
    models[MENDIANE]  = resourceManager->getModel(std::string(modelLocation + "gold.glb"));
    models[PHIRAS]    = resourceManager->getModel(std::string(modelLocation + "emerald.glb"));
    models[THYSTAME]  = resourceManager->getModel(std::string(modelLocation + "diamond.glb"));
    for (const auto& [type, model] : models)
        modelBoxes[type] = GetModelBoundingBox(model->_model);
}

void Resource::UnloadModels()
{
    models.clear();
    modelBoxes.clear();
}

void Resource::LoadTextures(ResourceManager *resourceManager)
{
    const std::string imageLocation = getAssetsPath() + "images/";
    resourceTextures = std::vector<std::shared_ptr<Zappy::Texture>>(7);
    resourceImagePaths = {
        imageLocation + "cake.png",
        imageLocation + "potion.png",
        imageLocation + "torch.png",
        imageLocation + "iron.png",
        imageLocation + "gold.png",
        imageLocation + "emerald.png",
        imageLocation + "diamond.png"
    };
    for (int i = 0; i < 7; ++i)
        resourceTextures[i] = resourceManager->getTexture(resourceImagePaths[i]);
}

void Resource::UnloadTextures()
{
    resourceImagePaths.clear();
    resourceTextures.clear();
}

Resource::Resource(Vector2 tile, Type type, float scale)
    : _tile(tile), _type(type), _scale(scale)
{
    _position = getResourcePosition(tile, type, TILESIZE, scale);
}

void Resource::Draw()
{
    float t = GetTime();
    float amplitude = 0.2f;
    float floatSpeed = 2.0f;
    float rotationSpeed = 50.0f;

    float baseY = _position.y;
    float floatPhase = t * floatSpeed + _position.x + _position.z;
    float animatedY = baseY + amplitude * 0.5f * (sinf(floatPhase) + 1.0f);

    float rotation = fmodf(t * rotationSpeed, 360.0f);

    Vector3 pos = {_position.x - TILESIZE / 2, animatedY, _position.z - TILESIZE / 2};
    DrawModelEx(models[_type]->_model,
        pos,
        {0, 1, 0},
        rotation,
        {_scale, _scale, _scale},
        WHITE);
}

Type Resource::getType() const
{
    return _type;
}

Vector3 Resource::getPosition() const
{
    return _position;
}
