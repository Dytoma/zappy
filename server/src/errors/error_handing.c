/*
** EPITECH PROJECT, 2025
** B-YEP-400-TLS-4-1-zappy-dytoma.batogouma
** File description:
** error handling functions for the zappy server
*/

#include "my_error.h"
#include "server.h"

const char *error_messages[] = {
    [ERR_SUCCESS] = "Success",
    [ERR_INVALID_PORT] = "Invalid port number",
    [ERR_INVALID_WIDTH] = "Invalid world width",
    [ERR_INVALID_HEIGHT] = "Invalid world height",
    [ERR_INVALID_TEAMS] = "Invalid team configuration",
    [ERR_INVALID_CLIENTS] = "Invalid number of clients per team",
    [ERR_INVALID_FREQ] = "Invalid frequency value",
    [ERR_INVALID_ARG] = "Invalid argument",
    [ERR_MISSING_ARGS] = "Missing required arguments",
    [ERR_SOCKET_INIT] = "Socket initialization failed",
    [ERR_SOCKET_BIND] = "Socket bind failed",
    [ERR_SOCKET_LISTEN] = "Socket listen failed",
    [ERR_SOCKET_ACCEPT] = "Socket accept failed",
    [ERR_POLL_ERROR] = "Poll system call failed",
    [ERR_WORLD_INIT] = "World initialization failed",
    [ERR_TEAMS_INIT] = "Team initialization failed",
    [ERR_RESOURCES_INIT] = "Resource initialization failed",
    [ERR_MEMORY_ALLOC] = "Memory allocation failed",
    [ERR_CLIENT_OVERFLOW] = "Maximum number of clients reached",
    [ERR_INVALID_COMMAND] = "Invalid command received",
    [ERR_TEAM_NOT_FOUND] = "Team not found",
    [ERR_INVALID_COORDINATES] = "Invalid coordinates",
    [ERR_RESOURCE_NOT_FOUND] = "Resource not found",
    [ERR_INCANTATION_FAILED] = "Incantation failed",
    [ERR_BUFFER_OVERFLOW] = "Buffer overflow detected"
};

int error_handler(error_code_t code, const char *context)
{
    print_error(code, context);
    if (code >= ERR_SOCKET_INIT && code <= ERR_POLL_ERROR)
        log_error(code, "critical");
    return code == ERR_SUCCESS ? 0 : -1;
}

static const char *get_error_message(error_code_t code)
{
    if ((size_t)code < sizeof(error_messages) / sizeof(error_messages[0]) &&
        error_messages[code] != NULL)
        return error_messages[code];
    return "Unknown error";
}

void print_error(error_code_t code, const char *context)
{
    const char *message = get_error_message(code);

    if (context != NULL)
        fprintf(stderr, "Error: %s (%s)\n\n", message, context);
    else
        fprintf(stderr, "Error: %s\n\n", message);
    if (errno != 0 && (code >= ERR_SOCKET_INIT && code <= ERR_POLL_ERROR)) {
        perror("System error");
        errno = 0;
    }
}

static void write_log_entry(FILE *log_file, error_code_t code,
    const char *function, char *time_str)
{
    const char *message = get_error_message(code);

    fprintf(log_file, "[%s] ERROR %d in %s - %s",
        time_str ? time_str : "Unknown time",
        code, function, message);
    if (errno != 0)
        fprintf(log_file, " - System: %s", strerror(errno));
    fprintf(log_file, "\n");
}

void log_error(error_code_t code, const char *function)
{
    FILE *log_file = fopen("zappy_server.log", "a");
    time_t now = time(NULL);
    char *time_str = ctime(&now);

    if (time_str)
        time_str[strlen(time_str) - 1] = '\0';
    if (log_file) {
        write_log_entry(log_file, code, function, time_str);
        fclose(log_file);
    }
}

int return_error_func(error_code_t code, const char *context)
{
    error_handler(code, context);
    return -1;
}

int check_null_func(void *ptr, const char *context)
{
    if (ptr == NULL) {
        error_handler(ERR_MEMORY_ALLOC, context);
        exit(84);
    }
    return 0;
}
