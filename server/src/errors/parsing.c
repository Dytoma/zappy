/*
** EPITECH PROJECT, 2025
** B-YEP-400-TLS-4-1-zappy-dytoma.batogouma
** File description:
** argument parsing functions for the zappy server
*/

#include "server.h"

int parse_port(const char *str)
{
    char *endptr = NULL;
    long port = 0;

    if (!str)
        return -1;
    port = strtol(str, &endptr, 10);
    if (*endptr != '\0' || port <= 0 || port > 65535)
        return -1;
    return (int)port;
}

int parse_positive_int(const char *str)
{
    char *endptr = NULL;
    long value = 0;

    if (!str)
        return -1;
    value = strtol(str, &endptr, 10);
    if (*endptr != '\0' || value <= 0)
        return -1;
    return (int)value;
}

static int copy_team_name(server_t *server, char *name, int team_count)
{
    if (strlen(name) >= MAX_TEAM_NAME)
        return -1;
    memmove(server->teams[team_count].name, name, strlen(name) + 1);
    server->teams[team_count].id = team_count;
    return 0;
}

int parse_team_names(int argc, char **argv, server_t *server)
{
    int team_count = 0;
    int i = optind - 1;

    while (i < argc && argv[i][0] != '-' && team_count < MAX_TEAMS) {
        if (copy_team_name(server, argv[i], team_count) == -1)
            return -1;
        team_count++;
        i++;
    }
    server->nb_teams = team_count;
    return team_count > 0 ? team_count : -1;
}

int validate_server_config(server_t *server)
{
    if (server->port <= 0)
        return error_handler(ERR_INVALID_PORT, "Port not set");
    if (server->world.width <= 0)
        return error_handler(ERR_INVALID_WIDTH, "Width not set");
    if (server->world.height <= 0)
        return error_handler(ERR_INVALID_HEIGHT, "Height not set");
    if (server->nb_teams <= 0)
        return error_handler(ERR_INVALID_TEAMS, "No teams specified");
    if (server->clients_per_team <= 0)
        return error_handler(ERR_INVALID_CLIENTS, "Clients per team not set");
    if (server->freq <= 0)
        server->freq = 100;
    return 0;
}
