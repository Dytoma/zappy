/*
** EPITECH PROJECT, 2025
** server
** File description:
** time_utils
*/

#include "../include/server.h"
#include "../include/utils.h"

uint64_t elapsed_time(const uint64_t end, const uint64_t start)
{
    if (end < start)
        return 0;
    return end - start;
}
