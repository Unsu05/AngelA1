#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "room.h"

/* ============================================================
 * Helper: Internal Tile Logic
 * ============================================================ */
static bool internal_is_floor(const Room *r, int col, int row) {
    // If we have an explicit grid, use it
    if (r->floor_grid != NULL) {
        return r->floor_grid[row * r->width + col];
    }
    
    // Implicit Mode: Perimeter is wall, interior is floor
    bool is_boundary = (col == 0 || col == r->width - 1 || 
                        row == 0 || row == r->height - 1);
    return !is_boundary;
}

/* ============================================================
 * Creation
 * ============================================================ */

Room *room_create(int id, const char *name, int width, int height) {
    // Fix: Clamp dimensions to minimum 1x1 to satisfy tests
    if (width < 1) width = 1;
    if (height < 1) height = 1;

    Room *r = malloc(sizeof(Room));
    if (r == NULL) {
        return NULL;
    }

    r->id = id;
    r->width = width;
    r->height = height;

    if (name != NULL) {
        r->name = strdup(name);
        if (r->name == NULL) {
            free(r);
            return NULL;
        }
    } else {
        r->name = NULL;
    }

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

    if (r->name) free(r->name);
    if (r->floor_grid) free(r->floor_grid);

    if (r->portals) {
        // Fix: Changed 'i' to 'idx' to be safe, though 'i' is standard
        for (int idx = 0; idx < r->portal_count; idx++) {
            if (r->portals[idx].name) {
                free(r->portals[idx].name);
            }
        }
        free(r->portals);
    }

    if (r->treasures) {
        for (int idx = 0; idx < r->treasure_count; idx++) {
            if (r->treasures[idx].name) {
                free(r->treasures[idx].name);
            }
        }
        free(r->treasures);
    }

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

    if (r->floor_grid != NULL) {
        free(r->floor_grid);
    }

    r->floor_grid = floor_grid;
    return OK;
}

Status room_set_portals(Room *r, Portal *portals, int portal_count) {
    if (r == NULL) return INVALID_ARGUMENT;
    if (portal_count > 0 && portals == NULL) return INVALID_ARGUMENT;

    if (r->portals != NULL) {
        for (int idx = 0; idx < r->portal_count; idx++) {
            if (r->portals[idx].name) free(r->portals[idx].name);
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

    if (r->treasures != NULL) {
        for (int idx = 0; idx < r->treasure_count; idx++) {
            if (r->treasures[idx].name) free(r->treasures[idx].name);
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

    int new_count = r->treasure_count + 1;
    Treasure *new_arr = realloc(r->treasures, new_count * sizeof(Treasure));

    if (new_arr == NULL) {
        return NO_MEMORY;
    }

    r->treasures = new_arr;
    r->treasures[r->treasure_count] = *treasure;
    
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

    for (int idx = 0; idx < r->treasure_count; idx++) {
        if (!r->treasures[idx].collected && 
            r->treasures[idx].x == x && 
            r->treasures[idx].y == y) {
            return r->treasures[idx].id;
        }
    }
    return -1;
}

int room_get_portal_destination(const Room *r, int x, int y) {
    if (r == NULL || r->portals == NULL) return -1;

    for (int idx = 0; idx < r->portal_count; idx++) {
        if (r->portals[idx].x == x && r->portals[idx].y == y) {
            return r->portals[idx].target_room_id;
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

    // Use our new helper that handles implicit grids
    return internal_is_floor(r, x, y);
}

RoomTileType room_classify_tile(const Room *r, int x, int y, int *out_id) {
    if (r == NULL) return ROOM_TILE_INVALID;

    int t_id = room_get_treasure_at(r, x, y);
    if (t_id != -1) {
        if (out_id) *out_id = t_id;
        return ROOM_TILE_TREASURE;
    }

    int p_dest = room_get_portal_destination(r, x, y);
    if (p_dest != -1) {
        if (out_id) *out_id = p_dest;
        return ROOM_TILE_PORTAL;
    }

    if (room_is_walkable(r, x, y)) {
        return ROOM_TILE_FLOOR;
    }

    return ROOM_TILE_WALL;
}

Status room_get_start_position(const Room *r, int *x_out, int *y_out) {
    if (r == NULL || x_out == NULL || y_out == NULL) return INVALID_ARGUMENT;

    if (r->portal_count > 0) {
        *x_out = r->portals[0].x;
        *y_out = r->portals[0].y;
        return OK;
    }

    // Fix: Rename loop variables for linter
    for (int row = 0; row < r->height; row++) {
        for (int col = 0; col < r->width; col++) {
            if (room_is_walkable(r, col, row)) {
                *x_out = col;
                *y_out = row;
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
    
    if (buffer_width != r->width || buffer_height != r->height) {
        return INVALID_ARGUMENT;
    }

    // 1. Fill base layer using internal logic (handles implicit grid)
    for (int row = 0; row < r->height; row++) {
        for (int col = 0; col < r->width; col++) {
            int idx = row * r->width + col;
            if (internal_is_floor(r, col, row)) {
                buffer[idx] = charset->floor;
            } else {
                buffer[idx] = charset->wall;
            }
        }
    }

    // 2. Overlay Treasures
    for (int idx = 0; idx < r->treasure_count; idx++) {
        Treasure *t = &r->treasures[idx];
        if (!t->collected) {
            int buf_idx = t->y * r->width + t->x;
            if (buf_idx >= 0 && buf_idx < r->width * r->height) {
                buffer[buf_idx] = charset->treasure;
            }
        }
    }

    // 3. Overlay Portals
    for (int idx = 0; idx < r->portal_count; idx++) {
        Portal *p = &r->portals[idx];
        int buf_idx = p->y * r->width + p->x;
        if (buf_idx >= 0 && buf_idx < r->width * r->height) {
            buffer[buf_idx] = charset->portal;
        }
    }

    return OK;
}