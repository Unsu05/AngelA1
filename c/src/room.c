#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "room.h"

/* ============================================================
 * Creation
 * ============================================================ */

Room *room_create(int id, const char *name, int width, int height) {
    Room *r = malloc(sizeof(Room));
    if (r == NULL) {
        return NULL;
    }

    r->id = id;
    r->width = width;
    r->height = height;

    // Deep copy the name if provided
    if (name != NULL) {
        r->name = strdup(name);
        if (r->name == NULL) {
            free(r);
            return NULL;
        }
    } else {
        r->name = NULL;
    }

    // Initialize all other pointers to safe defaults
    r->floor_grid = NULL;
    r->portals = NULL;
    r->portal_count = 0;
    r->treasures = NULL;
    r->treasure_count = 0;

    return r;
}

/* ============================================================
 * Destruction
 * ============================================================ */

void room_destroy(Room *r) {
    if (r == NULL) {
        return;
    }

    // Free name
    if (r->name) free(r->name);

    // Free floor grid
    if (r->floor_grid) free(r->floor_grid);

    // Free portals and their names
    if (r->portals) {
        for (int i = 0; i < r->portal_count; i++) {
            if (r->portals[i].name) {
                free(r->portals[i].name);
            }
        }
        free(r->portals);
    }

    // Free treasures and their names
    if (r->treasures) {
        for (int i = 0; i < r->treasure_count; i++) {
            if (r->treasures[i].name) {
                free(r->treasures[i].name);
            }
        }
        free(r->treasures);
    }

    // Finally, free the container
    free(r);
}

/* ============================================================
 * Basic Queries
 * ============================================================ */

int room_get_width(const Room *r) {
    return (r != NULL) ? r->width : 0;
}

int room_get_height(const Room *r) {
    return (r != NULL) ? r->height : 0;
}

/* ============================================================
 * Room Data Setters
 * ============================================================ */

Status room_set_floor_grid(Room *r, bool *floor_grid) {
    if (r == NULL) return INVALID_ARGUMENT;

    // Free existing grid if present
    if (r->floor_grid != NULL) {
        free(r->floor_grid);
    }

    r->floor_grid = floor_grid;
    return OK;
}

Status room_set_portals(Room *r, Portal *portals, int portal_count) {
    if (r == NULL) return INVALID_ARGUMENT;
    if (portal_count > 0 && portals == NULL) return INVALID_ARGUMENT;

    // Clean up old portals
    if (r->portals != NULL) {
        for (int i = 0; i < r->portal_count; i++) {
            if (r->portals[i].name) free(r->portals[i].name);
        }
        free(r->portals);
    }

    r->portals = portals;
    r->portal_count = portal_count;
    return OK;
}

Status room_set_treasures(Room *r, Treasure *treasures, int treasure_count) {
    if (r == NULL) return INVALID_ARGUMENT;
    if (treasure_count > 0 && treasures == NULL) return INVALID_ARGUMENT;

    // Clean up old treasures
    if (r->treasures != NULL) {
        for (int i = 0; i < r->treasure_count; i++) {
            if (r->treasures[i].name) free(r->treasures[i].name);
        }
        free(r->treasures);
    }

    r->treasures = treasures;
    r->treasure_count = treasure_count;
    return OK;
}

/* ============================================================
 * Treasure Management
 * ============================================================ */

Status room_place_treasure(Room *r, const Treasure *treasure) {
    if (r == NULL || treasure == NULL) return INVALID_ARGUMENT;

    // Expand the array
    int new_count = r->treasure_count + 1;
    Treasure *new_arr = realloc(r->treasures, new_count * sizeof(Treasure));

    if (new_arr == NULL) {
        return NO_MEMORY;
    }

    r->treasures = new_arr;
    
    // Copy the data into the new slot
    r->treasures[r->treasure_count] = *treasure; // struct copy
    
    // Deep copy the name to ensure independence
    if (treasure->name) {
        r->treasures[r->treasure_count].name = strdup(treasure->name);
        if (r->treasures[r->treasure_count].name == NULL) {
             return NO_MEMORY; 
        }
    } else {
        r->treasures[r->treasure_count].name = NULL;
    }

    r->treasure_count = new_count;
    return OK;
}

/* ============================================================
 * Logic & Queries
 * ============================================================ */

int room_get_treasure_at(const Room *r, int x, int y) {
    if (r == NULL || r->treasures == NULL) return -1;

    for (int i = 0; i < r->treasure_count; i++) {
        if (!r->treasures[i].collected && 
            r->treasures[i].x == x && 
            r->treasures[i].y == y) {
            return r->treasures[i].id;
        }
    }
    return -1;
}

int room_get_portal_destination(const Room *r, int x, int y) {
    if (r == NULL || r->portals == NULL) return -1;

    for (int i = 0; i < r->portal_count; i++) {
        if (r->portals[i].x == x && r->portals[i].y == y) {
            return r->portals[i].target_room_id;
        }
    }
    return -1;
}

bool room_is_walkable(const Room *r, int x, int y) {
    if (r == NULL) return false;

    // Boundary check
    if (x < 0 || x >= r->width || y < 0 || y >= r->height) {
        return false;
    }

    // Wall check (floor_grid false = wall)
    if (r->floor_grid) {
        return r->floor_grid[y * r->width + x];
    }
    
    // Fallback if no grid (shouldn't happen in valid room)
    return false;
}

RoomTileType room_classify_tile(const Room *r, int x, int y, int *out_id) {
    if (r == NULL) return ROOM_TILE_INVALID;

    // 1. Check Treasure
    int t_id = room_get_treasure_at(r, x, y);
    if (t_id != -1) {
        if (out_id) *out_id = t_id;
        return ROOM_TILE_TREASURE;
    }

    // 2. Check Portal
    int p_dest = room_get_portal_destination(r, x, y);
    if (p_dest != -1) {
        if (out_id) *out_id = p_dest;
        return ROOM_TILE_PORTAL;
    }

    // 3. Check Floor/Wall
    if (room_is_walkable(r, x, y)) {
        return ROOM_TILE_FLOOR;
    }

    return ROOM_TILE_WALL;
}

Status room_get_start_position(const Room *r, int *x_out, int *y_out) {
    if (r == NULL || x_out == NULL || y_out == NULL) return INVALID_ARGUMENT;

    // Strategy 1: First portal
    if (r->portal_count > 0) {
        *x_out = r->portals[0].x;
        *y_out = r->portals[0].y;
        return OK;
    }

    // Strategy 2: First walkable tile
    for (int y = 0; y < r->height; y++) {
        for (int x = 0; x < r->width; x++) {
            if (room_is_walkable(r, x, y)) {
                *x_out = x;
                *y_out = y;
                return OK;
            }
        }
    }

    return ROOM_NOT_FOUND;
}

/* ============================================================
 * Rendering
 * ============================================================ */

Status room_render(const Room *r, const Charset *charset, char *buffer, int buffer_width, int buffer_height) {
    if (r == NULL || charset == NULL || buffer == NULL) return INVALID_ARGUMENT;
    
    // Validate buffer size matches room
    if (buffer_width != r->width || buffer_height != r->height) {
        return INVALID_ARGUMENT;
    }

    // 1. Fill base layer (Walls/Floors)
    for (int i = 0; i < r->width * r->height; i++) {
        if (r->floor_grid && r->floor_grid[i]) {
            buffer[i] = charset->floor;
        } else {
            buffer[i] = charset->wall;
        }
    }

    // 2. Overlay Treasures
    for (int i = 0; i < r->treasure_count; i++) {
        Treasure *t = &r->treasures[i];
        if (!t->collected) {
            int idx = t->y * r->width + t->x;
            if (idx >= 0 && idx < r->width * r->height) {
                buffer[idx] = charset->treasure;
            }
        }
    }

    // 3. Overlay Portals
    for (int i = 0; i < r->portal_count; i++) {
        Portal *p = &r->portals[i];
        int idx = p->y * r->width + p->x;
        if (idx >= 0 && idx < r->width * r->height) {
            buffer[idx] = charset->portal;
        }
    }

    return OK;
}