/*
** EPITECH PROJECT, 2025
** B-YEP-400-TLS-4-1-zappy-dytoma.batogouma
** File description:
** init_other
*/

#include "server.h"

int initialize_tile_row(world_t *world, int y)
{
    for (int x = 0; x < world->width; x++) {
        memset(&world->map[y][x], 0, sizeof(tile_t));
        world->map[y][x].players = calloc(MAX_CLIENTS, sizeof(client_t *));
        if (!world->map[y][x].players)
            return error_handler(ERR_MEMORY_ALLOC, "tile players allocation");
    }
    return 0;
}

int init_teams(server_t *server)
{
    for (int i = 0; i < server->nb_teams; i++) {
        server->teams[i].id = i;
        server->teams[i].max_clients = server->clients_per_team;
        server->teams[i].connected_clients = 0;
        server->teams[i].available_slots = server->clients_per_team;
        server->teams[i].eggs = NULL;
    }
    return 0;
}

int spawn_resource_type(world_t *world, int resource_type,
    int total_tiles, double density)
{
    int quantity = (int)(total_tiles * density);
    int x = 0;
    int y = 0;
    resource_t *new_resource = NULL;

    if (quantity < 1)
        quantity = 1;
    for (int i = 0; i < quantity; i++) {
        x = rand() % world->width;
        y = rand() % world->height;
        new_resource = malloc(sizeof(resource_t));
        check_null_func(new_resource, "resource allocation");
        new_resource->type = resource_type;
        new_resource->x = x;
        new_resource->y = y;
        new_resource->next = world->map[y][x].resources[resource_type];
        world->map[y][x].resources[resource_type] = new_resource;
        world->map[y][x].resource_count[resource_type]++;
    }
    return 0;
}

int init_resources(world_t *world)
{
    int total_tiles = world->width * world->height;
    double densities[] = {0.5, 0.3, 0.15, 0.1, 0.1, 0.08, 0.05};

    for (int resource_type = 0; resource_type < MAX_RESOURCES;
        resource_type++) {
        if (spawn_resource_type(world, resource_type, total_tiles,
            densities[resource_type]) != 0) {
            return -1;
        }
    }
    return 0;
}

// Check if we shouldn't limit the number of resources on a tile
// and add a recursive call or a loop to look for an empty tile to place
// the resource
static int fill_resource_type(world_t *world, int resource_type, int quantity)
{
    int x = 0;
    int y = 0;
    resource_t *new_resource = NULL;

    if (quantity < 1)
        return 1;
    for (int i = 0; i < quantity; i++) {
        x = rand() % world->width;
        y = rand() % world->height;
        new_resource = malloc(sizeof(resource_t));
        check_null_func(new_resource, "resource allocation");
        new_resource->type = resource_type;
        new_resource->x = x;
        new_resource->y = y;
        new_resource->next = world->map[y][x].resources[resource_type];
        world->map[y][x].resources[resource_type] = new_resource;
        world->map[y][x].resource_count[resource_type]++;
    }
    return 0;
}

int fill_world_resources(server_t *server)
{
    double densities[] = {0.5, 0.3, 0.15, 0.1, 0.1, 0.08, 0.05};
    int usual_quantity = 0;

    printf("\033[33mFilling resources...\033[0m\n");
    for (int i = 0; i < MAX_RESOURCES; i++) {
        usual_quantity = (int)(server->world.width * server->world.height *
            densities[i]);
        fill_resource_type(&server->world, i, usual_quantity -
            count_resource_around(&server->world, i));
    }
    return 0;
}
