/*
** EPITECH PROJECT, 2025
** GUI [WSL: Ubuntu-24.04]
** File description:
** Utils
*/

#ifndef UTILS_HPP_
    #define UTILS_HPP_
    #include <string>
    #include <vector>
    #include "raylib.h"
    #include "Resource.hpp"

std::vector<std::string> str_to_word_array(const std::string &, const std::string &);
Vector3 getResourcePosition(const Vector2& position, Type type, const float tileSize, float scale);
bool MyCheckCollisionRayBox(Ray ray, BoundingBox box);
std::string getAssetsPath();

#endif /* !UTILS_HPP_ */
