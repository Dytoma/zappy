/*
** EPITECH PROJECT, 2025
** B-YEP-400-TLS-4-1-zappy-dytoma.batogouma
** File description:
** Initialization functions for server, world, teams and resources
*/

#include "server.h"

static int create_server_socket(server_t *server)
{
    server->socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server->socket_fd == -1)
        return error_handler(ERR_SOCKET_INIT, "socket creation failed");
    return 0;
}

static int configure_socket_options(int socket_fd)
{
    int opt = 1;

    if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
            &opt, sizeof(opt)) == -1) {
        close(socket_fd);
        return error_handler(ERR_SOCKET_INIT, "setsockopt failed");
    }
    return 0;
}

static int bind_socket(server_t *server, struct sockaddr_in *addr)
{
    if (bind(server->socket_fd, (struct sockaddr *)addr,
        sizeof(*addr)) == -1) {
        close(server->socket_fd);
        return error_handler(ERR_SOCKET_BIND, "bind failed");
    }
    return 0;
}

static void setup_address(struct sockaddr_in *addr, int port)
{
    memset(addr, 0, sizeof(*addr));
    addr->sin_family = AF_INET;
    addr->sin_addr.s_addr = INADDR_ANY;
    addr->sin_port = htons(port);
}

static int start_listening(server_t *server)
{
    if (listen(server->socket_fd, MAX_CLIENTS) == -1) {
        close(server->socket_fd);
        return error_handler(ERR_SOCKET_LISTEN, "listen failed");
    }
    return 0;
}

int init_poll_structure(server_t *server)
{
    server->max_fds = MAX_CLIENTS + 1;
    server->pollfds = calloc(server->max_fds, sizeof(struct pollfd));
    check_null_func(server->pollfds, "pollfd allocation");
    server->pollfds[0].fd = server->socket_fd;
    server->pollfds[0].events = POLLIN;
    server->poll_count = 1;
    return 0;
}

int init_socket(server_t *server)
{
    struct sockaddr_in addr;

    if (create_server_socket(server) != 0)
        return -1;
    if (configure_socket_options(server->socket_fd) != 0)
        return -1;
    setup_address(&addr, server->port);
    if (bind_socket(server, &addr) != 0)
        return -1;
    if (start_listening(server) != 0)
        return -1;
    return init_poll_structure(server);
}

void free_allocated_rows(world_t *world, int max_y)
{
    for (int i = 0; i < max_y; i++)
        free(world->map[i]);
    free(world->map);
}

int allocate_world_tiles(world_t *world)
{
    for (int y = 0; y < world->height; y++) {
        world->map[y] = malloc(sizeof(tile_t) * world->width);
        if (!world->map[y]) {
            free_allocated_rows(world, y);
            return error_handler(ERR_MEMORY_ALLOC, "tile row allocation");
        }
        if (initialize_tile_row(world, y) != 0)
            return -1;
    }
    return 0;
}

int init_world(world_t *world)
{
    if (world->width <= 0 || world->height <= 0)
        return error_handler(ERR_WORLD_INIT, "Invalid world dimensions");
    world->map = malloc(sizeof(tile_t *) * world->height);
    check_null_func(world->map, "world map allocation");
    if (allocate_world_tiles(world) != 0)
        return -1;
    world->last_resource_spawn = get_current_time();
    world->filled = false;
    return 0;
}
