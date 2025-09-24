/*
** EPITECH PROJECT, 2025
** server
** File description:
** process_commands
*/

#ifndef PROCESS_COMMANDS_H_
    #define PROCESS_COMMANDS_H_
    #include "server.h"

typedef struct {
    int x;
    int y;
} position_t;

typedef struct {
    client_t *ejector;
    client_t *ejected;
    position_t from;
    position_t to;
} ejection_info_t;

typedef struct {
    server_t *server;
    client_t *client;
    position_t pos;
} move_info_t;

typedef struct {
    server_t *server;
    client_t *ejector;
    tile_t *tile;
    int *count;
} eject_context_t;

typedef struct {
    int level;
    int nb_players;
    int linemate;
    int deraumere;
    int sibur;
    int mendiane;
    int phiras;
    int thystame;
} elevation_req_t;

static const elevation_req_t elevation_requirements[7] = {
    {1, 1, 1, 0, 0, 0, 0, 0},
    {2, 2, 1, 1, 1, 0, 0, 0},
    {3, 2, 2, 0, 1, 0, 2, 0},
    {4, 4, 1, 1, 2, 0, 1, 0},
    {5, 4, 1, 2, 1, 3, 0, 0},
    {6, 6, 1, 2, 3, 0, 1, 0},
    {7, 6, 2, 2, 2, 2, 2, 1}
};

void add_command_to_client(command_t *new_cmd, command_t *current,
    int duration, client_t *client);
char *clean_command(const char *command);
int process_command(server_t *server, client_t *client, const char *command);
int get_command_duration(const char *command, int freq);
int handle_empty_command(char *cmd);
int try_handle_immediate_commands(server_t *server,
    client_t *client, char *cmd);
int queue_client_command(client_t *client, char *cmd, int duration);
int get_resource_type_from_name(const char *object_name);
int get_p_direction(client_t *client, const int x, const int y);

// command execution
int compute_poll_timeout(const server_t *server);
void send_ko_response(client_t *client);
bool is_incantation_allowed(server_t *server, client_t *client);
bool are_requirements_met(server_t *server, client_t *client,
    const elevation_req_t *req, int same_level_players);
void execute_incantation(server_t *server, client_t *client,
    const elevation_req_t *req, int same_level_players);
bool check_victory_condition(server_t *server);
void check_victory_after_elevation(server_t *server, client_t *client);
int count_level_8_players(server_t *server);
const char *get_winning_team(server_t *server);
void create_end_message(char *end_message, const char *winning_team);
void send_message_to_clients(server_t *server, const char *end_message);
void elevate_players(server_t *server, client_t *client);
void eject_target(eject_context_t *ctx, int index);
void remove_player_from_tile(tile_t *tile, int index);
void move_player_to_tile(move_info_t move);
void notify_and_log_ejection(const ejection_info_t *info);
bool remove_resource_from_tile(world_t *world, int x, int y,
    int resource_type);

#endif /* !PROCESS_COMMANDS_H_ */
