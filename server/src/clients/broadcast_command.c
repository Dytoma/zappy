/*
** EPITECH PROJECT, 2025
** server
** File description:
** broadcast_command
*/

#include "../../include/server.h"
#include <math.h>

static int direction(const int dx, const int dy)
{
    if (dx == 0 && dy == 0)
        return 0;
    if (dx == 0 && dy < 0)
        return 1;
    if (dx < 0 && dy < 0)
        return 2;
    if (dx < 0 && dy == 0)
        return 3;
    if (dx < 0 && dy > 0)
        return 4;
    if (dx == 0 && dy > 0)
        return 5;
    if (dx > 0 && dy > 0)
        return 6;
    if (dx > 0 && dy == 0)
        return 7;
    if (dx > 0 && dy < 0)
        return 8;
    return 0;
}

static void set_direction_range(const move_direction_t dir, int *dx, int *dy)
{
    const int x = *dx;
    const int y = *dy;

    switch (dir) {
        case RIGHT:
            *dx = y;
            *dy = -x;
            break;
        case DOWN:
            *dx = -x;
            *dy = -y;
            break;
        case LEFT:
            *dx = -y;
            *dy = x;
            break;
        default:
            break;
    }
}

int get_direction(server_t *server, client_t *receiver, client_t *emitter)
{
    int dx = (emitter->x - receiver->x + server->world.width) %
        server->world.width;
    int dy = (emitter->y - receiver->y + server->world.height) %
        server->world.height;

    if ((float)dx > (float)server->world.width / 2.0)
        dx -= server->world.width;
    if ((float)dy > (float)server->world.height / 2.0)
        dy -= server->world.height;
    set_direction_range(receiver->direction, &dx, &dy);
    return direction(dx, dy);
}

void spread_broadcast(server_t *server, client_t *client, char *command)
{
    char broadcast_msg[BUFFER_SIZE];

    if (!server || !client || !command)
        return;
    for (int i = 0; server->clients[i] != NULL; i++) {
        if (server->clients[i]->is_graphic)
            continue;
        snprintf(broadcast_msg, BUFFER_SIZE, "message %d, %s\n",
            get_direction(server, server->clients[i], client), command);
        sent(server->clients[i]->fd, broadcast_msg, strlen(broadcast_msg), 0);
    }
}

static void send_message_to_ui(server_t *server, client_t *client,
    char *broadcast_msg)
{
    char msg[BUFFER_SIZE];

    if (!broadcast_msg)
        return;
    snprintf(msg, BUFFER_SIZE, "pbc #%d %s\n", client->id, broadcast_msg);
    for (int i = 0; server->clients[i] != NULL; i++)
        if (server->clients[i]->is_graphic) {
            sent(server->clients[i]->fd, msg, strlen(msg), 0);
        }
}

int broadcast_command(server_t *server, client_t *client, command_t *cmd)
{
    char *text = strdup(cmd->command + 10);
    const char *response = "ok\n";

    if (!server || !client || !text) {
        free(text);
        return -1;
    }
    sent(client->fd, response, strlen(response), 0);
    spread_broadcast(server, client, text);
    send_message_to_ui(server, client, text);
    free(text);
    return 0;
}
