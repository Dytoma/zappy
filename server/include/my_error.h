/*
** EPITECH PROJECT, 2025
** B-YEP-400-TLS-4-1-zappy-dytoma.batogouma
** File description:
** Error handling definitions and functions
** Contains error codes, structures and macros for error management
*/

#ifndef ERROR_H_
    #define ERROR_H_

    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <errno.h>
    #include <time.h>

typedef enum {
    ERR_SOCKET_CONF = -11,
    ERR_SUCCESS = 0,
    ERR_INVALID_PORT = 1,
    ERR_INVALID_WIDTH = 2,
    ERR_INVALID_HEIGHT = 3,
    ERR_INVALID_TEAMS = 4,
    ERR_INVALID_CLIENTS = 5,
    ERR_INVALID_FREQ = 6,
    ERR_INVALID_ARG = 7,
    ERR_MISSING_ARGS = 8,
    ERR_SOCKET_INIT = 9,
    ERR_SOCKET_BIND = 10,
    ERR_SOCKET_LISTEN = 11,
    ERR_SOCKET_ACCEPT = 12,
    ERR_POLL_ERROR = 13,
    ERR_WORLD_INIT = 14,
    ERR_TEAMS_INIT = 15,
    ERR_RESOURCES_INIT = 16,
    ERR_MEMORY_ALLOC = 17,
    ERR_CLIENT_OVERFLOW = 18,
    ERR_INVALID_COMMAND = 19,
    ERR_TEAM_NOT_FOUND = 20,
    ERR_INVALID_COORDINATES = 21,
    ERR_RESOURCE_NOT_FOUND = 22,
    ERR_INCANTATION_FAILED = 23,
    ERR_BUFFER_OVERFLOW = 24,
    ERR_CLIENT_ALLOCATION = 25,
} error_code_t;

typedef struct error_s {
    error_code_t code;
    const char *message;
    const char *context;
} error_t;

extern const char *error_messages[];
int error_handler(error_code_t code, const char *context);
void print_error(error_code_t code, const char *context);
void log_error(error_code_t code, const char *function);
int return_error_func(error_code_t code, const char *context);
int check_null_func(void *ptr, const char *context);
#endif /* ERROR_H_ */
