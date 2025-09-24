/*
** EPITECH PROJECT, 2025
** B-YEP-400-TLS-4-1-zappy-dytoma.batogouma
** File description:
** take_command
*/

#include "../../include/process_commands.h"

int get_resource_type_from_name(const char *object_name)
{
    const char *resource_names[] = {
        "food", "linemate", "deraumere",
        "sibur", "mendiane", "phiras", "thystame"
    };

    if (!object_name)
        return -1;
    for (int i = 0; i < MAX_RESOURCES; i++) {
        if (strcmp(object_name, resource_names[i]) == 0)
            return i;
    }
    return -1;
}

bool remove_resource_from_tile(world_t *world, int x, int y,
    int resource_type)
{
    resource_t *current = NULL;

    if (!world || !world->map || x < 0 || x >= world->width ||
        y < 0 || y >= world->height || resource_type < 0 ||
        resource_type >= MAX_RESOURCES)
        return false;
    current = world->map[y][x].resources[resource_type];
    if (!current)
        return false;
    world->map[y][x].resources[resource_type] = current->next;
    world->map[y][x].resource_count[resource_type]--;
    free(current);
    return true;
}

static bool is_valid_request(server_t *server, client_t *client,
    const char *object)
{
    return server && client && object && client->is_alive;
}

static bool is_resource_available(server_t *server, client_t *client,
    int resource_type)
{
    return server->world.map[client->y]
        [client->x].resource_count[resource_type] > 0;
}

static void handle_error(client_t *client)
{
    const char *error_msg = "ko\n";

    if (client)
        sent(client->fd, error_msg, strlen(error_msg), 0);
}

static void send_message_to_ui(server_t *server, client_t *client,
    int resource_type)
{
    char msg[BUFFER_SIZE];

    if (!server || !client)
        return;
    snprintf(msg, BUFFER_SIZE, "pgt #%d %d\n", client->id, resource_type);
    for (int i = 0; server->clients[i] != NULL; i++)
        if (server->clients[i]->is_graphic) {
            sent(server->clients[i]->fd, msg, strlen(msg), 0);
        }
}

static void handle_success(server_t *server, client_t *client,
    const char *object, int resource_type)
{
    const char *success_msg = "ok\n";

    sent(client->fd, success_msg, strlen(success_msg), 0);
    client->inventory.resources[resource_type]++;
    if (resource_type == FOOD) {
        client->life_units += LIFE_UNIT_TIME * 1000 / server->freq;
        client->last_meal = get_current_time();
    }
    printf("Client %d took %s at (%d, %d)\n", client->id, object,
        client->x, client->y);
}

static int take_object(server_t *server, client_t *client, const char *object)
{
    int resource_type;

    if (!is_valid_request(server, client, object)) {
        handle_error(client);
        return -1;
    }
    resource_type = get_resource_type_from_name(object);
    if (resource_type == -1 || !is_resource_available(server, client,
        resource_type)) {
        handle_error(client);
        return -1;
    }
    if (!remove_resource_from_tile(&server->world, client->x,
        client->y, resource_type)) {
        handle_error(client);
        return -1;
    }
    handle_success(server, client, object, resource_type);
    send_message_to_ui(server, client, resource_type);
    return 0;
}

int execute_take_command(server_t *server, client_t *client,
    command_t *cmd)
{
    char *object_name = strchr(cmd->command, ' ');
    const char *error_msg = "ko\n";

    printf("object: %s\n", object_name);
    if (object_name) {
        object_name++;
        take_object(server, client, object_name);
        return 0;
    }
    sent(client->fd, error_msg, strlen(error_msg), 0);
    return 1;
}
