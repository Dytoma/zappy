/*
** EPITECH PROJECT, 2025
** B-YEP-400-TLS-4-1-zappy-dytoma.batogouma
** File description:
** eject_target
*/

#include "../include/process_commands.h"
#include "../../include/gui_commands.h"

static position_t compute_new_coordinates(client_t *ejector, client_t *target)
{
    position_t pos = { target->x, target->y };

    switch (ejector->direction) {
        case UP:
            pos.y--;
            break;
        case DOWN:
            pos.y++;
            break;
        case LEFT:
            pos.x--;
            break;
        case RIGHT:
            pos.x++;
            break;
    }
    return pos;
}

static void unclock_players(server_t *server, const int x, const int y)
{
    tile_t *tile = &server->world.map[y][x];
    uint64_t current_time = get_current_time();

    for (int i = 0; i < tile->player_count; i++) {
        if (tile->players[i] && tile->players[i]->locked_in) {
            tile->players[i]->locked_in = false;
            tile->players[i]->scheduled_time = current_time;
        }
    }
}

static void cancel_incantation_if_needed(server_t *server, client_t *target)
{
    if (target->is_incantation) {
        target->is_incantation = false;
        unclock_players(server, target->x, target->y);
        target->scheduled_time = get_current_time();
        sent(target->fd, "ko\n", 3, 0);
    }
}

static ejection_info_t build_ejection_info(client_t *ejector,
    client_t *target, position_t new_pos)
{
    ejection_info_t info = {
        .ejector = ejector,
        .ejected = target,
        .from = { ejector->x, ejector->y },
        .to = new_pos
    };

    return info;
}

static void eject_and_move_player(server_t *server, tile_t *tile,
    client_t *ejector, int index)
{
    client_t *target = tile->players[index];
    position_t new_pos = compute_new_coordinates(ejector, target);
    move_info_t move = { server, target, new_pos };
    ejection_info_t info = build_ejection_info(ejector, target, new_pos);

    cancel_incantation_if_needed(server, target);
    remove_player_from_tile(tile, index);
    move_player_to_tile(move);
    notify_and_log_ejection(&info);
    notify_player_position(server, info.ejected);
}

int get_direction_vertically(int x, int y, int i, client_t *client)
{
    if (y + i == client->y && x == client->x)
        return 1;
    if (y - i == client->y && x == client->x)
        return 5;
    if (y == client->y && x + i == client->x)
        return 3;
    if (y == client->y && x - i == client->x)
        return 7;
    return 0;
}

int get_direction_horizontally(int x, int y, int j, client_t *client)
{
    if (y == client->y && x - j == client->x)
        return 1;
    if (y == client->y && x + j == client->x)
        return 5;
    if (y + j == client->y && x == client->x)
        return 3;
    if (y - j == client->y && x == client->x)
        return 7;
    return 0;
}

int get_p_direction(client_t *client, const int x, const int y)
{
    int i = client->direction == UP ? -1 : 1;
    int j = client->direction == RIGHT ? -1 : 1;

    if (client->direction == UP || client->direction == DOWN) {
        return get_direction_vertically(x, y, i, client);
    }
    return get_direction_horizontally(x, y, j, client);
}

void eject_target(eject_context_t *ctx, int index)
{
    if (!ctx || !ctx->tile || !ctx->ejector || index < 0 ||
        index >= ctx->tile->player_count)
        return;
    if (!ctx->tile->players[index])
        return;
    eject_and_move_player(ctx->server, ctx->tile, ctx->ejector, index);
    (*ctx->count)++;
}
