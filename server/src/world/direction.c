/*
** EPITECH PROJECT, 2025
** B-YEP-400-TLS-4-1-zappy-dytoma.batogouma
** File description:
** direction
*/

#include "gui_commands.h"
#include "server.h"
#include "utils.h"

int calculate_direction(direction_t direction, world_t *world)
{
    int dx = 0;
    int dy = 0;

    if (!world)
        return -1;
    dx = direction.to_x - direction.from_x;
    dy = direction.to_y - direction.from_y;
    if (abs(dx) > world->width / 2)
        dx = dx > 0 ? dx - world->width : dx + world->width;
    if (abs(dy) > world->height / 2)
        dy = dy > 0 ? dy - world->height : dy + world->height;
    if (abs(dx) > abs(dy))
        return dx > 0 ? 1 : 3;
    else
        return dy > 0 ? 2 : 0;
}

int count_resource_around(const world_t *world,
    const resource_type_t resource)
{
    int count = 0;

    for (int y = 0; y < world->height; y++) {
        for (int x = 0; x < world->width; x++) {
            count += world->map[y][x].resource_count[resource];
        }
    }
    return count;
}
