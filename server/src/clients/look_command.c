/*
** EPITECH PROJECT, 2025
** B-YEP-400-TLS-4-1-zappy-dytoma.batogouma
** File description:
** look_command
*/

#include "../../include/process_commands.h"

static void remove_empty_spaces(char *str)
{
    if (str == NULL || strlen(str) == 0)
        return;
    if (str[strlen(str) - 1] == ' ')
        str[strlen(str) - 1] = '\0';
}

static char *get_tile_data(tile_t *tile, const char **r_names)
{
    char *data = malloc(sizeof(char) * 9999);
    egg_t *temp = NULL;

    if (!data)
        return "";
    data[0] = '\0';
    for (int i = 0; i < tile->player_count; i++)
        strcat(data, "player ");
    for (int i = 0; i < MAX_RESOURCES; i++)
        for (int j = 0; j < tile->resource_count[i]; j++) {
            strcat(data, r_names[i]);
            strcat(data, " ");
        }
    temp = tile->eggs;
    while (temp) {
        strcat(data, "egg ");
        temp = temp->next;
    }
    remove_empty_spaces(data);
    return data;
}

// printf("Content %d %d %d %d %d %d %d\n", tile->resource_count[0],
// tile->resource_count[1], tile->resource_count[2], tile->resource_count[3],
// tile->resource_count[4], tile->resource_count[5], tile->resource_count[6]);
static void concat_tile_data(char *response, tile_t *tile,
    const char **r_names, const char *delim)
{
    char *data = get_tile_data(tile, r_names);

    if (strlen(data) == 0 && strlen(response) > 0) {
        if (response[strlen(response) - 1] == ' ')
            response[strlen(response) - 1] = '\0';
    } else if (strlen(data) > 0 && response[strlen(response) - 1] != '[')
        strcat(response, " ");
    strcat(response, data);
    strcat(response, delim);
    free(data);
}

// printf("Tile position (%d, %d)\n", client->x, client->y);
// printf("Tile position (%d, %d)\n", x_temp, y);
static int look_vertical(server_t *server, client_t *client,
    const char **r_names)
{
    char response[10240] = {0};
    int y = 0;
    int x_temp = 0;

    strcat(response, "[");
    concat_tile_data(response, &server->world.map[client->y][client->x],
        r_names, ",");
    for (int i = 1; i <= client->level; i++)
        for (int x = client->x - i; x <= client->x + i; x++) {
            y = client->y - i;
            x_temp = x;
            normalize_coordinates(&server->world, &x_temp, &y);
            concat_tile_data(response, &server->world.map[y][x_temp],
                r_names, ",");
        }
    response[strlen(response) - 1] = '\0';
    strcat(response, "]\n");
    sent(client->fd, response, strlen(response), 0);
    return 0;
}

int look_down(server_t *server, client_t *client,
    const char **r_names)
{
    char response[10240] = {0};
    int y = 0;
    int x_temp = 0;

    strcat(response, "[");
    concat_tile_data(response, &server->world.map[client->y][client->x],
        r_names, ",");
    for (int i = 1; i <= client->level; i++)
        for (int x = client->x + i; x >= client->x - i; x--) {
            y = client->y + i;
            x_temp = x;
            normalize_coordinates(&server->world, &x_temp, &y);
            concat_tile_data(response, &server->world.map[y][x_temp],
                r_names, ",");
        }
    response[strlen(response) - 1] = '\0';
    strcat(response, "]\n");
    sent(client->fd, response, strlen(response), 0);
    return 0;
}

// printf("Tile position (%d, %d)\n", client->x, client->y);
// printf("Tile position (%d, %d)\n", x, y_temp);
static int look_horizontal(server_t *server, client_t *client,
    const char **r_names)
{
    char response[10240] = {0};
    int x = 0;
    int y_temp = 0;

    strcat(response, "[");
    concat_tile_data(response, &server->world.map[client->y][client->x],
        r_names, ",");
    for (int i = 1; i <= client->level; i++)
        for (int y = client->y - i; y <= client->y + i; y++) {
            x = client->x + i;
            y_temp = y;
            normalize_coordinates(&server->world, &x, &y_temp);
            concat_tile_data(response, &server->world.map[y_temp][x],
                r_names, ",");
        }
    response[strlen(response) - 1] = '\0';
    strcat(response, "]\n");
    sent(client->fd, response, strlen(response), 0);
    return 0;
}

int look_left(server_t *server, client_t *client,
    const char **r_names)
{
    char response[10240] = {0};
    int x = 0;
    int y_temp = 0;

    strcat(response, "[");
    concat_tile_data(response, &server->world.map[client->y][client->x],
        r_names, ",");
    for (int i = 1; i <= client->level; i++)
        for (int y = client->y + i; y >= client->y - i; y--) {
            x = client->x - i;
            y_temp = y;
            normalize_coordinates(&server->world, &x, &y_temp);
            concat_tile_data(response, &server->world.map[y_temp][x],
                r_names, ",");
        }
    response[strlen(response) - 1] = '\0';
    strcat(response, "]\n");
    sent(client->fd, response, strlen(response), 0);
    return 0;
}

// printf("Client at (%d, %d) facing %d\n", client->x, client->y,
// client->direction);
int look_around(server_t *server, client_t *client)
{
    const char *r_names[MAX_RESOURCES] = {
        "food", "linemate", "deraumere",
        "sibur", "mendiane", "phiras", "thystame"
    };

    if (!client || !server)
        return -1;
    switch (client->direction) {
        case UP:
            return look_vertical(server, client, r_names);
        case DOWN:
            return look_down(server, client, r_names);
        case LEFT:
            return look_left(server, client, r_names);
        case RIGHT:
            return look_horizontal(server, client, r_names);
        default:
            return -1;
    }
}
