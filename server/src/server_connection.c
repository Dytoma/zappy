/*
** EPITECH PROJECT, 2025
** B-YEP-400-TLS-4-1-zappy-dytoma.batogouma
** File description:
** server_connection
*/

#include "server.h"

static int find_client_index(server_t *server, int client_fd,
    client_t **client)
{
    int i = 0;

    for (i = 0; i < server->client_count; i++) {
        if (server->clients[i] && server->clients[i]->fd == client_fd) {
            *client = server->clients[i];
            return i;
        }
    }
    *client = NULL;
    return -1;
}

static void remove_client_from_tile(server_t *server, client_t *client)
{
    tile_t *tile = NULL;
    int i = 0;

    tile = &server->world.map[client->y][client->x];
    for (i = 0; i < tile->player_count; i++) {
        if (tile->players[i] == client) {
            memmove(&tile->players[i], &tile->players[i + 1],
                (tile->player_count - i - 1) * sizeof(client_t *));
            tile->player_count--;
            break;
        }
    }
}

static void free_client_commands(client_t *client)
{
    command_t *cmd = client->command_queue;
    command_t *next = NULL;

    while (cmd) {
        next = cmd->next;
        free(cmd->command);
        free(cmd);
        cmd = next;
    }
}

static void remove_client_from_array(server_t *server, int client_array_idx)
{
    memmove(&server->clients[client_array_idx],
            &server->clients[client_array_idx + 1],
            (server->client_count - client_array_idx - 1) *
            sizeof(client_t *));
    server->client_count--;
    server->clients[server->client_count] = NULL;
}

static void remove_poll_fd(server_t *server, int poll_idx)
{
    size_t elements_to_move;

    if (poll_idx < 0 || poll_idx >= server->poll_count)
        return;
    elements_to_move = server->poll_count - poll_idx - 1;
    if (elements_to_move > 0) {
        memmove(&server->pollfds[poll_idx],
                &server->pollfds[poll_idx + 1],
                elements_to_move * sizeof(struct pollfd));
    }
    server->poll_count--;
}

static void client_disconnect(server_t *server, int client_fd, int poll_idx)
{
    if (client_fd >= 0) {
        shutdown(client_fd, SHUT_RDWR);
        close(client_fd);
    }
    remove_poll_fd(server, poll_idx);
}

void disconnect_client(server_t *server, int poll_idx)
{
    int client_fd = server->pollfds[poll_idx].fd;
    client_t *client = NULL;
    int client_array_idx = -1;

    if (poll_idx < 0 || poll_idx >= server->poll_count)
        return;
    client_array_idx = find_client_index(server, client_fd, &client);
    if (client) {
        printf("\033[31mClient %d disconnected\033[0m\n", client->id);
        send_client_death_message(server, client);
        remove_client_from_tile(server, client);
        free_client_commands(client);
        remove_client_from_array(server, client_array_idx);
        remove_poll_fd(server, poll_idx);
        free(client);
        shutdown(client_fd, SHUT_RDWR);
        close(client_fd);
    } else {
        client_disconnect(server, client_fd, poll_idx);
    }
}
