/*
** EPITECH PROJECT, 2025
** server
** File description:
** client_management
*/

#include "process_commands.h"

void add_command_to_client(command_t *new_cmd, command_t *current,
    int duration, client_t *client)
{
    new_cmd->start_time = get_current_time();
    new_cmd->duration = duration;
    new_cmd->next = NULL;
    if (!client->command_queue) {
        new_cmd->scheduled_time = get_current_time();
        client->command_queue = new_cmd;
    } else {
        current = client->command_queue;
        while (current->next)
            current = current->next;
        new_cmd->scheduled_time = current->scheduled_time +
            current->duration;
        current->next = new_cmd;
    }
    client->queue_size++;
}

char *clean_command(const char *command)
{
    char *clean_cmd = NULL;
    int len = 0;

    clean_cmd = strdup(command);
    if (!clean_cmd)
        return NULL;
    len = strlen(clean_cmd);
    while (len > 0 && (clean_cmd[len - 1] == '\r' ||
        clean_cmd[len - 1] == '\n' ||
        clean_cmd[len - 1] == ' ')) {
        --len;
        clean_cmd[len] = '\0';
        }
    return clean_cmd;
}
