/*
** EPITECH PROJECT, 2025
** GUI [WSL: Ubuntu-24.04]
** File description:
** Utils
*/

#include "Utils.hpp"
#include <ctime>
#include <filesystem>

std::vector<std::string> str_to_word_array(const std::string& input, const std::string& delimiters)
{
    std::vector<std::string> result;
    std::string token;

    for (char c : input) {
        if (delimiters.find(c) != std::string::npos) {
            if (!token.empty()) {
                result.push_back(token);
                token.clear();
            }
        } else {
            token += c;
        }
    }
    if (!token.empty()) {
        result.push_back(token);
    }
    return result;
}

Vector3 getResourcePosition(const Vector2& tilePos, Type type, float tileSize, float scale)
{
    float x = tilePos.x + static_cast<float>(rand()) / RAND_MAX * tileSize;
    float z = tilePos.y + static_cast<float>(rand()) / RAND_MAX * tileSize;

    float tileTopY = TILESIZE / 2.0f;

    const BoundingBox& box = Resource::modelBoxes[type];
    float modelBaseOffset = -box.min.y * scale;

    float y = tileTopY + modelBaseOffset;

    return Vector3{x, y - 0.5f, z};
}

bool MyCheckCollisionRayBox(Ray ray, BoundingBox box)
{
    float tmin = (box.min.x - ray.position.x) / ray.direction.x;
    float tmax = (box.max.x - ray.position.x) / ray.direction.x;
    if (tmin > tmax) std::swap(tmin, tmax);

    float tymin = (box.min.y - ray.position.y) / ray.direction.y;
    float tymax = (box.max.y - ray.position.y) / ray.direction.y;
    if (tymin > tymax) std::swap(tymin, tymax);

    if ((tmin > tymax) || (tymin > tmax))
        return false;

    if (tymin > tmin)
        tmin = tymin;
    if (tymax < tmax)
        tmax = tymax;

    float tzmin = (box.min.z - ray.position.z) / ray.direction.z;
    float tzmax = (box.max.z - ray.position.z) / ray.direction.z;
    if (tzmin > tzmax) std::swap(tzmin, tzmax);

    if ((tmin > tzmax) || (tzmin > tmax))
        return false;

    return true;
}

std::string getAssetsPath()
{
    std::string currentDir = std::filesystem::current_path().string();

    if (currentDir.substr(currentDir.size() - 3) == "GUI")
        return "assets/";
    else
        return "GUI/assets/";
}
