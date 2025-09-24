/*
** EPITECH PROJECT, 2025
** B-YEP-400-TLS-4-1-zappy-dytoma.batogouma
** File description:
** str_to_word_array
*/

#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

int count_words(const char *str, const char *delim)
{
    char *dup = strdup(str);
    char *token = NULL;
    int nb = 0;

    if (dup == NULL || str == NULL || delim == NULL)
        return 0;
    token = strtok(dup, delim);
    while (token != NULL) {
        nb++;
        token = strtok(NULL, delim);
    }
    free(dup);
    return nb;
}

char **str_to_word_array(const char *str, const char *delim)
{
    char *dup = strdup(str);
    char *token = NULL;
    int nb_args = 0;
    char **result = NULL;
    size_t i = 0;

    if (dup == NULL || !str || !delim)
        return NULL;
    nb_args = count_words(str, delim);
    result = malloc(sizeof(char *) * (nb_args + 1));
    token = strtok(dup, delim);
    while (token != NULL) {
        result[i] = strdup(token);
        token = strtok(NULL, delim);
        i++;
    }
    result[i] = NULL;
    free(dup);
    return result;
}

void destroy_array(char **arr)
{
    if (arr == NULL)
        return;
    for (size_t i = 0; arr[i] != NULL; i++)
        free(arr[i]);
    free(arr);
}

int array_length(const char **arr)
{
    int i = 0;

    if (arr == NULL)
        return i;
    for (i = 0; arr[i] != NULL; i++) {
    }
    return i;
}

void display_array(char **arr)
{
    if (arr == NULL)
        return;
    for (size_t i = 0; arr[i] != NULL; i++)
        printf("%s\n", arr[i]);
}
