/*
** EPITECH PROJECT, 2025
** GUI
** File description:
** ResourceManager
*/

#include "ResourceManager.hpp"
#include <filesystem>
#include <iostream>
#include <vector>

#include "Utils.hpp"

ResourceManager* ResourceManager::_instance = nullptr;

Zappy::Texture::Texture(const std::string& texturePath) : _texturePath(texturePath)
{
    _texture = LoadTexture(texturePath.c_str());
}

Texture2D& Zappy::Texture::getTexture()
{
    return _texture;
}

Zappy::Texture::~Texture()
{
    UnloadTexture(_texture);
}

Zappy::ModelGui::ModelGui(const std::string& modelPath) : _modelPath(modelPath)
{
    _model = LoadModel(modelPath.c_str());
}

Zappy::ModelGui::ModelGui(float size)
{
    Mesh cubeMesh = GenMeshCube(size, 1.0f, size);
    _model = LoadModelFromMesh(cubeMesh);
    _modelPath = "Cube";
}


Model& Zappy::ModelGui::getModel()
{
    return _model;
}

Zappy::ModelGui::~ModelGui()
{
    UnloadModel(_model);
}

ResourceManager* ResourceManager::getInstance()
{
    if (_instance == nullptr) {
        _instance = new ResourceManager();
        _instance->loadAssets();
    }
    return _instance;
}

void ResourceManager::loadAssets()
{
    std::vector<std::string> pngFiles;
    std::vector<std::string> modelFiles;
    const std::string path = getAssetsPath();

    // std::cout << path << std::endl;
    for (const auto& entry : std::filesystem::recursive_directory_iterator(path)) {
        if (entry.is_regular_file() && entry.path().extension() == ".png") {
            pngFiles.push_back(entry.path().string());
        }
        if (entry.is_regular_file() && entry.path().extension() == ".glb") {
            modelFiles.push_back(entry.path().string());
        }
    }
    // for (auto& pngFile : pngFiles)
    //     std::cout << pngFile << std::endl;
    // for (auto& modelFile : modelFiles)
    //     std::cout << modelFile << std::endl;
    for (auto &pngFile : pngFiles)
        if (!textures.contains(pngFile))
            textures[pngFile] = std::make_shared<Zappy::Texture>(pngFile);
    // textures.insert(std::make_pair(pngFile, std::make_shared<Zappy::Texture>(pngFile)));
    for (auto &modelFile : modelFiles)
        if (!models.contains(modelFile))
            models[modelFile] = std::make_shared<Zappy::ModelGui>(modelFile);
    if (!models.contains("Cube_oak"))
        models["Cube_oak"] = std::make_shared<Zappy::ModelGui>(TILESIZE);
    if (!models.contains("Cube_spruce"))
        models["Cube_spruce"] = std::make_shared<Zappy::ModelGui>(TILESIZE);
    _font = LoadFont(std::string(path + "textures/SpaceGrotesk.ttf").c_str());
    // models.insert(std::make_pair(modelFile, std::make_shared<Zappy::ModelGui>(modelFile)));
}

std::shared_ptr<Zappy::Texture> ResourceManager::getTexture(const std::string &texturePath)
{
    if (textures.contains(texturePath))
        return textures[texturePath];
    return nullptr;
}

std::shared_ptr<Zappy::ModelGui> ResourceManager::getModel(const std::string &modelPath)
{
    if (models.contains(modelPath))
        return models[modelPath];
    return nullptr;
}

Font& ResourceManager::getFont()
{
    return _font;
}

ResourceManager::~ResourceManager()
{
    models.clear();
    textures.clear();
}


