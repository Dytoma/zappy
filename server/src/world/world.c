/*
** EPITECH PROJECT, 2025
** B-YEP-400-TLS-4-1-zappy-dytoma.batogouma
** File description:
** world management functions for the zappy server
*/

#include "gui_commands.h"
#include "process_commands.h"
#include "server.h"

static void spawn_single_resource(world_t *world, int resource_type)
{
    int x = rand() % world->width;
    int y = rand() % world->height;
    resource_t *new_resource = malloc(sizeof(resource_t));

    if (!new_resource)
        return;
    new_resource->type = resource_type;
    new_resource->x = x;
    new_resource->y = y;
    new_resource->next = world->map[y][x].resources[resource_type];
    world->map[y][x].resources[resource_type] = new_resource;
    world->map[y][x].resource_count[resource_type]++;
    world->filled = true;
}

void spawn_resources(world_t *world, const int freq)
{
    const uint64_t current_time = get_current_time();
    int total_tiles = 0;
    double densities[] = {0.5, 0.3, 0.15, 0.1, 0.1, 0.08, 0.05};
    int r_count = 0;

    if (!world || !world->map)
        return;
    if (current_time - world->last_resource_spawn <= (uint64_t)(20000 / freq))
        return;
    total_tiles = world->width * world->height;
    for (int r_type = 0; r_type < MAX_RESOURCES; r_type++) {
        r_count = (int)(total_tiles * densities[r_type]) -
            count_resource_around(world, r_type);
        if (r_count <= 0)
            continue;
        printf("Filling resource type %d and quantity %d\n", r_type, r_count);
        for (int i = 0; i < r_count; i++)
            spawn_single_resource(world, r_type);
    }
    world->last_resource_spawn = current_time;
}

static void handle_client_death(server_t *server, client_t *client)
{
    const char *death_msg = "dead\n";
    const int fd = client->fd;
    const int id = client->id;

    if (client->is_graphic)
        return;
    client->is_alive = false;
    client->life_units = 0;
    sent(client->fd, death_msg, strlen(death_msg), 0);
    send_client_death_message(server, client);
    remove_client(server, client);
    remove_client_pollfd(server, fd);
    cleanup_server_client(client);
    printf("\033[31mClient %d died of starvation\033[0m\n", id);
}

static void update_client_life(server_t *server, client_t *client)
{
    uint64_t current_time = 0;
    int life_units_lost = 0;

    if (!client || !client->is_alive || client->is_graphic)
        return;
    current_time = get_current_time();
    life_units_lost = current_time - client->last_meal;
    if (life_units_lost > 0) {
        client->life_units -= life_units_lost;
        client->inventory.resources[0] = client->life_units / MILLISECONDS;
        client->last_meal = current_time;
        if (client->life_units <= 0)
            handle_client_death(server, client);
    }
}

static void notify_filling_resources(server_t *server)
{
    for (int i = 0; i < server->client_count; i++) {
        if (server->clients[i] && server->clients[i]->is_graphic) {
            map_content(server, server->clients[i], NULL);
        }
    }
}

void update_world(server_t *server)
{
    if (!server)
        return;
    spawn_resources(&server->world, server->freq);
    if (server->world.filled) {
        server->world.filled = false;
        notify_filling_resources(server);
    }
    for (int i = 0; i < server->client_count; i++) {
        if (server->clients[i]) {
            update_client_life(server, server->clients[i]);
            process_client_commands(server, server->clients[i]);
        }
    }
}

int get_tile_index(world_t *world, int x, int y)
{
    if (!world || x < 0 || x >= world->width || y < 0 || y >= world->height)
        return -1;
    return y * world->width + x;
}

void normalize_coordinates(world_t *world, int *x, int *y)
{
    if (!world || !x || !y)
        return;
    *x = (*x % world->width + world->width) % world->width;
    *y = (*y % world->height + world->height) % world->height;
}
