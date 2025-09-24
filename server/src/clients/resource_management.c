/*
** EPITECH PROJECT, 2025
** B-YEP-400-TLS-4-1-zappy-dytoma.batogouma
** File description:
** resource_management
*/

#include "../include/process_commands.h"
#include "../../include/gui_commands.h"

static void remove_single_resource(client_t *client, int resource_type)
{
    if (client->inventory.resources[resource_type] > 0) {
        client->inventory.resources[resource_type]--;
        printf("Removed 1 resource of type %d ",
            resource_type);
        printf("from client %d (remaining: %d)\n",
            client->id, client->inventory.resources[resource_type]);
    } else {
        printf("Warning: Trying to remove resource type ");
        printf("%d but none available\n", resource_type);
    }
}

static void consume_resources(client_t *client,
    const elevation_req_t *req)
{
    int resources_to_consume[] = {0, req->linemate, req->deraumere,
        req->sibur, req->mendiane, req->phiras, req->thystame};

    printf("Starting resource consumption for client %d\n", client->id);
    for (int i = 1; i < MAX_RESOURCES; i++) {
        printf("Consuming %d resources of type %d\n",
            resources_to_consume[i], i);
        for (int j = 0; j < resources_to_consume[i]; j++)
            remove_single_resource(client, i);
    }
    printf("Resource consumption completed\n");
}

static void send_elevation_message(client_t *player)
{
    char success_msg[256];

    snprintf(success_msg, sizeof(success_msg), "Current level: %d\n",
        player->level);
    printf("Sending elevation message to client %d: %s\n",
        player->id, success_msg);
    if (sent(player->fd, success_msg, strlen(success_msg), 0) == -1) {
        printf("Error: Failed to send elevation message to client %d\n",
            player->id);
        perror("send");
    } else
        printf("Client %d elevated to level %d\n", player->id, player->level);
}

void elevate_players(server_t *server, client_t *client)
{
    tile_t *tile = &server->world.map[client->y][client->x];
    const int original_level = client->level;
    const elevation_req_t *req = &elevation_requirements[client->level - 1];
    int clients = 0;

    for (int i = 0; i < tile->player_count &&
        clients < req->nb_players - 1; i++) {
        if (tile->players[i] &&
            tile->players[i]->level == original_level &&
            tile->players[i]->id != client->id) {
            tile->players[i]->level++;
            tile->players[i]->locked_in = false;
            tile->players[i]->scheduled_time = get_current_time();
            send_elevation_message(tile->players[i]);
            clients++;
        }
    }
    client->level = original_level + 1;
    send_elevation_message(client);
    check_victory_after_elevation(server, client);
}

static void print_success_message(client_t *client, int same_level_players)
{
    printf("Incantation successful at (%d, %d) ",
        client->x, client->y);
    printf("- %d players elevated from level %d to %d\n",
        same_level_players, client->level - 1, client->level);
}

static void send_elevation_start(client_t *client)
{
    char response[256];

    snprintf(response, sizeof(response),
        "Elevation underway\n");
    printf("Sending elevation start to client %d: %s", client->id, response);
    if (sent(client->fd, response, strlen(response), 0) == -1) {
        printf("Error: Failed to send elevation start to client %d\n",
            client->id);
        perror("send");
    }
}

void execute_incantation(server_t *server, client_t *client,
    const elevation_req_t *req, int same_level_players)
{
    (void)server;
    send_elevation_start(client);
    consume_resources(client, req);
    print_success_message(client, same_level_players);
}
