/*
** EPITECH PROJECT, 2025
** server
** File description:
** client_movements
*/

#ifndef CLIENT_MOVEMENTS_H_
    #define CLIENT_MOVEMENTS_H_
    #include "server.h"
    #include "utils.h"

int move_forward(server_t *server, client_t *client);
int turn_right(server_t *server, client_t *client);
int turn_left(server_t *server, client_t *client);

#endif /* !CLIENT_MOVEMENTS_H_ */
