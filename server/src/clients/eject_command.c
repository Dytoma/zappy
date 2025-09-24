/*
** EPITECH PROJECT, 2025
** B-YEP-400-TLS-4-1-zappy-dytoma.batogouma
** File description:
** eject_command
*/

#include "../include/process_commands.h"
#include "../../include/gui_commands.h"

static bool is_eject_valid(server_t *server, client_t *client)
{
    tile_t *tile = NULL;

    if (!server || !client || !client->is_alive)
        return false;
    tile = &server->world.map[client->y][client->x];
    return tile->player_count > 1;
}

void remove_player_from_tile(tile_t *tile, int index)
{
    int j = 0;

    if (!tile || index < 0 || index >= tile->player_count)
        return;
    for (j = index; j < tile->player_count - 1; j++)
        tile->players[j] = tile->players[j + 1];
    tile->player_count--;
}

static void move_player(tile_t *tile, move_info_t move)
{
    tile->players[tile->player_count] = move.client;
    tile->player_count++;
    move.client->x = move.pos.x;
    move.client->y = move.pos.y;
}

void move_player_to_tile(move_info_t move)
{
    tile_t *tile = NULL;

    normalize_coordinates(&move.server->world, &move.pos.x, &move.pos.y);
    if (move.pos.x < 0 || move.pos.x >= move.server->world.width ||
        move.pos.y < 0 || move.pos.y >= move.server->world.height) {
        printf("Error: Invalid coordinates after normalization (%d, %d)\n",
            move.pos.x, move.pos.y);
        return;
    }
    tile = &move.server->world.map[move.pos.y][move.pos.x];
    if (tile->player_count >= MAX_CLIENTS) {
        printf("Error: player_count overflow in move_player_to_tile\n");
        return;
    }
    move_player(tile, move);
}

void notify_and_log_ejection(const ejection_info_t *info)
{
    char message[1024] = {0};

    if (!info || !info->ejected || !info->ejector)
        return;
    snprintf(message, 1024, "eject %d\n",
        get_p_direction(info->ejected, info->ejector->x, info->ejector->y));
    sent(info->ejected->fd, message, strlen(message), 0);
    printf("Client %d ejected client %d from (%d, %d) to (%d, %d)\n",
        info->ejector->id, info->ejected->id,
        info->from.x, info->from.y, info->to.x, info->to.y);
}

static void send_eject_result(client_t *client, int count)
{
    if (!client)
        return;
    if (count > 0) {
        sent(client->fd, "ok\n", 3, 0);
        printf("Client %d successfully ejected %d players\n",
            client->id, count);
    } else
        sent(client->fd, "ko\n", 3, 0);
}

static bool handle_invalid_eject(server_t *server, client_t *client)
{
    if (!is_eject_valid(server, client)) {
        if (client)
            sent(client->fd, "ko\n", 3, 0);
        return true;
    }
    return false;
}

void eject_all_other_players(eject_context_t *ctx)
{
    int i = 0;
    client_t *target = NULL;

    if (!ctx || !ctx->tile)
        return;
    for (i = ctx->tile->player_count - 1; i >= 0; i--) {
        target = ctx->tile->players[i];
        if (!target || target->id == ctx->ejector->id)
            continue;
        eject_target(ctx, i);
    }
}

static void eject_eggs(server_t *server, tile_t *tile)
{
    egg_t *egg = tile->eggs;
    egg_t *temp = NULL;

    while (egg != NULL) {
        temp = egg->next;
        printf("Egg with id %d ejected\n", egg->id);
        notify_egg_death(server, egg->id);
        for (int i = 0; i < server->nb_teams; i++)
            remove_egg_from_team(&server->teams[i], egg->id);
        free(egg);
        egg = temp;
    }
    tile->eggs = NULL;
}

int eject_players(server_t *server, client_t *client)
{
    int count = 0;
    eject_context_t ctx = { server, client, NULL, &count };

    if (handle_invalid_eject(server, client))
        return -1;
    ctx.tile = &server->world.map[client->y][client->x];
    eject_all_other_players(&ctx);
    eject_eggs(server, ctx.tile);
    send_eject_result(client, count);
    return (count > 0 ? 0 : -1);
}
