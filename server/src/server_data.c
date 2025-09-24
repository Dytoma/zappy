/*
** EPITECH PROJECT, 2025
** B-YEP-400-TLS-4-1-zappy-dytoma.batogouma
** File description:
** server_data
*/

#include "server.h"
#include "process_commands.h"

static ssize_t read_client_data(int client_fd, char *buffer,
    size_t buffer_size)
{
    ssize_t bytes_read = recv(client_fd, buffer, buffer_size - 1, 0);

    if (bytes_read < 0) {
        if (errno == EAGAIN || errno == EWOULDBLOCK)
            return 0;
        perror("recv failed");
        return -1;
    }
    if (bytes_read == 0)
        return -2;
    buffer[bytes_read] = '\0';
    return bytes_read;
}

static int append_to_client_buffer(client_t *client, const char *data,
    size_t data_len)
{
    if (client->buffer_pos + data_len >= BUFFER_SIZE)
        return error_handler(ERR_BUFFER_OVERFLOW, "Client buffer overflow");
    strcat(client->buffer + client->buffer_pos, data);
    client->buffer_pos += data_len;
    return 0;
}

static int handle_client_disconnect(server_t *server, int client_idx,
    int client_id)
{
    printf("Client %d closed connection\n", client_id);
    disconnect_client(server, client_idx);
    return -1;
}

static client_t *find_client_by_fd(server_t *server, int fd)
{
    int i = 0;

    for (i = 0; i < server->client_count; i++) {
        if (server->clients[i] && server->clients[i]->fd == fd)
            return server->clients[i];
    }
    return NULL;
}

int handle_client_data(server_t *server, int client_idx)
{
    client_t *client = NULL;
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read = 0;

    client = find_client_by_fd(server, server->pollfds[client_idx].fd);
    if (!client)
        return -1;
    bytes_read = read_client_data(client->fd, buffer, BUFFER_SIZE);
    if (bytes_read == -1) {
        disconnect_client(server, client_idx);
        return -1;
    }
    if (bytes_read == 0)
        return 0;
    if (bytes_read == -2)
        return handle_client_disconnect(server, client_idx, client->id);
    if (append_to_client_buffer(client, buffer, bytes_read) < 0)
        return -1;
    return process_client_buffer(server, client);
}
