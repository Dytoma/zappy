/*
** EPITECH PROJECT, 2025
** GUI
** File description:
** Commands
*/

#include "Client.hpp"

void Client::bct_command(const std::vector<std::string> &words)
{
    //content of a tile
    if (words[0] != "bct")
        return;
    int x = std::atoi(words[1].c_str());
    int y = std::atoi(words[2].c_str());
    size_t index = x * _mapSize.y + y;
    if (index < _map->_grid.size() && _map->_grid[index]) {
        int q0 = std::atoi(words[3].c_str());
        int q1 = std::atoi(words[4].c_str());
        int q2 = std::atoi(words[5].c_str());
        int q3 = std::atoi(words[6].c_str());
        int q4 = std::atoi(words[7].c_str());
        int q5 = std::atoi(words[8].c_str());
        int q6 = std::atoi(words[9].c_str());

        _map->_grid[index]->reloadResource(q0, q1, q2, q3, q4, q5, q6);
    }
}

void Client::tna_command(const std::vector<std::string> &words)
{
    //name of all the teams
    if (words[0] != "tna")
        return;
    _teams.push_back({words[1], 0, 0});
    const int nb = std::experimental::randint(0, static_cast<int>(_skins.size() - 1));
    _teamSkins[words[1]] = _skins[nb];
    _skins.erase(_skins.begin() + nb);
    std::cout << "Team" << _teams.size() << ": " << words[1] << std::endl;
}

void Client::pnw_command(const std::vector<std::string> &words)
{
    //connection of a new player
    if (words[0] != "pnw")
        return;
    Vector2 pos;
    int id = atoi(words[1].c_str());
    pos.x = static_cast<float>(atoi(words[2].c_str()));
    pos.y = static_cast<float>(atoi(words[3].c_str()));
    std::cout << "X: " << pos.x << " Y: " << pos.y << std::endl;
    if (_teamSkins.find(words[6]) != _teamSkins.end()) {
        auto player = std::make_shared<Player>(
            id,
            words[6],
            pos,
            _mapSize,
            TILESIZE,
            Vector3{0.0f, 0.0f, 0.0f},
            _teamSkins[words[6]].c_str(),
            0.025f,
            (float)timeUnit
        );
        player->setLevel(std::atoi(words[5].c_str()));
        player->setOrientation(std::atoi(words[4].c_str()));
        _players.push_back(player);
        for (auto& team : _teams)
            if (team.name == words[6])
                team.connected_clients += 1;
    }
}

void Client::ppo_command(const std::vector<std::string> &words)
{
    //player's position
    if (words[0] != "ppo")
        return;
    int id = atoi(words[1].c_str());
    float newX = atoi(words[2].c_str());
    float newY = atoi(words[3].c_str());
    int newOrientation = atoi(words[4].c_str());

    for (const auto& player : _players) {
        if (player->getId() == id) {
                player->setOrientation(newOrientation);
                Vector2 pos = {newX, newY};
                player->setPosition(pos);
        }
    }
}


void Client::plv_command(const std::vector<std::string> &words)
{
    //player's level
    if (words[0] != "plv")
        return;
    for (const auto& player : _players) {
        if (player->getId() == atoi(words[1].c_str())) {
            player->setLevel(atoi(words[2].c_str()));
            break;
        }
    }
}

void Client::pin_command(const std::vector<std::string> &words)
{
    //player's inventory
    if (words[0] != "pin")
        return;
    int playerId = std::atoi(words[1].substr(1).c_str());
    Vector2 playerPos = { static_cast<float>(std::atoi(words[2].c_str())),
                          static_cast<float>(std::atoi(words[3].c_str())) };

    for (const auto& player : _players) {
        if (player->getId() == playerId) {
            player->setPosition(playerPos);

            player->setInventory(FOOD, atoi(words[4].c_str()));
            player->setInventory(LINEMATE, atoi(words[5].c_str()));
            player->setInventory(DERAUMERE, atoi(words[6].c_str()));
            player->setInventory(SIBUR, atoi(words[7].c_str()));
            player->setInventory(MENDIANE, atoi(words[8].c_str()));
            player->setInventory(PHIRAS, atoi(words[9].c_str()));
            player->setInventory(THYSTAME, atoi(words[10].c_str()));
            break;
        }
    }
}

void Client::pdi_command(const std::vector<std::string> &words)
{
    //death of a player
    if (words[0] != "pdi")
        return;
    int id = atoi(words[1].c_str());
    std::string teamName;
    _players.erase(
    std::remove_if(_players.begin(), _players.end(),
        [id, &teamName](const std::shared_ptr<Player> &player) {
            teamName = player->getTeamName();
            return player->getId() == id;
        }),
    _players.end()
    );
    for (auto& team : _teams)
        if (team.name == teamName)
            team.connected_clients -= 1;
}

void Client::pdr_command(const std::vector<std::string> &words)
{
    //ressource dropping
    if (words[0] != "pdr")
        return;
    for (const auto& player: _players) {
        if (player->getId() == atoi(words[1].c_str())) {
            Type type = whichType(atoi(words[2].c_str()));
            player->resourceDrop(type);
            Vector2 pos = player->getPosition();
            size_t index = pos.y * _mapSize.x + pos.x;
            if (index < _map->_grid.size() && _map->_grid[index]) {
                _map->_grid[index]->addResource(type, pos);
            }
            player->playPickupAnimation();
        }
    }
}

void Client::pgt_command(const std::vector<std::string> &words)
{
    //ressource collecting
    if (words[0] != "pgt")
        return;
    for (const auto& player: _players) {
        if (player->getId() == atoi(words[1].c_str())) {
            Type type = whichType(atoi(words[2].c_str()));
            Vector2 pos = player->getPosition();
            int gridX = static_cast<int>(std::round(pos.x));
            int gridY = static_cast<int>(std::round(pos.y));
            std::cout << "Player position: " << gridX << " " << gridY << std::endl;
            player->addInventory(type, Vector2{(float)gridX, (float)gridY});
            size_t index = gridY * _mapSize.x + gridX;
            if (index < _map->_grid.size() && _map->_grid[index]) {
                _map->_grid[index]->popResource(type);
            }
            player->playPickupAnimation();
        }
    }
}

void Client::enw_command(const std::vector<std::string> &words)
{
    //egg was laid by player
    if (words[0] != "enw")
        return;
    for (const auto& player: _players) {
        if (player->getId() == atoi(words[2].c_str())) {
            Vector2 pos;
            pos.x = atoi(words[3].c_str());
            pos.y = atoi(words[4].c_str());
            std::cout << "Egg position: " << pos.x << " " << pos.y << std::endl;
            size_t index = pos.y * _mapSize.x + pos.x;
            if (index < _map->_grid.size() && _map->_grid[index]) {
                std::unique_ptr<Egg> newEgg = std::make_unique<Egg>(atoi(words[1].c_str()), pos);
                _map->_grid[index]->addEgg(std::move(newEgg));
            }
            for (auto& team : _teams)
                if (team.name == player->getTeamName())
                    team.nb_eggs += 1;
        }
    }
}

void Client::sst_command(const std::vector<std::string> &words)
{
    if (words[0] != "sst")
        return;
    timeUnit = atoi(words[1].c_str());
    float animDuration = 7.0f / std::max(1, timeUnit);
    for (auto& player : _players)
        player->setDuration(animDuration);
}

void Client::pic_command(const std::vector<std::string> &words)
{
    //start of incantation
    if (words[0] != "pic")
        return;
    Vector2 playerPos = {static_cast<float>(std::atoi(words[1].c_str())),
        static_cast<float>(std::atoi(words[2].c_str()))};
    int id = atoi(words[4].c_str());

    for (const auto& player : _players)
        if (player->getId() == id) {
            player->setIncantation(true);
            player->setPosition(playerPos);
        }
    for (size_t i = 5; i < words.size(); ++i) {
        id = std::atoi(words[i].c_str());
        for (const auto& player : _players)
            if (player->getId() == id) {
                player->setIncantation(true);
            }
    }
}

void Client::pie_command(const std::vector<std::string> &words)
{
    if (words[0] != "pie")
        return;
    int x = std::atoi(words[1].c_str());
    int y = std::atoi(words[2].c_str());

    for (const auto& player : _players)
        if (player->checkPosition(x, y))
            player->setIncantation(false);
}

void Client::sgt_command(const std::vector<std::string> &words)
{
    if (words[0] != "sgt")
        return;
    timeUnit = atoi(words[1].c_str());
    float animDuration = 7.0f / std::max(1, timeUnit);
    for (auto& player : _players)
        player->setDuration(animDuration);
}

void Client::pbc_command(const std::vector<std::string> &words)
{
    //broadcast
    if (words[0] != "pbc")
        return;

    int id = atoi(words[1].c_str());
    std::string message = words[2];

    for (const auto& player : _players) {
        if (player->getId() == id) {
            player->setBroadcast(message.c_str());
        }
    }

    if (_messages.size() >= 7) {
        _messages.erase(_messages.begin());
    }
    _messages.push_back({id, message});

    _broadcasting = true;
    _broadcastingStartTime = std::chrono::steady_clock::now();
}

void Client::pfk_command(const std::vector<std::string> &words)
{
    //egg laying by the player
    if (words[0] != "pfk")
        return;

    int id = atoi(words[1].c_str());

    for (const auto& player : _players) {
        if (player->getId() == id) {
        }
    }

    if (_messages.size() >= 7) {
        _messages.erase(_messages.begin());
    }
    _messages.push_back({id, "*laid an egg*"});

    _broadcasting = true;
    _broadcastingStartTime = std::chrono::steady_clock::now();
}

void Client::edi_command(const std::vector<std::string> &words)
{
    // death of an egg
    if (words[0] != "edi")
        return;

    int id = atoi(words[1].c_str());

    for (const auto& tile : _map->_grid) {
        auto& eggs = tile->getEggs();
        for (auto it = eggs.begin(); it != eggs.end(); ++it) {
            if ((*it)->getId() == id) {
                eggs.erase(it);
                return;
            }
        }
    }
}

void Client::ebo_command(const std::vector<std::string> &words)
{
    //player connection for an egg
    if (words[0] != "edi")
        return;

    int id = atoi(words[1].c_str());

    for (const auto& tile : _map->_grid) {
        auto& eggs = tile->getEggs();
        for (auto it = eggs.begin(); it != eggs.end(); ++it) {
            if ((*it)->getId() == id) {
                eggs.erase(it);
                return;
            }
        }
    }
}

void Client::suc_command(const std::vector<std::string> &words)
{
    //unknown command
    if (words[0] != "suc")
        return;

    std::cout << "Command unknown." << std::endl;

}

void Client::sbp_command(const std::vector<std::string> &words)
{
    //command parameter
    if (words[0] != "sbp")
        return;

    std::cout << "Command parameter." << std::endl;
}

void Client::seg_command(const std::vector<std::string> &words)
{
    //end game
    if (words[0] != "seg")
        return;

    _winningTeam = words[1].c_str();
    _isGameOver = true;

}

void Client::pex_command(const std::vector<std::string> &words)
{
    // expulsion of a player
    if (words[0] != "pex")
        return;

    int id = atoi(words[1].c_str());

    auto it = std::remove_if(_players.begin(), _players.end(),
        [id](const std::shared_ptr<Player>& player) {
            return player->getId() == id;
        });

    if (it != _players.end()) {
        _players.erase(it, _players.end());
    }
}
