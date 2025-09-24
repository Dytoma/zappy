/*
** EPITECH PROJECT, 2025
** B-YEP-400-TLS-4-1-zappy-dytoma.batogouma
** File description:
** fork_command
*/

#include "../include/process_commands.h"
#include "../../include/gui_commands.h"

static egg_t *create_new_egg(client_t *client)
{
    egg_t *new_egg = NULL;
    static int egg_id_counter = 0;

    new_egg = malloc(sizeof(egg_t));
    if (!new_egg)
        return NULL;
    egg_id_counter++;
    new_egg->id = egg_id_counter;
    new_egg->team_id = client->team_id;
    new_egg->x = client->x;
    new_egg->y = client->y;
    new_egg->laid_time = get_current_time();
    new_egg->next = NULL;
    return new_egg;
}

static int add_egg_to_tile(server_t *server, client_t *client, egg_t *new_egg)
{
    tile_t *current_tile = NULL;

    current_tile = &server->world.map[client->y][client->x];
    new_egg->next = current_tile->eggs;
    current_tile->eggs = new_egg;
    return 0;
}

static int add_egg_to_team(team_t *team, egg_t *new_egg)
{
    egg_t *temp = team->eggs;
    egg_t *team_egg = NULL;

    if (!temp) {
        team->eggs = malloc(sizeof(egg_t));
        if (!team->eggs)
            return -1;
        memcpy(team->eggs, new_egg, sizeof(egg_t));
        team->eggs->next = NULL;
    } else {
        while (temp->next)
            temp = temp->next;
        team_egg = malloc(sizeof(egg_t));
        if (!team_egg)
            return -1;
        memcpy(team_egg, new_egg, sizeof(egg_t));
        team_egg->next = NULL;
        temp->next = team_egg;
    }
    return 0;
}

static bool is_fork_valid(server_t *server, client_t *client)
{
    return server && client && client->is_alive;
}

static int send_ko_and_return(client_t *client)
{
    if (client)
        sent(client->fd, "ko\n", 3, 0);
    return -1;
}

static egg_t *create_egg_or_fail(client_t *client)
{
    egg_t *egg = create_new_egg(client);

    if (!egg)
        sent(client->fd, "ko\n", 3, 0);
    return egg;
}

static int register_egg(server_t *server, client_t *client, egg_t *egg)
{
    team_t *team = &server->teams[client->team_id];

    add_egg_to_tile(server, client, egg);
    if (add_egg_to_team(team, egg) == -1)
        return send_ko_and_return(client);
    team->available_slots++;
    return 0;
}

static void send_message_to_ui(server_t *server, client_t *client, egg_t *egg)
{
    char msg[BUFFER_SIZE];

    if (!server || !client)
        return;
    snprintf(msg, BUFFER_SIZE, "enw #%d #%d %d %d\n",
        egg->id, client->id, egg->x, egg->y);
    for (int i = 0; server->clients[i] != NULL; i++)
        if (server->clients[i]->is_graphic) {
            sent(server->clients[i]->fd, msg, strlen(msg), 0);
        }
}

int fork_player(server_t *server, client_t *client)
{
    egg_t *new_egg = NULL;

    if (!is_fork_valid(server, client))
        return send_ko_and_return(client);
    new_egg = create_egg_or_fail(client);
    if (!new_egg)
        return -1;
    if (register_egg(server, client, new_egg) == -1)
        return -1;
    notify_egg_laying(server, client->id);
    sent(client->fd, "ok\n", 3, 0);
    send_message_to_ui(server, client, new_egg);
    printf("Client %d forked an egg at (%d, %d)\n", client->id,
        client->x, client->y);
    return 0;
}
