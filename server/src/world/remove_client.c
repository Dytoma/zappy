/*
** EPITECH PROJECT, 2025
** server
** File description:
** remove_client
*/

#include "../../include/server.h"

void send_client_death_message(server_t *server, client_t *client)
{
    char msg[BUFFER_SIZE] = {0};

    snprintf(msg, BUFFER_SIZE, "pdi #%d\n", client->id);
    for (int i = 0; server->clients[i] != NULL; i++)
        if (server->clients[i]->is_graphic) {
            sent(server->clients[i]->fd, msg, strlen(msg), 0);
        }
}

static void remove_player_from_tile(tile_t *tile, client_t *client)
{
    tile_t *old_tile = tile;
    int i = 0;

    for (i = 0; i < old_tile->player_count; i++) {
        if (old_tile->players[i] == client) {
            memmove(&old_tile->players[i], &old_tile->players[i + 1],
                (old_tile->player_count - i - 1) * sizeof(client_t *));
            old_tile->player_count--;
            break;
        }
    }
}

void remove_client(server_t *server, client_t *client)
{
    remove_player_from_tile(&server->world.map[client->y][client->x], client);
    for (int i = 0; i < server->client_count; i++) {
        if (server->clients[i]->id == client->id) {
            memmove(&server->clients[i], &server->clients[i + 1],
                (server->client_count - i - 1) * sizeof(client_t *));
            server->client_count -= 1;
            server->teams[client->team_id].connected_clients -= 1;
            break;
        }
    }
}

void remove_client_pollfd(server_t *server, const int fd)
{
    for (int i = 0; i < server->poll_count; i++) {
        if (server->pollfds[i].fd == fd) {
            memmove(&server->pollfds[i], &server->pollfds[i + 1],
                (server->poll_count - i - 1) * sizeof(struct pollfd));
            server->poll_count -= 1;
            shutdown(fd, SHUT_RDWR);
            close(fd);
            break;
        }
    }
}

void remove_egg(const egg_t *egg, tile_t *tile)
{
    egg_t *temp = tile->eggs;
    egg_t *prev = NULL;

    while (temp != NULL) {
        if (temp->team_id == egg->team_id && prev == NULL) {
            tile->eggs = temp->next;
            free(temp);
            return;
        }
        if (temp->team_id == egg->team_id && prev != NULL) {
            prev->next = temp->next;
            free(temp);
            return;
        }
        prev = temp;
        temp = temp->next;
    }
}

void remove_egg_from_team(team_t *team, int egg_id)
{
    egg_t *egg = team->eggs;
    egg_t *temp = egg;

    if (egg == NULL)
        return;
    if (egg->id == egg_id) {
        team->eggs = egg->next;
        free(egg);
        return;
    }
    while (egg != NULL) {
        if (egg->id == egg_id) {
            temp->next = egg->next;
            free(egg);
            return;
        }
        temp = egg;
        egg = egg->next;
    }
}
