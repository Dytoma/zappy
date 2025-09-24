/*
** EPITECH PROJECT, 2025
** GUI
** File description:
** Resource
*/

#ifndef RESSOURCE_HPP_
    #define RESSOURCE_HPP_
    #include <string>
    #include <unordered_map>
    #include <vector>
    #include "raylib.h"
#include "ResourceManager.hpp"

enum Type {
    FOOD,
    LINEMATE,
    DERAUMERE,
    SIBUR,
    MENDIANE,
    PHIRAS,
    THYSTAME,
    EGG,
    NONE
};

class Resource {
    public:
        Resource(Vector2 _tile, Type type, float scale);

        void Draw();
        static std::unordered_map<Type, std::shared_ptr<Zappy::ModelGui>> models;
        static std::unordered_map<Type, BoundingBox> modelBoxes;

        static std::vector<std::shared_ptr<Zappy::Texture>> resourceTextures;
        static std::vector<std::string> resourceImagePaths;

        static void LoadModels(ResourceManager *resourceManager);
        static void UnloadModels();

        static void LoadTextures(ResourceManager *resourceManager);
        static void UnloadTextures();

        Type getType() const;
        Vector3 getPosition() const;

    protected:
        Vector2 _tile;
        Vector3 _position;
        Type _type;
        float _scale;

};

#endif /* !RESSOURCE_HPP_ */
