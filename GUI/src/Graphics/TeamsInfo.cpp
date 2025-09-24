/*
** EPITECH PROJECT, 2025
** GUI
** File description:
** TeamsInfo
*/

#include "TeamsInfo.hpp"
#include <filesystem>
#include <iostream>

void TeamsInfo::computeValues()
{
    _x = 28;
    _height = 110 + 25 * static_cast<int>(_teams->size());
    _y = GetScreenHeight() - _height - 48 - 120;
}

TeamsInfo::TeamsInfo(std::vector<Team>* teams, ResourceManager* resources)
    : _width(245),
      _height(0),
      _teams(teams),
      _active(true),
      _color({230, 72, 72, 100}),
    _resourceManager(resources)
{
    computeValues();
}

void TeamsInfo::drawWithFont()
{
    DrawTextEx(_resourceManager->getFont(),
    "TEAMS INFORMATION", Vector2(_x + 20, _y + 20), 18, 1.0f, BLACK);
    DrawTextEx(_resourceManager->getFont(),
    "NAME", Vector2(_x + 20, _y + 62), 16, 1.0f, BLACK);
    DrawTextEx(_resourceManager->getFont(),
    "NB PL", Vector2(_x + 108, _y + 62), 16, 1.0f, BLACK);
    DrawTextEx(_resourceManager->getFont(),
    "EGGS", Vector2(_x + 189, _y + 62), 16, 1.0f, BLACK);
    for (size_t i = 0; i < _teams->size(); i++) {
        std::string displayName = _teams->at(i).name;
        if (displayName.size() > 7)
            displayName.erase(displayName.begin() + 7);
        DrawTextEx(_resourceManager->getFont(),
            displayName.c_str(),
            Vector2(_x + 20, _y + 90 + i * 25), 14, 1.0f, DARKGRAY);
        DrawTextEx(_resourceManager->getFont(),
            std::to_string(_teams->at(i).connected_clients).c_str(),
            Vector2(_x + 124, _y + 90 + i * 25), 14, 1.0f, DARKGRAY);
        DrawTextEx(_resourceManager->getFont(),
            std::to_string(_teams->at(i).nb_eggs).c_str(),
            Vector2(_x + 194, _y + 90 + i * 25), 14, 1.0f, DARKGRAY);
    }
}

void TeamsInfo::drawWithoutFont()
{
    DrawText("TEAMS INFORMATION", _x + 20, _y + 20, 18, BLACK);
    DrawText("NAME", _x + 20, _y + 62, 16, BLACK);
    DrawText("NB PL", _x + 108, _y + 62, 16, BLACK);
    DrawText("EGGS", _x + 189, _y + 62, 16, BLACK);
    for (size_t i = 0; i < _teams->size(); i++) {
        std::string displayName = _teams->at(i).name;
        if (displayName.size() > 7)
            displayName.erase(displayName.begin() + 7);
        DrawText(
            displayName.c_str(),
            _x + 20, _y + 90 + i * 25, 14, DARKGRAY);
        DrawText(
            std::to_string(_teams->at(i).connected_clients).c_str(),
            _x + 124, _y + 90 + i * 25, 14, DARKGRAY);
        DrawText(
            std::to_string(_teams->at(i).nb_eggs).c_str(),
            _x + 194, _y + 90 + i * 25, 14, DARKGRAY);
    }
}

void TeamsInfo::Draw()
{
    if (IsKeyPressed(KEY_TAB))
        _active = !_active;
    if (!_active)
        return;
    computeValues();
    DrawRectangle(_x, _y, _width, _height, Fade(_color, 0.5f));
    DrawRectangleLines(_x, _y, _width, _height, Fade(_color, 0.8f));
    if (IsFontValid(_resourceManager->getFont())) {
        drawWithFont();
    } else {
        drawWithoutFont();
    }
}

