/*
** EPITECH PROJECT, 2025
** B-YEP-400-TLS-4-1-zappy-dytoma.batogouma
** File description:
** Main file for Zappy server
** Contains argument parsing, server initialization and main loop
*/

#include "include/parse_arguments.h"

void print_usage(void)
{
    printf("USAGE: ./zappy_server -p port -x width -y height ");
    printf("-n name1 name2 ... -c clientsNb -f freq\n");
    printf("option\t\tdescription\n");
    printf("-p port\t\tport number\n");
    printf("-x width\twidth of the world\n");
    printf("-y height\theight of the world\n");
    printf("-n name1 name2 ...\tname of the team\n");
    printf("-c clientsNb\tnumber of authorized clients per team\n");
    printf("-f freq\t\treciprocal of time unit for execution of actions\n");
}

int handle_freq_option(server_t *server, char *optarg)
{
    server->freq = parse_positive_int(optarg);
    if (server->freq <= 0)
        return error_handler(ERR_INVALID_FREQ, optarg);
    return 0;
}

int parse_single_option(int opt, int argc,
    char **argv, server_t *server)
{
    if (opt == 'p')
        return handle_port_option(server, optarg);
    if (opt == 'x')
        return handle_width_option(server, optarg);
    if (opt == 'y')
        return handle_height_option(server, optarg);
    if (opt == 'n')
        return handle_teams_option(argc, argv, server);
    if (opt == 'c')
        return handle_clients_option(server, optarg);
    if (opt == 'f')
        return handle_freq_option(server, optarg);
    if (opt == 'h') {
        print_usage();
        return 0;
    }
    return error_handler(ERR_INVALID_ARG, NULL);
}

int parse_arguments(int argc, char **argv, server_t *server)
{
    int opt = getopt(argc, argv, "p:x:y:n:c:f:h");
    int result = 0;

    while (opt != -1) {
        result = parse_single_option(opt, argc, argv, server);
        if (result != 0)
            return result;
        opt = getopt(argc, argv, "p:x:y:n:c:f:h");
    }
    return validate_server_config(server);
}

static void print_server_info(server_t *server)
{
    printf("Server initialized successfully\n");
    printf("Port: %d\n", server->port);
    printf("World size: %dx%d\n", server->world.width, server->world.height);
    printf("Teams: %d\n", server->nb_teams);
    printf("Clients per team: %d\n", server->clients_per_team);
    printf("Frequency: %d\n", server->freq);
    printf("Teams:\n");
    for (int i = 0; i < server->nb_teams; i++)
        printf("    Team %d -> \t \033[32m%s\033[0m\n", server->teams[i].id,
            server->teams[i].name);
}

static int init_server(server_t *server)
{
    init_id_generator(&server->gen);
    if (init_socket(server) == -1)
        return error_handler(ERR_SOCKET_INIT, NULL);
    if (init_world(&server->world) == -1)
        return error_handler(ERR_WORLD_INIT, NULL);
    if (init_teams(server) == -1)
        return error_handler(ERR_TEAMS_INIT, NULL);
    if (init_resources(&server->world) == -1)
        return error_handler(ERR_RESOURCES_INIT, NULL);
    server->resource_time = get_current_time() + 20000 / server->freq;
    print_server_info(server);
    return 0;
}

static void cleanup_server(server_t *server)
{
    if (server != NULL) {
        cleanup_clients(server);
        cleanup_teams(server);
        cleanup_world(server);
        cleanup_socket(server);
        printf("Server cleanup completed\n");
    }
}

static void init_server_defaults(server_t *server)
{
    server->freq = 100;
    server->port = -1;
    server->world.width = -1;
    server->world.height = -1;
    server->clients_per_team = -1;
    server->nb_teams = 0;
    server->client_count = 0;
}

int main(int argc, char **argv)
{
    server_t server = {0};
    int ret = 0;

    if (check_help_argument(argc, argv))
        return 0;
    if (check_minimum_arguments(argc))
        return 84;
    init_server_defaults(&server);
    if (parse_arguments(argc, argv, &server) != 0) {
        print_usage();
        return 84;
    }
    if (init_server(&server) != 0) {
        cleanup_server(&server);
        return 84;
    }
    printf("Starting Zappy server...\n");
    ret = run_server(&server);
    cleanup_server(&server);
    return ret;
}
