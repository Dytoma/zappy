/*
** EPITECH PROJECT, 2025
** server
** File description:
** client_movements
*/

#include "client_movements.h"

#include "gui_commands.h"

// check the calculation of the size to copy with memmove
static void remove_client_from_tile(server_t *server, client_t *client)
{
    tile_t *old_tile = NULL;
    int i = 0;

    old_tile = &server->world.map[client->y][client->x];
    for (i = 0; i < old_tile->player_count; i++) {
        if (old_tile->players[i] == client) {
            memmove(&old_tile->players[i], &old_tile->players[i + 1],
                (old_tile->player_count - i - 1) * sizeof(client_t *));
            old_tile->player_count--;
            break;
        }
    }
}

static void calculate_new_position(client_t *client, int *new_x, int *new_y)
{
    *new_x = client->x;
    *new_y = client->y;
    switch (client->direction) {
    case UP:
        (*new_y)--;
        break;
    case RIGHT:
        (*new_x)++;
        break;
    case DOWN:
        (*new_y)++;
        break;
    case LEFT:
        (*new_x)--;
        break;
    default:
        break;
    }
}

static void add_client_to_tile(tile_t *tile, client_t *client)
{
    if (tile->player_count < MAX_CLIENTS) {
        tile->players[tile->player_count] = client;
        tile->player_count++;
    }
}

int move_forward(server_t *server, client_t *client)
{
    int new_x = 0;
    int new_y = 0;
    tile_t *new_tile = NULL;
    const char *response = "ok\n";

    if (!server || !client)
        return -1;
    calculate_new_position(client, &new_x, &new_y);
    normalize_coordinates(&server->world, &new_x, &new_y);
    remove_client_from_tile(server, client);
    client->x = new_x;
    client->y = new_y;
    new_tile = &server->world.map[client->y][client->x];
    add_client_to_tile(new_tile, client);
    sent(client->fd, response, strlen(response), 0);
    notify_player_position(server, client);
    return 0;
}

int turn_right(server_t *server, client_t *client)
{
    const char *response = "ok\n";

    if (!client)
        return -1;
    client->direction = (client->direction + 1) % 4;
    sent(client->fd, response, strlen(response), 0);
    notify_player_position(server, client);
    return 0;
}

int turn_left(server_t *server, client_t *client)
{
    const char *response = "ok\n";

    if (!client)
        return -1;
    client->direction = (client->direction + 3) % 4;
    sent(client->fd, response, strlen(response), 0);
    notify_player_position(server, client);
    return 0;
}

int check_inventory(client_t *client)
{
    char response[256] = {0};
    const char *r_names[] = {
        "food", "linemate", "deraumere",
        "sibur", "mendiane", "phiras", "thystame"
    };
    size_t offset = 0;

    if (!client)
        return -1;
    offset += snprintf(response + offset, sizeof(response) - offset, "[");
    for (int i = 0; i < 7; ++i) {
        offset += snprintf(response + offset, sizeof(response) - offset,
            "%s %d%s", r_names[i], client->inventory.resources[i],
            (i < 6) ? ", " : "]\n");
    }
    sent(client->fd, response, offset, 0);
    return 0;
}

int check_available_slots(server_t *server, client_t *client)
{
    const char *error_res = "ko\n";
    char response[256] = {0};

    if (!client || !server)
        return -1;
    if (!client->team_name && !client->is_graphic) {
        sent(client->fd, error_res, strlen(error_res), 0);
        return -1;
    }
    snprintf(response, sizeof(response), "%d\n",
        server->teams[client->team_id].available_slots);
    sent(client->fd, response, strlen(response), 0);
    return 0;
}
