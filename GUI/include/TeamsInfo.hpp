/*
** EPITECH PROJECT, 2025
** GUI
** File description:
** TeamsInfo
*/

#ifndef TEAMSINFO_HPP_
#define TEAMSINFO_HPP_

#include <raylib.h>
#include <string>
#include <vector>

#include "ResourceManager.hpp"

struct Team
{
    std::string name;
    int connected_clients;
    int nb_eggs;
};

class TeamsInfo
{
public:
    TeamsInfo(std::vector<Team> *teams, ResourceManager *resourceManager);
    ~TeamsInfo() = default;

    TeamsInfo(const TeamsInfo&) = delete;
    TeamsInfo& operator=(const TeamsInfo&) = delete;

    void Draw();
    void drawWithFont();
    void drawWithoutFont();
    // void toggleActive();

private:
    void computeValues();
    int _x, _y;
    int _width, _height;
    std::vector<Team> *_teams;
    bool _active;
    Color _color;
    ResourceManager *_resourceManager;
};

#endif /* !TEAMSINFO_HPP_ */