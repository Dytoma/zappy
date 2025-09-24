/*
** EPITECH PROJECT, 2025
** B-YEP-400-TLS-4-1-zappy-dytoma.batogouma
** File description:
** cleanup functions for server resources management
*/

#include "server.h"
#include "../include/gui_commands.h"

static void cleanup_client_commands(client_t *client)
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

void cleanup_server_client(client_t *client)
{
    if (client) {
        cleanup_client_commands(client);
        if (client->fd != -1)
            close(client->fd);
        if (client->team_name)
            free(client->team_name);
        free(client);
        client = NULL;
    }
}

void cleanup_clients(server_t *server)
{
    if (!server)
        return;
    for (int i = 0; i < server->client_count; i++) {
        cleanup_server_client(server->clients[i]);
    }
    server->client_count = 0;
}

void cleanup_teams(server_t *server)
{
    egg_t *egg = NULL;
    egg_t *next = NULL;

    if (!server)
        return;
    for (int i = 0; i < server->nb_teams; i++) {
        egg = server->teams[i].eggs;
        while (egg) {
            next = egg->next;
            notify_egg_death(server, egg->id);
            free(egg);
            egg = next;
        }
        server->teams[i].eggs = NULL;
    }
}

static void cleanup_tile_resources(tile_t *tile)
{
    resource_t *resource = NULL;
    resource_t *next = NULL;

    for (int r = 0; r < MAX_RESOURCES; r++) {
        resource = tile->resources[r];
        while (resource) {
            next = resource->next;
            free(resource);
            resource = next;
        }
    }
}

static void cleanup_tile_eggs(server_t *server, tile_t *tile)
{
    egg_t *egg = tile->eggs;
    egg_t *next = NULL;

    while (egg) {
        next = egg->next;
        notify_egg_death(server, egg->id);
        free(egg);
        egg = next;
    }
}

static void cleanup_single_tile(server_t *server, tile_t *tile)
{
    cleanup_tile_resources(tile);
    if (tile->players)
        free(tile->players);
    cleanup_tile_eggs(server, tile);
}

static void cleanup_world_height(server_t *world, int y)
{
    if (world->world.map[y]) {
        for (int x = 0; x < world->world.width; x++)
            cleanup_single_tile(world, &world->world.map[y][x]);
        free(world->world.map[y]);
    }
}

void cleanup_world(server_t *world)
{
    if (!world || !world->world.map)
        return;
    for (int y = 0; y < world->world.height; y++) {
        cleanup_world_height(world, y);
    }
    free(world->world.map);
    world->world.map = NULL;
}

void cleanup_socket(server_t *server)
{
    if (!server)
        return;
    if (server->socket_fd != -1) {
        close(server->socket_fd);
        server->socket_fd = -1;
    }
    if (server->pollfds) {
        free(server->pollfds);
        server->pollfds = NULL;
    }
}
