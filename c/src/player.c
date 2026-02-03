#include <stdlib.h>
#include "player.h"

/* ============================================================
 * Lifecycle
 * ============================================================ */

Status player_create(int initial_room_id, int initial_x, int initial_y, Player **player_out) {
    // Safety check: caller must provide a valid pointer to store the result
    if (player_out == NULL) {
        return INVALID_ARGUMENT;
    }

    Player *p = malloc(sizeof(Player));
    if (p == NULL) {
        return NO_MEMORY;
    }

    // Initialize state
    p->room_id = initial_room_id;
    p->x = initial_x;
    p->y = initial_y;

    *player_out = p;
    return OK;
}

void player_destroy(Player *p) {
    // Standard safe-free pattern
    if (p != NULL) {
        free(p);
    }
}

/* ============================================================
 * Position and Room State
 * ============================================================ */

int player_get_room(const Player *p) {
    if (p == NULL) {
        return -1; // Error sentinel as per API design
    }
    return p->room_id;
}

Status player_get_position(const Player *p, int *x_out, int *y_out) {
    if (p == NULL || x_out == NULL || y_out == NULL) {
        return INVALID_ARGUMENT;
    }

    *x_out = p->x;
    *y_out = p->y;
    return OK;
}

Status player_set_position(Player *p, int x, int y) {
    if (p == NULL) {
        return INVALID_ARGUMENT;
    }

    // Validation is the Game Engine's job; we just store the data.
    p->x = x;
    p->y = y;
    return OK;
}

Status player_move_to_room(Player *p, int new_room_id) {
    if (p == NULL) {
        return INVALID_ARGUMENT;
    }

    p->room_id = new_room_id;
    return OK;
}

/* ============================================================
 * Reset
 * ============================================================ */

Status player_reset_to_start(Player *p, int starting_room_id, int start_x, int start_y) {
    if (p == NULL) {
        return INVALID_ARGUMENT;
    }

    p->room_id = starting_room_id;
    p->x = start_x;
    p->y = start_y;
    
    return OK;
    
}