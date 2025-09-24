/*
** EPITECH PROJECT, 2025
** server
** File description:
** string_utils
*/

#include "../include/utils.h"

bool is_int(const char *str)
{
    int i = 0;

    if (str[0] == '-')
        i = 1;
    while (str[i] != '\0') {
        if (str[i] < '0' || str[i] > '9')
            return false;
        i++;
    }
    return true;
}

int sent(int fd, const char *msg, const size_t size, const int flag)
{
    int sent = send(fd, msg, size, flag);

    while (sent != -1) {
        if ((size_t)sent < strlen(msg))
            sent += send(fd, msg + sent, strlen(msg) - sent, flag);
        if ((size_t)sent >= strlen(msg)) {
            return sent;
        }
    }
    return -1;
}

void init_id_generator(id_generator_t *gen)
{
    int j;
    int tmp;

    if (!gen)
        return;
    for (int i = 0; i < MAX_IDS; ++i)
        gen->ids[i] = i;
    for (int i = MAX_IDS - 1; i > 0; --i) {
        j = rand() % (i + 1);
        tmp = gen->ids[i];
        gen->ids[i] = gen->ids[j];
        gen->ids[j] = tmp;
    }
    gen->index = 0;
}

int get_unique_id(id_generator_t *gen)
{
    int id;

    if (!gen || gen->index >= MAX_IDS)
        return -1;
    id = gen->ids[gen->index];
    gen->index += 1;
    return id;
}
