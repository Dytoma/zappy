/*
** EPITECH PROJECT, 2025
** Zappy [WSL: Ubuntu-24.04]
** File description:
** Parsing
*/

#include "Parsing.hpp"
#include "Error.hpp"
#include <cstring>
#include <iostream>

Argument parse(int ac, char **av)
{
    Argument args = {-1, "-1", false};

    for (int i = 1; i < ac; i++) {
        if (strcmp(av[i], "-p") == 0 && i + 1 < ac) {
            args.port = std::atoi(av[i + 1]);
            i++;
        } else if (strcmp(av[i], "-h") == 0 && i + 1 < ac) {
            args.machine = av[i + 1];
            i++;
        } else if (strcmp(av[i], "-d") == 0) {
            args.debugMode = true;
        }
    }

    if (args.port <= 0 || args.machine == "-1")
        throw Error::invalid_argument("Options -p (port) and -h (machine) must all be specified.");
    return args;
}
