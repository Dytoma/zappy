/*
** EPITECH PROJECT, 2025
** B-YEP-400-TLS-4-1-zappy-dytoma.batogouma
** File description:
** Commands implementation
*/

#include "server.h"
#include "client_movements.h"

int find_team_by_name(server_t *server, const char *name)
{
    int i = 0;

    if (!server || !name)
        return -1;
    for (i = 0; i < server->nb_teams; i++) {
        if (strcmp(server->teams[i].name, name) == 0)
            return i;
    }
    return -1;
}

int count_eggs(egg_t *eggs)
{
    int i = 0;
    egg_t *egg = eggs;

    if (!egg)
        return i;
    while (egg != NULL) {
        i++;
        egg = egg->next;
    }
    return i;
}

static void notify_client_connection(server_t *server, egg_t *egg)
{
    char msg[BUFFER_SIZE];

    if (!egg)
        return;
    snprintf(msg, BUFFER_SIZE, "ebo #%d\n", egg->id);
    for (int i = 0; server->clients[i] != NULL; i++)
        if (server->clients[i]->is_graphic) {
            sent(server->clients[i]->fd, msg, strlen(msg), 0);
        }
}

static void connect_client_with_egg(server_t *server, client_t *client)
{
    team_t *team = &server->teams[client->team_id];
    egg_t *egg = NULL;
    tile_t *tile = NULL;

    if (!team)
        return;
    if (team->available_slots != 0 && team->available_slots <=
        count_eggs(team->eggs)) {
        egg = team->eggs;
        tile = &server->world.map[egg->y][egg->x];
        client->x = egg->x;
        client->y = egg->y;
        team->eggs = egg->next;
        remove_egg(egg, tile);
        notify_client_connection(server, egg);
        free(egg);
    }
}

static void send_message_to_gui(server_t *server, client_t *client)
{
    char msg[BUFFER_SIZE] = {0};

    snprintf(msg, BUFFER_SIZE, "pnw #%d %d %d %d %d %s\n",
        client->id, client->x, client->y, client->direction + 1, client->level,
        client->team_name);
    for (int i = 0; server->clients[i] != NULL; i++)
        if (server->clients[i]->is_graphic) {
            sent(server->clients[i]->fd, msg, strlen(msg), 0);
        }
}

static int setup_client_in_team(server_t *server, client_t *client,
    const char *team_name, int team_id)
{
    team_t *team = NULL;
    tile_t *tile = NULL;

    team = &server->teams[team_id];
    client->team_name = strdup(team_name);
    if (!client->team_name)
        return -1;
    client->team_id = team_id;
    team->connected_clients++;
    connect_client_with_egg(server, client);
    team->available_slots = team->max_clients - team->connected_clients;
    tile = &server->world.map[client->y][client->x];
    if (tile->player_count < MAX_CLIENTS) {
        tile->players[tile->player_count] = client;
        tile->player_count++;
    }
    send_message_to_gui(server, client);
    return 0;
}

int add_client_to_team(server_t *server, client_t *client,
    const char *team_name)
{
    int team_id = 0;
    team_t *team = NULL;

    if (!server || !client || !team_name)
        return -1;
    team_id = find_team_by_name(server, team_name);
    if (team_id == -1)
        return -1;
    team = &server->teams[team_id];
    if (team->connected_clients >= team->max_clients)
        return -1;
    return setup_client_in_team(server, client, team_name, team_id);
}

int move_commands(server_t *server, client_t *client, command_t *cmd)
{
    if (client->is_graphic || (!client->is_graphic && !client->team_name))
        return 1;
    if (strcmp(cmd->command, "Forward") == 0)
        return move_forward(server, client);
    if (strcmp(cmd->command, "Right") == 0)
        return turn_right(server, client);
    if (strcmp(cmd->command, "Left") == 0)
        return turn_left(server, client);
    return 1;
}

static int part_commands(server_t *server, client_t *client, command_t *cmd)
{
    if (strcmp(cmd->command, "Inventory") == 0)
        return check_inventory(client);
    if (strcmp(cmd->command, "Connect_nbr") == 0)
        return check_available_slots(server, client);
    if (strcmp(cmd->command, "Look") == 0)
        return look_around(server, client);
    if (strncmp(cmd->command, "Take ", 5) == 0)
        return execute_take_command(server, client, cmd);
    if (strncmp(cmd->command, "Set ", 4) == 0)
        return execute_set_command(server, client, cmd);
    return 1;
}

int other_commands(server_t *server, client_t *client, command_t *cmd)
{
    if (client->is_graphic || (!client->is_graphic && !client->team_name))
        return 1;
    if (part_commands(server, client, cmd) == 0)
        return 0;
    if (strcmp(cmd->command, "Fork") == 0)
        return fork_player(server, client);
    if (strcmp(cmd->command, "Eject") == 0)
        return eject_players(server, client);
    if (strncmp(cmd->command, "Broadcast ", 10) == 0)
        return broadcast_command(server, client, cmd);
    return 1;
}
