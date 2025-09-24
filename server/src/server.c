/*
** EPITECH PROJECT, 2025
** B-YEP-400-TLS-4-1-zappy-dytoma.batogouma
** File description:
** Server implementation with main loop and client management
*/

#include "server.h"
#include "process_commands.h"

static int signal_state(const int signum)
{
    static int state = 1;

    if (signum == SIGINT || signum == SIGTERM)
        state = 0;
    return state;
}

static void signal_handler(int sig)
{
    (void)sig;
    signal_state(sig);
}

static int handle_clients(server_t *server, int i)
{
    if (i < 0 || i >= server->poll_count)
        return i;
    if (server->pollfds[i].revents & POLLIN) {
        if (handle_client_data(server, i) == -1) {
            return (i > 0) ? i - 1 : 0;
        }
    }
    return i;
}

static int process_line_command(server_t *server, client_t *client,
    char *line_start, int *processed_len)
{
    char *line_end = NULL;

    line_end = strchr(line_start, '\n');
    if (!line_end)
        return 0;
    *line_end = '\0';
    if (process_command(server, client, line_start) == -1)
        return -1;
    *processed_len = line_end - line_start + 1;
    return 1;
}

int process_client_buffer(server_t *server, client_t *client)
{
    char *line_start = client->buffer;
    int processed_len = 0;
    int result = process_line_command(server, client, line_start,
        &processed_len);

    while (result > 0) {
        memmove(client->buffer, line_start + processed_len,
                client->buffer_pos - processed_len);
        client->buffer_pos -= processed_len;
        client->buffer[client->buffer_pos] = '\0';
        line_start = client->buffer;
        result = process_line_command(server, client, line_start,
        &processed_len);
    }
    return result;
}

int handle_poll_events(server_t *server)
{
    int poll_result = 0;
    const int timeout = compute_poll_timeout(server);
    int i = 0;

    poll_result = poll(server->pollfds, server->poll_count, timeout);
    if (poll_result == -1) {
        if (errno == EINTR)
            return 0;
        return error_handler(ERR_POLL_ERROR, "poll failed");
    }
    if (server->pollfds[0].revents & POLLIN) {
        if (accept_client(server) == -1)
            log_error(ERR_SOCKET_ACCEPT, "Failed to accept client");
    }
    i = 1;
    while (i < server->poll_count)
        i = handle_clients(server, i) + 1;
    update_world(server);
    return 0;
}

void seed_random(void)
{
    struct timeval tv;

    gettimeofday(&tv, NULL);
    srand(tv.tv_sec ^ tv.tv_usec);
}

int run_server(server_t *server)
{
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
    seed_random();
    server->running = true;
    server->start_time = get_current_time();
    printf("Server listening on port %d\n", server->port);
    while (server->running && signal_state(1)) {
        if (handle_poll_events(server) == -1)
            return -1;
    }
    server->running = false;
    return 0;
}

uint64_t get_current_time(void)
{
    struct timeval tv;

    if (gettimeofday(&tv, NULL) == 0) {
        return (uint64_t)tv.tv_sec * 1000 + (tv.tv_usec + 500) / 1000;
    }
    return (uint64_t)time(NULL) * 1000;
}
