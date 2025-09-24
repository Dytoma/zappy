/*
** EPITECH PROJECT, 2025
** server
** File description:
** parse_arguments
*/

#ifndef PARSE_ARGUMENTS_H_
    #define PARSE_ARGUMENTS_H_
    #include "server.h"

int handle_port_option(server_t *server, char *optarg);
int handle_width_option(server_t *server, char *optarg);
int handle_height_option(server_t *server, char *optarg);
int handle_teams_option(int argc, char **argv, server_t *server);
int handle_clients_option(server_t *server, char *optarg);
int check_help_argument(int argc, char **argv);
int check_minimum_arguments(int argc);
void print_usage(void);

#endif /* !PARSE_ARGUMENTS_H_ */
