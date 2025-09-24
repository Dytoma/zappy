/*
** EPITECH PROJECT, 2025
** B-YEP-400-TLS-4-1-zappy-dytoma.batogouma
** File description:
** incantation_utils
*/

#include "../include/process_commands.h"

static bool is_incantation_valid(server_t *server, client_t *client)
{
    if (!server || !client)
        return false;
    if (!client->is_alive)
        return false;
    if (client->level < 1 || client->level > 7)
        return false;
    return true;
}

bool is_incantation_allowed(server_t *server, client_t *client)
{
    if (!is_incantation_valid(server, client)) {
        send_ko_response(client);
        return false;
    }
    return true;
}

static bool check_player_requirements(const elevation_req_t *req,
    int same_level_players)
{
    if (same_level_players < req->nb_players) {
        printf("Incantation failed: not enough players (need %d, have %d)\n",
            req->nb_players, same_level_players);
        return false;
    }
    return true;
}

static bool check_resource_requirements(server_t *server, client_t *client,
    const elevation_req_t *req)
{
    const int x = client->x;
    const int y = client->y;

    if (server->world.map[y][x].resource_count[1] < req->linemate)
        return false;
    if (server->world.map[y][x].resource_count[2] < req->deraumere)
        return false;
    if (server->world.map[y][x].resource_count[3] < req->sibur)
        return false;
    if (server->world.map[y][x].resource_count[4] < req->mendiane)
        return false;
    if (server->world.map[y][x].resource_count[5] < req->phiras)
        return false;
    if (server->world.map[y][x].resource_count[6] < req->thystame)
        return false;
    return true;
}

bool are_requirements_met(server_t *server, client_t *client,
    const elevation_req_t *req, int same_level_players)
{
    if (!check_player_requirements(req, same_level_players)) {
        send_ko_response(client);
        return false;
    }
    if (!check_resource_requirements(server, client, req)) {
        printf("Incantation failed: insufficient resources\n");
        send_ko_response(client);
        return false;
    }
    return true;
}

void send_ko_response(client_t *client)
{
    const char *ko_response = "ko\n";

    sent(client->fd, ko_response, strlen(ko_response), 0);
}
