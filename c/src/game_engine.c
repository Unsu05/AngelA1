#include "game_engine.h"
#include <stddef.h>

Status game_engine_create(const char *config_file_path, GameEngine **engine_out) {
    (void)config_file_path;
    (void)engine_out;

    return INTERNAL_ERROR;
}

void game_engine_destroy(GameEngine *eng) {
    (void)eng;
}

const Player *game_engine_get_player(const GameEngine *eng) {
    (void)eng;
    return NULL;
}

Status game_engine_move_player(GameEngine *eng, Direction dir) {
    (void)eng;
    (void)dir;

    return INTERNAL_ERROR;
}

Status game_engine_get_room_count(const GameEngine *eng, int *count_out) {
    (void)eng;
    (void)count_out;

    return INTERNAL_ERROR;
}

Status game_engine_get_room_dimensions(const GameEngine *eng,
                                       int *width_out,
                                       int *height_out) {
    (void)eng;
    (void)width_out;
    (void)height_out;

    return INTERNAL_ERROR;
}

Status game_engine_reset(GameEngine *eng) {
    (void)eng;

    return INTERNAL_ERROR;
}

Status game_engine_render_current_room(const GameEngine *eng, char **str_out) {
    (void)eng;
    (void)str_out;

    return INTERNAL_ERROR;
}

Status game_engine_render_room(const GameEngine *eng, int room_id, char **str_out) {
    (void)eng;
    (void)room_id;
    (void)str_out;

    return INTERNAL_ERROR;
}

Status game_engine_get_room_ids(const GameEngine *eng, int **ids_out, int *count_out) {
    (void)eng;
    (void)ids_out;
    (void)count_out;

    return INTERNAL_ERROR;
}
