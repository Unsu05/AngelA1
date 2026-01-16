#ifndef TYPES_H
#define TYPES_H

#include <stdbool.h>



/* ============================================================
 * Status Codes
 *
 * Unified status codes returned by all subsystems.
 * Each module uses a subset of these values.
 * ============================================================ */

typedef enum {
    /* Success */
    OK = 0,

    /* Generic errors */
    INVALID_ARGUMENT,
    NULL_POINTER,
    NO_MEMORY,
    BOUNDS_EXCEEDED,
    INTERNAL_ERROR,

    /* Room-specific errors */
    ROOM_IMPASSABLE,
    ROOM_NO_PORTAL,
    ROOM_NOT_FOUND,

    /* GameEngine-specific errors */
    GE_NO_SUCH_ROOM,

    
    /* WorldLoader-specific errors */
    WL_ERR_CONFIG,
    WL_ERR_DATAGEN
} Status;


/* ============================================================
 * Direction
 *
 * Cardinal movement directions used by the GameEngine.
 * ============================================================ */

typedef enum {
    DIR_NORTH = 0,
    DIR_SOUTH,
    DIR_EAST,
    DIR_WEST
} Direction;


/* ============================================================
 * Portal
 *
 * Static link from one room to another.
 * Portals never move and never change destination.
 * ============================================================ */

typedef struct {
    int id;                 /* Unique within the room */
    char *name;             /* Portal name (owned by room) */
    int x;                  /* Tile position */
    int y;                  /* Tile position */
    int target_room_id;     /* Destination room ID */
} Portal;


/* ============================================================
 * Treasure
 *
 * Collectible object with persistent identity.
 *
 * NOTE: In A1, treasures are visual only - the 'collected' field
 * exists for future assignments but is not used in A1 movement logic.
 * Players walk through treasures without collecting them.
 *
 * In later assignments, treasures will move from rooms to the player
 * when collected.
 * ============================================================ */

typedef struct {
    int id;                 /* Globally unique */
    char *name;             /* Treasure name */
    int starting_room_id;   /* For reset */
    int initial_x;          /* Initial position */
    int initial_y;
    int x;                  /* Current position */
    int y;
    bool collected;         /* True if collected */
} Treasure;


/* ============================================================
 * Charset
 *
 * Rendering characters used for ASCII display.
 *
 * NOTE: The 'pushable' field is for future assignments (A2/A3)
 * and is not used in A1.
 * ============================================================ */

typedef struct {
    char wall;
    char floor;
    char player;
    char treasure;
    char portal;
    char pushable; 
} Charset;


/* Forward declaration for opaque Room in public APIs */
typedef struct Room Room;

#endif /* TYPES_H */
