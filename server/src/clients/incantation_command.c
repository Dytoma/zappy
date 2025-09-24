/*
** EPITECH PROJECT, 2025
** B-YEP-400-TLS-4-1-zappy-dytoma.batogouma
** File description:
** incantation_command
*/

#include "../include/process_commands.h"
#include "../../include/gui_commands.h"

static int count_same_level_players(server_t *server, client_t *client)
{
    tile_t *tile = &server->world.map[client->y][client->x];
    int count = 0;

    for (int i = 0; i < tile->player_count; i++) {
        if (tile->players[i] && tile->players[i]->level == client->level &&
            tile->players[i]->is_alive) {
            count++;
        }
    }
    return count;
}

const elevation_req_t *get_elevation_requirements(client_t *client)
{
    return &elevation_requirements[client->level - 1];
}

static void send_elevation_start(client_t *client)
{
    char response[256] = "Elevation underway\n";

    printf("Sending elevation start to client %d: %s\n", client->id, response);
    if (sent(client->fd, response, strlen(response), 0) == -1) {
        printf("Error: Failed to send elevation start to client %d\n",
            client->id);
        perror("send");
    }
}

static void lock_players(server_t *server, client_t *client, int nb_players)
{
    tile_t *tile = &server->world.map[client->y][client->x];
    const int id = client->id;
    int locked_players = 0;

    for (int i = 0; i < tile->player_count &&
        locked_players < nb_players - 1; i++) {
        if (tile->players[i] && tile->players[i]->level == client->level &&
            tile->players[i]->is_alive && tile->players[i]->id != id) {
            tile->players[i]->scheduled_time = client->scheduled_time;
            tile->players[i]->locked_in = true;
            locked_players++;
        }
    }
}

int start_incantation(server_t *server, client_t *client,
    uint64_t current_time, command_t *cmd)
{
    const elevation_req_t *req = NULL;
    int same_level_players = 0;

    if (!is_incantation_allowed(server, client))
        return -1;
    req = get_elevation_requirements(client);
    same_level_players = count_same_level_players(server, client);
    if (!are_requirements_met(server, client, req, same_level_players))
        return -1;
    notify_incantation_start(server, client->x, client->y, client->level);
    send_elevation_start(client);
    client->is_incantation = true;
    client->scheduled_time = current_time + cmd->duration;
    lock_players(server, client, req->nb_players);
    return 0;
}
