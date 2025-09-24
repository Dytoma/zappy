/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Error
*/

#include "Error.hpp"
#include <cstring>
#include <iostream>

Error::invalid_argument::invalid_argument(const std::string &msg)
    : message(msg) {}

const char *Error::invalid_argument::what() const noexcept
{
    return message.c_str();
}

Error::runtime_error::runtime_error(const std::string &msg)
   : message(msg) {}

const char *Error::runtime_error::what() const noexcept
{
    return message.c_str();
}

void check_arguments(const int ac, char **av)
{
    if (ac == 2 && strcmp(av[1], "-help") == 0) {
        std::cout << "USAGE: ./zappy_gui -p port -h machine [-d]" << std::endl;
        std::cout << "    -p port\t" << "port number" << std::endl;
        std::cout << "    -h machine\t" << "hostname of the server" << std::endl;
        std::cout << "    -d\t\t" << "debug mode" << std::endl;
        exit(0);
    }
    if (ac < 5)
        throw Error::invalid_argument("too few arguments. See -help for more informations.");
    if (ac > 5)
        throw Error::invalid_argument("too many arguments. See -help for more informations.");
}
