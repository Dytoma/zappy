/*
** EPITECH PROJECT, 2025
** GUI [WSL: Ubuntu-24.04]
** File description:
** Tile
*/

#ifndef TILE_HPP_
    #define TILE_HPP_
    #include <string>
    #include <vector>
    #include <memory>
    #include "Resource.hpp"
    #include "Egg.hpp"
#include "ResourceManager.hpp"

class Tile {
    public:
        Tile(Vector2 pos, float size, const std::string &textPath);
        ~Tile();

        void Draw() const;

        Vector2 GetPosition() const;
        float GetSize() const;
        Color GetColor() const;

        void SetPosition(Vector2 pos);
        // void SetSize(float s);
        void SetColor(Color c);

        void reloadResource(int q0, int q1, int q2, int q3, int q4, int q5, int q6);
        void addResource(Type type, Vector2 pos);
        void popResource(Type type);
        void addEgg(std::unique_ptr<Egg> egg);

        int getResourceCount(Type type) const;
        std::string getResourceName(Type type) const;
        std::vector<std::unique_ptr<Egg>>& getEggs();

        //for debug
        void printResources() const;

    private:
        Vector2 position;
        float size;
        Color color;
        // Texture2D texture;
        std::shared_ptr<Zappy::Texture> texture;
        ResourceManager *_resourceManager;
        std::shared_ptr<Zappy::ModelGui> model;
        bool selected;
        std::vector<std::unique_ptr<Resource>> _resources;
        std::vector<std::unique_ptr<Egg>> _eggs;
};

#endif /* !TILE_HPP_ */
