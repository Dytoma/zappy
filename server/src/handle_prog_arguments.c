/*
** EPITECH PROJECT, 2025
** B-YEP-400-TLS-4-1-zappy-dytoma.batogouma
** File description:
** parse program arguments
*/

#include "parse_arguments.h"

int handle_port_option(server_t *server, char *optarg)
{
    server->port = parse_port(optarg);
    if (server->port == -1)
        return error_handler(ERR_INVALID_PORT, optarg);
    return 0;
}

int handle_width_option(server_t *server, char *optarg)
{
    server->world.width = parse_positive_int(optarg);
    if (server->world.width <= 0)
        return error_handler(ERR_INVALID_WIDTH, optarg);
    return 0;
}

int handle_height_option(server_t *server, char *optarg)
{
    server->world.height = parse_positive_int(optarg);
    if (server->world.height <= 0)
        return error_handler(ERR_INVALID_HEIGHT, optarg);
    return 0;
}

int handle_teams_option(int argc, char **argv, server_t *server)
{
    int team_count = 0;

    team_count = parse_team_names(argc, argv, server);
    if (team_count <= 0)
        return error_handler(ERR_INVALID_TEAMS, NULL);
    return 0;
}

int handle_clients_option(server_t *server, char *optarg)
{
    server->clients_per_team = parse_positive_int(optarg);
    if (server->clients_per_team <= 0)
        return error_handler(ERR_INVALID_CLIENTS, optarg);
    return 0;
}
