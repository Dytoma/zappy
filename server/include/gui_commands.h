/*
** EPITECH PROJECT, 2025
** server
** File description:
** gui_commands
*/

#ifndef GUI_COMMANDS_H_
    #define GUI_COMMANDS_H_
    #include "server.h"

int map_size(server_t *server, client_t *gui, command_t *cmd);
int tile_content(server_t *server, client_t *gui, command_t *cmd);
int map_content(server_t *server, client_t *gui, command_t *cmd);
int name_of_teams(server_t *server, client_t *gui, command_t *cmd);
int player_position(server_t *server, client_t *gui, command_t *cmd);
int player_level(server_t *server, client_t *gui, command_t *cmd);
int player_inventory(server_t *server, client_t *gui, command_t *cmd);
int time_unit_request(server_t *server, client_t *gui, command_t *cmd);
int time_unit_modification(server_t *server, client_t *gui, command_t *cmd);
void notify_player_expulsion(server_t *server, int player_id);
void notify_incantation_start(server_t *server, int x, int y, int level);
void notify_incantation_end(server_t *server, int x, int y, int result);
void notify_egg_laying(server_t *server, int player_id);
void notify_egg_death(server_t *server, int egg_id);
void notify_game_end(server_t *server, const char *winning_team);
void notify_server_message(server_t *server, const char *message);
void notify_player_position(server_t *server, client_t *client);
void notify_player_level(server_t *server, client_t *client);

int unknown_cmd(client_t *gui);
int bad_param_cmd(client_t *gui);
void process_gui_cmd(server_t *server, client_t *client, command_t *cmd);

#endif /* !GUI_COMMANDS_H_ */
