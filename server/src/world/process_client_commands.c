/*
** EPITECH PROJECT, 2025
** B-YEP-400-TLS-4-1-zappy-dytoma.batogouma
** File description:
** process_client_commands
*/

#include "gui_commands.h"
#include "process_commands.h"
#include "server.h"

static void execute_command(server_t *server, client_t *client,
    command_t *cmd)
{
    if (move_commands(server, client, cmd) == 0)
        return;
    if (other_commands(server, client, cmd) == 0)
        return;
    process_gui_cmd(server, client, cmd);
}

static void remove_resources(server_t *server, client_t *client)
{
    const elevation_req_t *req = &elevation_requirements[client->level - 1];
    const int x = client->x;
    const int y = client->y;

    printf("\033[93mStarting resource removal on (%d, %d)\033[0m\n", x, y);
    for (int i = 0; i < req->linemate; i++)
        remove_resource_from_tile(&server->world, x, y, LINEMATE);
    for (int i = 0; i < req->deraumere; i++)
        remove_resource_from_tile(&server->world, x, y, DERAUMERE);
    for (int i = 0; i < req->sibur; i++)
        remove_resource_from_tile(&server->world, x, y, SIBUR);
    for (int i = 0; i < req->mendiane; i++)
        remove_resource_from_tile(&server->world, x, y, MENDIANE);
    for (int i = 0; i < req->phiras; i++)
        remove_resource_from_tile(&server->world, x, y, PHIRAS);
    for (int i = 0; i < req->thystame; i++)
        remove_resource_from_tile(&server->world, x, y, THYSTAME);
    for (int i = 0; i < server->client_count; i++)
        if (server->clients[i] && server->clients[i]->is_graphic)
            map_content(server, server->clients[i], NULL);
}

static void handle_incantation_completion(server_t *server,
    client_t *client, uint64_t current_time)
{
    command_t *cmd = client->command_queue;

    printf("Incantation completed for client %d\n", client->id);
    client->is_incantation = false;
    remove_resources(server, client);
    elevate_players(server, client);
    notify_incantation_end(server, client->x, client->y, 1);
    client->scheduled_time = get_current_time();
    if (cmd) {
        printf("Processing next command '%s' after incantation\n",
            cmd->command);
        client->scheduled_time = current_time;
    }
}

static void execute_and_cleanup_command(server_t *server,
    client_t *client, command_t *cmd, uint64_t current_time)
{
    printf("Executing \033[94m%s\033[0m\n", cmd->command);
    if (strcmp(cmd->command, "Incantation") == 0) {
        start_incantation(server, client, current_time, cmd);
    } else {
        execute_command(server, client, cmd);
        client->scheduled_time = current_time + cmd->duration;
    }
    client->command_queue = cmd->next;
    client->queue_size--;
    free(cmd->command);
    free(cmd);
}

void process_client_commands(server_t *server, client_t *client)
{
    uint64_t current_time = get_current_time();
    command_t *cmd = client->command_queue;

    if (client->is_alive == false) {
        client->scheduled_time = 0;
        return;
    }
    if (client->is_incantation) {
        if (current_time >= client->scheduled_time) {
            handle_incantation_completion(server, client, current_time);
        }
    }
    if (!cmd) {
        return;
    }
    if (current_time >= client->scheduled_time) {
        execute_and_cleanup_command(server, client, cmd, current_time);
    }
}
