/*
** EPITECH PROJECT, 2025
** B-YEP-400-TLS-4-1-zappy-dytoma.batogouma
** File description:
** Main
*/

#include <iostream>
#include "Error.hpp"
#include "Parsing.hpp"
#include "Window.hpp"
#include "Client.hpp"



int main(int ac, char **av)
{
    try {
        check_arguments(ac, av);
        Argument args = parse(ac, av);
        Client client(args.machine, args.port, args.debugMode);
        client.start();
    } catch (const Error::invalid_argument &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 84;
    }
    return 0;
}
