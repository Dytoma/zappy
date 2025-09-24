/*
** EPITECH PROJECT, 2025
** server
** File description:
** process_commands
*/

#include "process_commands.h"

static int send_team_response(server_t *server, client_t *client)
{
    char response[256];

    snprintf(response, sizeof(response), "%d\n%d %d\n",
        server->teams[client->team_id].available_slots,
        server->world.width, server->world.height);
    sent(client->fd, response, strlen(response), 0);
    client->is_graphic = false;
    return 0;
}

static int handle_team_assignment(server_t *server, client_t *client,
    char *clean_cmd)
{
    const char *ko_response = "ko\n";

    if (client->team_name || client->is_graphic)
        return 0;
    if (find_team_by_name(server, clean_cmd) != -1) {
        if (add_client_to_team(server, client, clean_cmd) == 0) {
            send_team_response(server, client);
        } else {
            sent(client->fd, ko_response, strlen(ko_response), 0);
            return 0;
        }
    } else {
        sent(client->fd, ko_response, strlen(ko_response), 0);
        return 0;
    }
    free(clean_cmd);
    return 1;
}

static int handle_graphic_command(client_t *client, char *clean_cmd)
{
    const char *response = "ok\n";

    if (strcmp(clean_cmd, "GRAPHIC") == 0) {
        client->is_graphic = true;
        sent(client->fd, response, strlen(response), 0);
        free(clean_cmd);
        return 1;
    }
    return 0;
}

static int get_other_command_duration(const char *command, int freq)
{
    if (strncmp(command, "Take", 4) == 0)
        return 7 * (1000 / freq);
    if (strncmp(command, "Set", 3) == 0)
        return 7 * (1000 / freq);
    if (strncmp(command, "Broadcast", 9) == 0)
        return 7 * (1000 / freq);
    if (strcmp(command, "Fork") == 0)
        return 42 * (1000 / freq);
    if (strcmp(command, "Eject") == 0)
        return 7 * (1000 / freq);
    if (strcmp(command, "Incantation") == 0)
        return 300 * (1000 / freq);
    return 0;
}

static int add_command_to_queue(client_t *client, const char *command,
    int duration)
{
    command_t *new_cmd = NULL;
    command_t *current = NULL;

    if (!client || !command || client->queue_size >= MAX_COMMAND_QUEUE)
        return -1;
    new_cmd = malloc(sizeof(command_t));
    if (!new_cmd)
        return -1;
    new_cmd->command = strdup(command);
    if (!new_cmd->command) {
        free(new_cmd);
        return -1;
    }
    add_command_to_client(new_cmd, current, duration, client);
    return 0;
}

int queue_client_command(client_t *client, char *cmd, int duration)
{
    const char *ko_response = "ko\n";

    if (add_command_to_queue(client, cmd, duration) == -1) {
        sent(client->fd, ko_response, strlen(ko_response), 0);
        return -1;
    }
    return 0;
}

int get_command_duration(const char *command, int freq)
{
    if (strcmp(command, "Forward") == 0)
        return 7 * (1000 / freq);
    if (strcmp(command, "Right") == 0)
        return 7 * (1000 / freq);
    if (strcmp(command, "Left") == 0)
        return 7 * (1000 / freq);
    if (strcmp(command, "Look") == 0)
        return 7 * (1000 / freq);
    if (strcmp(command, "Inventory") == 0)
        return 1 * (1000 / freq);
    if (strcmp(command, "Connect_nbr") == 0)
        return 0;
    return get_other_command_duration(command, freq);
}

int try_handle_immediate_commands(server_t *server,
    client_t *client, char *cmd)
{
    if (handle_graphic_command(client, cmd)) {
        return 1;
    }
    if (handle_team_assignment(server, client, cmd)) {
        return 1;
    }
    return 0;
}

int handle_empty_command(char *cmd)
{
    if (strlen(cmd) == 0) {
        free(cmd);
        return 1;
    }
    return 0;
}

int process_command(server_t *server, client_t *client, const char *command)
{
    char *clean_cmd = NULL;
    int duration = 0;

    if (!server || !client || !command)
        return -1;
    clean_cmd = clean_command(command);
    if (!clean_cmd)
        return -1;
    if (handle_empty_command(clean_cmd))
        return 0;
    if (try_handle_immediate_commands(server, client, clean_cmd))
        return 0;
    duration = get_command_duration(clean_cmd, server->freq);
    if (queue_client_command(client, clean_cmd, duration) == -1) {
        free(clean_cmd);
        return -1;
    }
    free(clean_cmd);
    return 0;
}
