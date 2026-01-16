#ifndef PLAYER_H
#define PLAYER_H

#include "types.h"   /* Status, Direction, Treasure */

/* ============================================================
 * Player (First-Class Entity)
 *
 * Represents the player's persistent state across the world.
 *
 * Responsibilities:
 *   • Track current room ID and position
 *   • Support reset to initial state
 *
 * Design notes:
 *   • Player never stores Room pointers (IDs only)
 *   • All movement logic lives in GameEngine
 * ============================================================ */

typedef struct Player {
    int room_id;                 /* Current room ID */
    int x;                       /* X position within room */
    int y;                       /* Y position within room */

} Player;


/* ============================================================
 * Lifecycle
 * ============================================================ */

/*
 * Create a new player at the given room and position.
 *
 * Preconditions:
 *   player_out must not be NULL.
 *
 * Returns:
 *   OK on success (player_out set)
 *   INVALID_ARGUMENT if player_out is NULL
 *   NO_MEMORY on allocation failure
 */
Status player_create(int initial_room_id, int initial_x, int initial_y, Player **player_out);

/*
 * Destroy the player and free all owned memory.
 *
 * Must be safe to call on NULL.
 */
void player_destroy(Player *p);


/* ============================================================
 * Position and Room State
 * ============================================================ */

/*
 * Get the ID of the room the player is currently in.
 *
 * Returns:
 *   Room ID on success
 *   -1 if p is NULL
 */
int player_get_room(const Player *p);

/*
 * Get the player's current position.
 *
 * Returns:
 *   OK on success (x_out and y_out are set)
 *   INVALID_ARGUMENT if any pointer is NULL
 */
Status player_get_position(const Player *p, int *x_out, int *y_out);

/*
 * Set the player's position within the current room (no validation).
 *
 * Returns:
 *   OK on success
 *   INVALID_ARGUMENT if player is NULL
 */
Status player_set_position(Player *p, int x, int y);

/*
 * Transition the player to a different room (position unchanged).
 *
 * Returns:
 *   OK on success
 *   INVALID_ARGUMENT if player is NULL
 */
Status player_move_to_room(Player *p, int new_room_id);


/* ============================================================
 * Reset
 * ============================================================ */

/*
 * Reset the player to an initial state:
 *   • Resets room ID and position
 *
 * Returns:
 *   OK on success
 *   INVALID_ARGUMENT if player is NULL
 */
Status player_reset_to_start(Player *p,
                             int starting_room_id,
                             int start_x,
                             int start_y);

#endif /* PLAYER_H */
