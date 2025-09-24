/*
** EPITECH PROJECT, 2025
** GUI
** File description:
** Egg
*/

#include "Egg.hpp"
#include "Utils.hpp"

Egg::Egg(int id, Vector2 tile)
    : _id(id), _type(EGG), _tile(tile)
{
    _position = getResourcePosition(_tile, _type, TILESIZE, 1.0f);
    _position.y += 0.3f;
    _model = LoadModel(std::string(getAssetsPath() + "models/yoshi_egg.glb").c_str());
    _scale = 0.1f;
}

Egg::~Egg()
{
}

int Egg::getId() const
{
    return _id;
}

Vector3 Egg::getPosition() const
{
    return _position;
}

void Egg::Draw()
{
    DrawModelEx(_model, _position, {0,1,0}, 0.0f, {_scale, _scale, _scale}, WHITE);

}
