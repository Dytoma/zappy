/*
** EPITECH PROJECT, 2025
** B-YEP-400-TLS-4-1-zappy-dytoma.batogouma
** File description:
** Server main header file
** Contains all structures, enums and function prototypes for the server
*/

#ifndef SERVER_H_
    #define SERVER_H_

    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <unistd.h>
    #include <getopt.h>
    #include <sys/socket.h>
    #include <sys/types.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <poll.h>
    #include <fcntl.h>
    #include <errno.h>
    #include <time.h>
    #include <signal.h>
    #include <stdbool.h>
    #include "my_error.h"
    #include <sys/time.h>

    #define MAX_TEAMS 20
    #define MAX_TEAM_NAME 256
    #define MAX_CLIENTS 200
    #define MAX_COMMAND_QUEUE 10
    #define BUFFER_SIZE 2048
    #define MAX_RESOURCES 7
    #define INITIAL_LIFE_UNITS 10
    #define LIFE_UNIT_TIME 126
    #define MILLISECONDS 1000
    #define MAX_IDS 10000

typedef enum {
    FOOD = 0,
    LINEMATE,
    DERAUMERE,
    SIBUR,
    MENDIANE,
    PHIRAS,
    THYSTAME
} resource_type_t;

typedef enum {
    UP,
    RIGHT,
    DOWN,
    LEFT
} move_direction_t;

typedef enum {
    FALSE,
    TRUE
} bool_t;

typedef struct resource_s {
    resource_type_t type;
    int x;
    int y;
    struct resource_s *next;
} resource_t;

typedef struct id_generator_s {
    int ids[MAX_IDS];
    int index;
} id_generator_t;

typedef struct tile_s {
    resource_t *resources[MAX_RESOURCES];
    int resource_count[MAX_RESOURCES];
    struct client_s **players;
    int player_count;
    struct egg_s *eggs;
} tile_t;

typedef struct world_s {
    int width;
    int height;
    tile_t **map;
    uint64_t last_resource_spawn;
    bool filled;
} world_t;

typedef struct inventory_s {
    int resources[MAX_RESOURCES];
} inventory_t;

typedef struct command_s {
    char *command;
    uint64_t start_time;
    uint64_t scheduled_time;
    int duration;
    struct command_s *next;
} command_t;

typedef struct client_s {
    int fd;
    int id;
    char *team_name;
    int team_id;
    int x;
    int y;
    move_direction_t direction;
    int level;
    int64_t life_units;
    uint64_t last_meal;
    uint64_t scheduled_time;
    inventory_t inventory;
    command_t *command_queue;
    int queue_size;
    char buffer[BUFFER_SIZE];
    int buffer_pos;
    bool is_graphic;
    bool is_alive;
    bool is_incantation;
    bool locked_in;
} client_t;

typedef struct egg_s {
    int id;
    int team_id;
    int x;
    int y;
    uint64_t laid_time;
    struct egg_s *next;
} egg_t;

typedef struct team_s {
    char name[MAX_TEAM_NAME];
    int id;
    int max_clients;
    int connected_clients;
    egg_t *eggs;
    int available_slots;
} team_t;

typedef struct server_s {
    int socket_fd;
    int port;
    struct pollfd *pollfds;
    int poll_count;
    int max_fds;
    world_t world;
    client_t *clients[MAX_CLIENTS];
    int client_count;
    team_t teams[MAX_TEAMS];
    int nb_teams;
    int clients_per_team;
    int freq;
    uint64_t start_time;
    bool running;
    uint64_t resource_time;
    id_generator_t gen;
} server_t;

typedef struct direction_s {
    int from_x;
    int from_y;
    int to_x;
    int to_y;
} direction_t;

int parse_port(const char *str);
int parse_positive_int(const char *str);
int parse_team_names(int argc, char **argv, server_t *server);
int validate_server_config(server_t *server);

int init_socket(server_t *server);
int init_world(world_t *world);
int init_teams(server_t *server);
int init_resources(world_t *world);
void init_id_generator(id_generator_t *gen);
int initialize_tile_row(world_t *world, int y);
int fill_world_resources(server_t *server);
int count_resource_around(const world_t *world,
    resource_type_t resource);

int run_server(server_t *server);
int get_unique_id(id_generator_t *gen);

int accept_client(server_t *server);
int handle_client_data(server_t *server, int client_idx);
void disconnect_client(server_t *server, int client_idx);

int move_commands(server_t *server, client_t *client, command_t *cmd);
int other_commands(server_t *server, client_t *client, command_t *cmd);
int process_command(server_t *server, client_t *client, const char *command);

int move_forward(server_t *server, client_t *client);
int look_around(server_t *server, client_t *client);
int check_inventory(client_t *client);
int execute_set_command(server_t *server, client_t *client,
    command_t *cmd);
int execute_take_command(server_t *server, client_t *client,
    command_t *cmd);
int broadcast_message(server_t *server, client_t *client,
    const char *message);
int fork_player(server_t *server, client_t *client);
int eject_players(server_t *server, client_t *client);
int start_incantation(server_t *server, client_t *client,
    uint64_t current_time, command_t *cmd);
int check_available_slots(server_t *server, client_t *client);
int broadcast_command(server_t *server, client_t *client, command_t *cmd);

void spawn_resources(world_t *world, int freq);
void update_world(server_t *server);

int find_team_by_name(server_t *server, const char *name);
int add_client_to_team(server_t *server, client_t *client,
    const char *team_name);

void cleanup_clients(server_t *server);
void cleanup_teams(server_t *server);
void cleanup_world(server_t *world);
void cleanup_socket(server_t *server);
void cleanup_server_client(client_t *client);

int get_tile_index(world_t *world, int x, int y);
void normalize_coordinates(world_t *world, int *x, int *y);
int calculate_direction(direction_t direction, world_t *world);
uint64_t get_current_time(void);

void send_client_death_message(server_t *server, client_t *client);
void remove_client(server_t *server, client_t *client);
void remove_client_pollfd(server_t *server, int fd);
void remove_egg(const egg_t *egg, tile_t *tile);
void remove_egg_from_team(team_t *team, int egg_id);
int count_eggs(egg_t *eggs);
int process_client_buffer(server_t *server, client_t *client);
void process_client_commands(server_t *server, client_t *client);
int sent(int fd, const char *msg, const size_t size, const int flag);

#endif /* SERVER_H_ */
