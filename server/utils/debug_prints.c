/*
** EPITECH PROJECT, 2025
** server
** File description:
** debug_prints
*/

#include "../include/utils.h"

void print_p_position(client_t *client)
{
    if (!client)
        return;
    printf("Client %d: Looking %d at position:\n",
        client->fd, client->direction);
    printf("\033[34mx: %d, y: %d\033[0m\n", client->x, client->y);
}
