/*
** EPITECH PROJECT, 2025
** B-YEP-400-TLS-4-1-zappy-dytoma.batogouma
** File description:
** victory
*/

#include "../include/process_commands.h"
#include "../../include/gui_commands.h"

static void print_level_8_player(client_t *client)
{
    printf("Player %d (team: %s) is at level 8\n",
        client->id,
        client->team_name ? client->team_name : "Unknown");
}

int count_level_8_players(server_t *server)
{
    int level_8_count = 0;
    int i = 0;

    if (!server)
        return 0;
    for (i = 0; i < server->client_count; i++) {
        if (server->clients[i] &&
            server->clients[i]->is_alive &&
            server->clients[i]->level == 8) {
            level_8_count++;
            print_level_8_player(server->clients[i]);
        }
    }
    printf("Total players at level 8: %d\n", level_8_count);
    return level_8_count;
}

static void init_team_arrays(server_t *server, int *team_counts,
    const char **team_names)
{
    int i = 0;

    for (i = 0; i < server->nb_teams; i++) {
        team_counts[i] = 0;
        team_names[i] = server->teams[i].name;
    }
}

static void change_team_count(server_t *server, int *team_counts, int i)
{
    for (int j = 0; j < server->nb_teams; j++) {
        if (strcmp(server->clients[i]->team_name,
            server->teams[j].name) == 0) {
            team_counts[j]++;
            break;
        }
    }
}

static void count_teams_level_8(server_t *server, int *team_counts)
{
    for (int i = 0; i < server->client_count; i++) {
        if (server->clients[i] &&
            server->clients[i]->is_alive &&
            server->clients[i]->level == 8 &&
            server->clients[i]->team_name) {
            change_team_count(server, team_counts, i);
        }
    }
}

static int find_winning_team_index(server_t *server, int *team_counts)
{
    int winning_team_index = -1;
    int i = 0;

    for (i = 0; i < server->nb_teams; i++) {
        if (team_counts[i] >= 6) {
            return i;
        }
    }
    return winning_team_index;
}

const char *get_winning_team(server_t *server)
{
    int team_counts[server->nb_teams];
    const char *team_names[server->nb_teams];
    int winning_team_index = -1;

    init_team_arrays(server, team_counts, team_names);
    count_teams_level_8(server, team_counts);
    winning_team_index = find_winning_team_index(server, team_counts);
    if (winning_team_index != -1) {
        printf("Winning team: %s with %d players at level 8\n",
            team_names[winning_team_index], team_counts[winning_team_index]);
        return team_names[winning_team_index];
    }
    return NULL;
}

void create_end_message(char *end_message, const char *winning_team)
{
    if (winning_team) {
        snprintf(end_message, 512,
            "Game ended. Winning team: %s\n", winning_team);
    } else
        snprintf(end_message, 512, "Game ended. No winner.\n");
}

static void render_message(server_t *server, const char *end_message, int i)
{
    if (server->clients[i] && server->clients[i]->fd != -1) {
        if (sent(server->clients[i]->fd, end_message,
            strlen(end_message), 0) == -1) {
            printf("Error: Failed to send game end message to client %d\n",
                server->clients[i]->id);
            perror("send");
        }
    }
}

void send_message_to_clients(server_t *server, const char *end_message)
{
    int i = 0;

    for (i = 0; i < server->client_count; i++) {
        render_message(server, end_message, i);
    }
}
