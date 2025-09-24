/*
** EPITECH PROJECT, 2025
** server
** File description:
** wrong_commands
*/

#include "../../include/gui_commands.h"

int unknown_cmd(client_t *gui)
{
    const char *msg = "suc\n";

    if (!gui)
        return 1;
    sent(gui->fd, msg, strlen(msg), 0);
    return 1;
}

int bad_param_cmd(client_t *gui)
{
    const char *msg = "sbp\n";

    if (!gui)
        return 1;
    sent(gui->fd, msg, strlen(msg), 0);
    return 1;
}
