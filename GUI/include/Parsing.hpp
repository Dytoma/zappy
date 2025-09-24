/*
** EPITECH PROJECT, 2025
** Zappy [WSL: Ubuntu-24.04]
** File description:
** Parsing
*/

#ifndef PARSING_HPP_
    #define PARSING_HPP_
    #include <string>

struct Argument {
    int port;
    std::string machine;
    bool debugMode;
};

Argument parse(int, char **);

#endif /* !PARSING_HPP_ */
