/*
** EPITECH PROJECT, 2025
** B-YEP-400-TLS-4-1-zappy-dytoma.batogouma
** File description:
** accespt_client
*/

#include "server.h"
#include <fcntl.h>
#include <errno.h>

static int setup_client_socket(int client_fd)
{
    int flags = 0;

    flags = fcntl(client_fd, F_GETFL, 0);
    fcntl(client_fd, F_SETFL, flags | O_NONBLOCK);
    return 0;
}

static void initialize_client(client_t *client, int client_fd,
    server_t *server)
{
    memset(client, 0, sizeof(client_t));
    client->fd = client_fd;
    client->id = get_unique_id(&server->gen);
    client->x = rand() % server->world.width;
    client->y = rand() % server->world.height;
    client->direction = rand() % 4;
    client->level = 1;
    client->life_units = (INITIAL_LIFE_UNITS * LIFE_UNIT_TIME * 1000) /
        server->freq;
    client->last_meal = get_current_time();
    client->scheduled_time = get_current_time();
    client->is_alive = true;
    client->buffer_pos = 0;
    client->team_name = NULL;
    client->command_queue = NULL;
    memset(&client->inventory, 0, sizeof(inventory_t));
    memset(client->buffer, 0, BUFFER_SIZE);
    client->inventory.resources[0] = client->life_units / MILLISECONDS;
    client->is_incantation = false;
    client->is_graphic = false;
}

static void setup_poll_fd(server_t *server, int client_fd)
{
    server->pollfds[server->poll_count].fd = client_fd;
    server->pollfds[server->poll_count].events = POLLIN;
    server->poll_count++;
}

static int create_and_setup_client(server_t *server, int client_fd,
    struct sockaddr_in *client_addr)
{
    client_t *client = NULL;
    const char *welcome = "WELCOME\n";

    client = malloc(sizeof(client_t));
    if (!client)
        return error_handler(ERR_CLIENT_ALLOCATION,
            "client allocation failed");
    initialize_client(client, client_fd, server);
    server->clients[server->client_count] = client;
    server->client_count++;
    setup_poll_fd(server, client_fd);
    sent(client_fd, welcome, strlen(welcome), 0);
    printf("Client %d connected from %s\n", client->id,
        inet_ntoa(client_addr->sin_addr));
    return 0;
}

static int accept_socket_connection(int server_fd,
    struct sockaddr_in *client_addr)
{
    socklen_t addr_len = sizeof(*client_addr);
    int client_fd;

    client_fd = accept(server_fd, (struct sockaddr *)client_addr, &addr_len);
    if (client_fd < 0)
        return error_handler(ERR_SOCKET_ACCEPT, "Accept failed");
    return client_fd;
}

static int check_client_capacity(server_t *server, int client_fd)
{
    if (server->client_count >= MAX_CLIENTS) {
        sent(client_fd, "ko\n", 3, 0);
        close(client_fd);
        return -1;
    }
    return 0;
}

static int set_socket_nonblocking(int client_fd)
{
    int flags;

    flags = fcntl(client_fd, F_GETFL, 0);
    if (flags == -1)
        return error_handler(ERR_SOCKET_CONF, "Failed to get socket flags");
    if (fcntl(client_fd, F_SETFL, flags | O_NONBLOCK) == -1)
        return error_handler(ERR_SOCKET_CONF,
            "Failed to set non-blocking mode");
    return 0;
}

static int configure_socket_options(int client_fd)
{
    int opt = 1;

    if (setsockopt(client_fd, SOL_SOCKET, SO_KEEPALIVE, &opt, sizeof(opt)) < 0)
        return error_handler(ERR_SOCKET_CONF, "Failed to set keepalive");
    return 0;
}

static void cleanup_failed_client(int client_fd)
{
    if (client_fd >= 0)
        close(client_fd);
}

int accept_client(server_t *server)
{
    struct sockaddr_in client_addr;
    int client_fd;

    client_fd = accept_socket_connection(server->socket_fd, &client_addr);
    if (client_fd < 0)
        return client_fd;
    setup_client_socket(client_fd);
    if (check_client_capacity(server, client_fd) < 0)
        return 0;
    if (set_socket_nonblocking(client_fd) < 0) {
        cleanup_failed_client(client_fd);
        return -1;
    }
    if (configure_socket_options(client_fd) < 0) {
        cleanup_failed_client(client_fd);
        return -1;
    }
    return create_and_setup_client(server, client_fd, &client_addr);
}
