/*
** EPITECH PROJECT, 2025
** B-YEP-400-TLS-4-1-zappy-dytoma.batogouma
** File description:
** check_victory
*/

#include "../include/process_commands.h"
#include "../../include/gui_commands.h"

void notify_game_end_gui(server_t *server, const char *winning_team)
{
    char end_message[512];

    create_end_message(end_message, winning_team);
    printf("Broadcasting game end message: %s", end_message);
    send_message_to_clients(server, end_message);
    notify_game_end(server, winning_team);
}

bool check_victory_condition(server_t *server)
{
    int level_8_players = 0;
    const char *winning_team = NULL;

    if (!server) {
        printf("Error: Invalid server structure\n");
        return false;
    }
    level_8_players = count_level_8_players(server);
    if (level_8_players >= 6) {
        printf("Checking winning condition\n");
        winning_team = get_winning_team(server);
        if (winning_team == NULL)
            return false;
        notify_game_end_gui(server, winning_team);
        return true;
    }
    printf("Victory condition not yet met: %d/6 players at level 8\n",
        level_8_players);
    return false;
}

void check_victory_after_elevation(server_t *server, client_t *client)
{
    if (!server || !client)
        return;
    if (client->level == 8) {
        printf("Client %d reached level 8, checking victory condition\n",
            client->id);
        if (check_victory_condition(server)) {
            printf("Game ending due to victory condition\n");
            server->running = false;
        }
    }
}
