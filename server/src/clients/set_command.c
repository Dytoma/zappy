/*
** EPITECH PROJECT, 2025
** B-YEP-400-TLS-4-1-zappy-dytoma.batogouma
** File description:
** set_command
*/

#include "../../include/process_commands.h"

static bool is_valid_set_request(server_t *server, client_t *client,
    const char *object)
{
    return server && client && object && client->is_alive;
}

static bool has_resource(client_t *client, int resource_type)
{
    return client->inventory.resources[resource_type] > 0;
}

static void send_error(client_t *client)
{
    const char *error_msg = "ko\n";

    if (client)
        sent(client->fd, error_msg, strlen(error_msg), 0);
}

static void add_resource_to_tile(world_t *world, int x, int y,
    int resource_type)
{
    resource_t *new_resource = NULL;

    if (!world || !world->map || x < 0 || x >= world->width ||
        y < 0 || y >= world->height || resource_type < 0 ||
        resource_type >= MAX_RESOURCES)
        return;
    new_resource = malloc(sizeof(resource_t));
    if (!new_resource)
        return;
    new_resource->type = resource_type;
    new_resource->x = x;
    new_resource->y = y;
    new_resource->next = world->map[y][x].resources[resource_type];
    world->map[y][x].resources[resource_type] = new_resource;
    world->map[y][x].resource_count[resource_type]++;
}

static void send_message_to_ui(server_t *server, client_t *client,
    int resource_type)
{
    char msg[BUFFER_SIZE];

    if (!server || !client)
        return;
    snprintf(msg, BUFFER_SIZE, "pdr #%d %d\n", client->id, resource_type);
    for (int i = 0; server->clients[i] != NULL; i++)
        if (server->clients[i]->is_graphic) {
            sent(server->clients[i]->fd, msg, strlen(msg), 0);
        }
}

static void send_success_and_log(client_t *client, const char *object,
    int resource_type)
{
    const char *success_msg = "ok\n";

    sent(client->fd, success_msg, strlen(success_msg), 0);
    printf("Client %d set %s at (%d, %d)\n", client->id, object,
        client->x, client->y);
    (void)resource_type;
}

static int set_object(server_t *server, client_t *client, const char *object)
{
    int resource_type;

    if (!is_valid_set_request(server, client, object)) {
        send_error(client);
        return -1;
    }
    resource_type = get_resource_type_from_name(object);
    if (resource_type == -1 || !has_resource(client, resource_type)) {
        send_error(client);
        return -1;
    }
    client->inventory.resources[resource_type]--;
    add_resource_to_tile(&server->world, client->x, client->y, resource_type);
    send_success_and_log(client, object, resource_type);
    send_message_to_ui(server, client, resource_type);
    return 0;
}

int execute_set_command(server_t *server, client_t *client,
    command_t *cmd)
{
    char *object_name = strchr(cmd->command, ' ');
    const char *error_msg = "ko\n";

    if (object_name) {
        object_name++;
        set_object(server, client, object_name);
        return 0;
    }
    sent(client->fd, error_msg, strlen(error_msg), 0);
    return 1;
}
