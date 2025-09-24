/*
** EPITECH PROJECT, 2025
** B-YEP-400-TLS-4-1-zappy-dytoma.batogouma
** File description:
** command_utils
*/

#include "../include/process_commands.h"

void remove_command(client_t *client)
{
    command_t *temp = NULL;

    if (client->command_queue == NULL)
        return;
    temp = client->command_queue->next;
    free(client->command_queue->command);
    free(client->command_queue);
    client->command_queue = temp;
}

// computing the timeout of the poll function
// using a scheduled time for better visibility
int compute_poll_timeout(const server_t *server)
{
    long min_remaining_ms = -1;
    const uint64_t now = get_current_time();
    long remaining = 0;

    for (int i = 0; i < server->client_count; ++i) {
        if (server->clients[i]->is_graphic) {
            remaining = 0;
            continue;
        }
        remaining = server->clients[i]->scheduled_time - now;
        if (remaining < 0)
            remaining = 0;
        if (server->clients[i]->life_units < remaining)
            remaining = server->clients[i]->life_units;
        if (min_remaining_ms == -1 || remaining < min_remaining_ms)
            min_remaining_ms = remaining;
    }
    return min_remaining_ms == -1 ? -1 : (int)min_remaining_ms;
}
