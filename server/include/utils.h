/*
** EPITECH PROJECT, 2025
** B-YEP-400-TLS-4-1-zappy-dytoma.batogouma
** File description:
** Utility functions header
** Contains string manipulation and array handling functions
*/

#ifndef UTILS_H_
    #define UTILS_H_

    #include <stdio.h>
    #include <time.h>
    #include "server.h"

int count_words(const char *str, const char *delim);
void destroy_array(char **arr);
char **str_to_word_array(const char *str, const char *delim);
void display_array(char **arr);
uint64_t elapsed_time(const uint64_t end, const uint64_t start);

void print_p_position(client_t *client);
int array_length(char **arr);

// string utils
bool is_int(const char *str);

#endif /* UTILS_H_ */
