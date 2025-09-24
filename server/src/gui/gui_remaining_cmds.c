/*
** EPITECH PROJECT, 2025
** server
** File description:
** gui_remaining_cmds
*/

#include "../../include/gui_commands.h"
#include "../../include/utils.h"

int player_level(server_t *server, client_t *gui, command_t *cmd)
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
    snprintf(response, BUFFER_SIZE, "plv #%d %d\n",
        player->id, player->level);
    sent(gui->fd, response, strlen(response), 0);
    return 0;
}

static void send_inventory(const client_t *gui, client_t *player, const int id)
{
    char response[BUFFER_SIZE] = {0};

    if (!player)
        return;
    snprintf(response, BUFFER_SIZE, "pin #%d %d %d %d %d %d %d %d\n", id,
        player->inventory.resources[0], player->inventory.resources[1],
        player->inventory.resources[2], player->inventory.resources[3],
        player->inventory.resources[4], player->inventory.resources[5],
        player->inventory.resources[6]);
    sent(gui->fd, response, strlen(response), 0);
}

int player_inventory(server_t *server, client_t *gui, command_t *cmd)
{
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
    send_inventory(gui, player, player_nbr);
    return 0;
}

// sending the frequency so that the GUI can compute the time unit
int time_unit_request(server_t *server, client_t *gui, command_t *cmd)
{
    char response[BUFFER_SIZE] = {0};

    if (!server || !gui || !cmd)
        return 1;
    snprintf(response, BUFFER_SIZE, "sgt %d\n", server->freq);
    sent(gui->fd, response, strlen(response), 0);
    return 0;
}

int time_unit_modification(server_t *server, client_t *gui, command_t *cmd)
{
    char response[BUFFER_SIZE] = {0};
    char *value_str = NULL;
    int value = 0;

    if (!server || !gui || !cmd || !cmd->command)
        return bad_param_cmd(gui);
    value_str = strdup(cmd->command + 4);
    if (!is_int(value_str))
        return bad_param_cmd(gui);
    value = atoi(value_str);
    if (value <= 0)
        return bad_param_cmd(gui);
    server->freq = value;
    snprintf(response, BUFFER_SIZE, "sgt %d\n", server->freq);
    sent(gui->fd, response, strlen(response), 0);
    return 0;
}

void process_gui_cmd(server_t *server, client_t *client, command_t *cmd)
{
    int (*pFunc[])(server_t *, client_t *, command_t *) = {map_size,
        tile_content, map_content, name_of_teams, player_position,
        player_level, player_inventory, time_unit_request,
        time_unit_modification};
    char *commands[] = {"msz", "bct ", "mct", "tna", "ppo ", "plv ",
        "pin ", "sgt", "sst ", NULL};

    if (!client->is_graphic && client->team_name) {
        sent(client->fd, "ko\n", 3, 0);
        return;
    }
    if (!client->is_graphic && !client->team_name)
        return;
    for (int i = 0; commands[i]; i++)
        if (strncmp(cmd->command, commands[i], strlen(commands[i])) == 0) {
            pFunc[i](server, client, cmd);
            return;
        }
    unknown_cmd(client);
}
