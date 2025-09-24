/*
** EPITECH PROJECT, 2025
** B-YEP-400-TLS-4-1-zappy-dytoma.batogouma
** File description:
** gui_mess_cmds - CORRECTED
*/

#include "../../include/gui_commands.h"
#include "../../include/utils.h"

void notify_game_end(server_t *server, const char *winning_team)
{
    char response[BUFFER_SIZE] = {0};

    if (!server || !winning_team)
        return;
    snprintf(response, BUFFER_SIZE, "seg %s\n", winning_team);
    for (int i = 0; i < server->client_count; i++) {
        if (server->clients[i] && server->clients[i]->is_graphic) {
            sent(server->clients[i]->fd, response, strlen(response), 0);
        }
    }
}

void notify_server_message(server_t *server, const char *message)
{
    char response[BUFFER_SIZE] = {0};

    if (!server || !message)
        return;
    snprintf(response, BUFFER_SIZE, "smg %s\n", message);
    for (int i = 0; i < server->client_count; i++) {
        if (server->clients[i] && server->clients[i]->is_graphic) {
            sent(server->clients[i]->fd, response, strlen(response), 0);
        }
    }
}

void notify_player_position(server_t *server, client_t *client)
{
    char response[BUFFER_SIZE] = {0};

    if (!server || !client)
        return;
    snprintf(response, BUFFER_SIZE, "ppo #%d %d %d %d\n", client->id,
        client->x, client->y, client->direction + 1);
    for (int i = 0; i < server->client_count; i++) {
        if (server->clients[i] && server->clients[i]->is_graphic) {
            sent(server->clients[i]->fd, response, strlen(response), 0);
        }
    }
}

void notify_player_level(server_t *server, client_t *client)
{
    char response[BUFFER_SIZE] = {0};

    if (!server || !client)
        return;
    snprintf(response, BUFFER_SIZE, "plv #%d %d\n", client->id,
        client->level);
    for (int i = 0; i < server->client_count; i++) {
        if (server->clients[i] && server->clients[i]->is_graphic) {
            sent(server->clients[i]->fd, response, strlen(response), 0);
        }
    }
}
