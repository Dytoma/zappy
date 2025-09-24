/*
** EPITECH PROJECT, 2025
** server
** File description:
** gui_commands
*/

#include "../../include/gui_commands.h"

#include "../../include/utils.h"

int map_size(server_t *server, client_t *gui, command_t *cmd)
{
    char response[BUFFER_SIZE] = {0};

    if (!server || !gui || !cmd)
        return 1;
    if (strlen(cmd->command) != 3)
        return bad_param_cmd(gui);
    snprintf(response, BUFFER_SIZE, "msz %d %d\n",
        server->world.width, server->world.height);
    sent(gui->fd, response, strlen(response), 0);
    return 0;
}

static void get_tile_content(tile_t *tile, char *content,
    const int x, const int y)
{
    if (!tile)
        return;
    memset(content, 0, BUFFER_SIZE);
    snprintf(content, BUFFER_SIZE, "bct %d %d %d %d %d %d %d %d %d\n", x, y,
        tile->resource_count[0], tile->resource_count[1],
        tile->resource_count[2], tile->resource_count[3],
        tile->resource_count[4], tile->resource_count[5],
        tile->resource_count[6]);
}

int tile_content(server_t *server, client_t *gui, command_t *cmd)
{
    char **parsed_cmd = NULL;
    int x = 0;
    int y = 0;
    char response[BUFFER_SIZE] = {0};

    if (!server || !gui || !cmd || !cmd->command)
        return bad_param_cmd(gui);
    parsed_cmd = str_to_word_array(cmd->command, " \n");
    if (array_length(parsed_cmd) != 3 || !is_int(parsed_cmd[1]) ||
        !is_int(parsed_cmd[2]))
        return 1;
    x = atoi(parsed_cmd[1]);
    y = atoi(parsed_cmd[2]);
    destroy_array(parsed_cmd);
    normalize_coordinates(&server->world, &x, &y);
    get_tile_content(&server->world.map[y][x], response, x, y);
    sent(gui->fd, response, strlen(response), 0);
    return 0;
}

int map_content(server_t *server, client_t *gui, command_t *cmd)
{
    char response[BUFFER_SIZE] = {0};

    (void)cmd;
    if (!server || !gui)
        return 1;
    for (int y = 0; y < server->world.height; y++)
        for (int x = 0; x < server->world.width; x++) {
            get_tile_content(&server->world.map[y][x], response, x, y);
            sent(gui->fd, response, strlen(response), 0);
        }
    return 0;
}

int name_of_teams(server_t *server, client_t *gui, command_t *cmd)
{
    char response[BUFFER_SIZE] = {0};

    if (!server || !gui || !cmd)
        return 1;
    for (int i = 0; i < server->nb_teams; i++) {
        memset(response, 0, BUFFER_SIZE);
        snprintf(response, BUFFER_SIZE, "tna %s\n", server->teams[i].name);
        sent(gui->fd, response, strlen(response), 0);
    }
    return 0;
}

int player_position(server_t *server, client_t *gui, command_t *cmd)
{
    char response[BUFFER_SIZE] = {0};
    char *player_nbr_str = NULL;
    int player_nbr = 0;
    client_t *player = NULL;

    if (!server || !gui || !cmd || !cmd->command)
        return bad_param_cmd(gui);
    player_nbr_str = strdup(cmd->command + 5);
    if (!is_int(player_nbr_str))
        return bad_param_cmd(gui);
    player_nbr = atoi(player_nbr_str);
    for (int i = 0; server->clients[i] != NULL; i++)
        if (server->clients[i]->id == player_nbr)
            player = server->clients[i];
    if (!player)
        return bad_param_cmd(gui);
    snprintf(response, BUFFER_SIZE, "ppo #%d %d %d %d\n",
        player->id, player->x, player->y, player->direction + 1);
    sent(gui->fd, response, strlen(response), 0);
    return 0;
}
