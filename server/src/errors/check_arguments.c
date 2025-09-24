/*
** EPITECH PROJECT, 2025
** server
** File description:
** check_arguments
*/

#include "parse_arguments.h"

int check_help_argument(int argc, char **argv)
{
    if (argc == 2 && strcmp(argv[1], "-help") == 0) {
        print_usage();
        return 1;
    }
    return 0;
}

int check_minimum_arguments(int argc)
{
    if (argc < 14) {
        error_handler(ERR_MISSING_ARGS, NULL);
        print_usage();
        return 1;
    }
    return 0;
}
