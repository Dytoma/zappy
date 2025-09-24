/*
** EPITECH PROJECT, 2025
** GUI
** File description:
** ResourceManager
*/

#ifndef RESOURCEMANAGER_HPP_
    #define RESOURCEMANAGER_HPP_

#include <string>
#include <memory>
#include <raylib.h>
#include <unordered_map>
#define TILESIZE 2.0f

namespace Zappy
{
    class Texture
    {
    public:
        Texture(const std::string &texturePath);
        Texture(const Texture&) = delete;
        Texture2D& getTexture();
        ~Texture();
        Texture2D _texture;

    private:
        std::string _texturePath;
    };

    class ModelGui
    {
    public:
        ModelGui(const std::string &modelPath);
        ModelGui(float size);
        ModelGui(const ModelGui&) = delete;
        Model& getModel();
        ~ModelGui();
        Model _model;
    private:
        std::string _modelPath;
    };

}


class ResourceManager {
public:
    ResourceManager() = default;
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;
    ~ResourceManager();
    static ResourceManager* getInstance(); // Access the singleton instance

    void loadAssets();
    std::shared_ptr<Zappy::Texture> getTexture(const std::string &texturePath);
    std::shared_ptr<Zappy::ModelGui> getModel(const std::string &modelPath);
    Font& getFont();

private:

    std::unordered_map<std::string, std::shared_ptr<Zappy::Texture>> textures;
    std::unordered_map<std::string, std::shared_ptr<Zappy::ModelGui>> models;
    Font _font;

    static ResourceManager *_instance;
};


#endif /* !RESOURCEMANAGER_HPP_ */
