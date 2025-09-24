/*
** EPITECH PROJECT, 2025
** B-YEP-400-TLS-4-1-zappy-dytoma.batogouma
** File description:
** gui_notification_commands - CORRECTED
*/

#include "../../include/gui_commands.h"
#include "../../include/utils.h"

void notify_player_expulsion(server_t *server, int player_id)
{
    char response[BUFFER_SIZE] = {0};

    if (!server)
        return;
    snprintf(response, BUFFER_SIZE, "pex #%d\n", player_id);
    for (int i = 0; i < server->client_count; i++) {
        if (server->clients[i] && server->clients[i]->is_graphic) {
            sent(server->clients[i]->fd, response, strlen(response), 0);
        }
    }
}

static void incantation_message(char *response, server_t *server)
{
    strcat(response, "\n");
    for (int i = 0; i < server->client_count; i++) {
        if (server->clients[i] && server->clients[i]->is_graphic)
            sent(server->clients[i]->fd, response, strlen(response), 0);
    }
}

static int check_incantation(int offset, char *response, const char *temp)
{
    if (offset + strlen(temp) < BUFFER_SIZE - 2) {
        strcpy(response + offset, temp);
        offset += strlen(temp);
    }
    return offset;
}

void notify_incantation_start(server_t *server, int x, int y, int level)
{
    char response[BUFFER_SIZE] = {0};
    char temp[32] = {0};
    int offset = 0;

    if (!server)
        return;
    offset = snprintf(response, BUFFER_SIZE, "pic %d %d %d", x, y, level);
    for (int i = 0; i < server->client_count &&
        offset < BUFFER_SIZE - 10; i++) {
        if (server->clients[i] && !server->clients[i]->is_graphic &&
            server->clients[i]->is_alive && server->clients[i]->level == level
            && server->clients[i]->x == x && server->clients[i]->y == y) {
            snprintf(temp, sizeof(temp), " #%d", server->clients[i]->id);
            offset = check_incantation(offset, response, temp);
        }
    }
    incantation_message(response, server);
}

void notify_incantation_end(server_t *server, int x, int y, int result)
{
    char response[BUFFER_SIZE] = {0};

    if (!server)
        return;
    snprintf(response, BUFFER_SIZE, "pie %d %d %d\n", x, y, result);
    for (int i = 0; i < server->client_count; i++) {
        if (server->clients[i] && server->clients[i]->is_graphic) {
            sent(server->clients[i]->fd, response, strlen(response), 0);
        }
    }
}

void notify_egg_laying(server_t *server, int player_id)
{
    char response[BUFFER_SIZE] = {0};

    if (!server)
        return;
    snprintf(response, BUFFER_SIZE, "pfk #%d\n", player_id);
    for (int i = 0; i < server->client_count; i++) {
        if (server->clients[i] && server->clients[i]->is_graphic) {
            sent(server->clients[i]->fd, response, strlen(response), 0);
        }
    }
}

void notify_egg_death(server_t *server, int egg_id)
{
    char response[BUFFER_SIZE] = {0};

    if (!server)
        return;
    snprintf(response, BUFFER_SIZE, "edi #%d\n", egg_id);
    for (int i = 0; i < server->client_count; i++) {
        if (server->clients[i] && server->clients[i]->is_graphic) {
            sent(server->clients[i]->fd, response, strlen(response), 0);
        }
    }
}
