#ifndef GRAPH_H
#define GRAPH_H

#include <stdbool.h>

/* ============================================================
 * Function Pointer Types
 * ============================================================ */

/*
 * Compare two payloads.
 *
 * Returns:
 *   0 if equal
 *   <0 or >0 for ordering otherwise
 */
typedef int (*GraphCompareFn)(const void *a, const void *b);

/*
 * Destroy a payload stored in the graph.
 *
 * Called once for each payload if destroy_fn != NULL.
 */
typedef void (*GraphDestroyFn)(void *payload);

/* ============================================================
 * Graph Status Codes
 * 
 * The Graph module uses its own status code system because it is
 * a standalone, reusable ADT independent of Treasure Runner.
 * When integrating graph operations, you may need to map these
 * codes to the Treasure Runner Status enum as appropriate.
 * ============================================================ */

typedef enum GraphStatus {
    GRAPH_STATUS_OK = 0,
    GRAPH_STATUS_NULL_ARGUMENT,
    GRAPH_STATUS_DUPLICATE_PAYLOAD,
    GRAPH_STATUS_DUPLICATE_EDGE,
    GRAPH_STATUS_NOT_FOUND,
    GRAPH_STATUS_NO_MEMORY
} GraphStatus;

/* ============================================================
 * Opaque Type
 * ============================================================ */
typedef struct Graph Graph;

/* ============================================================
 * CREATE: Graph Creation & Destruction
 * ============================================================ */

/*
 * Create an empty graph.
 *
 * compare_fn:
 *   Used to match payloads when adding edges or searching.
 *   Must not be NULL.
 *
 * destroy_fn:
 *   Used to free payloads when the graph is destroyed.
 *   May be NULL if the caller retains ownership of payloads.
 *
 * graph_out:
 *   On success, receives a pointer to the new Graph.
 *
 * Returns:
 *   GRAPH_STATUS_OK on success
 *   GRAPH_STATUS_NULL_ARGUMENT if inputs are NULL
 *   GRAPH_STATUS_NO_MEMORY on allocation failure
 */
GraphStatus graph_create(GraphCompareFn compare_fn,
                         GraphDestroyFn destroy_fn,
                         Graph **graph_out);

/*
 * Destroy the graph.
 *
 * If destroy_fn != NULL:
 *   - destroy_fn(payload) is called once for each stored payload.
 *
 * All graph memory is freed.
 *
 * Must be safe to call on NULL.
 */
void graph_destroy(Graph *g);

/* ============================================================
 * CREATE: Payload Insertion
 * ============================================================ */

/*
 * Insert a payload into the graph.
 *
 * Payloads must be unique as determined by compare_fn.
 *
 * Returns:
 *   GRAPH_STATUS_OK on success
 *   GRAPH_STATUS_DUPLICATE_PAYLOAD if payload already exists
 *   GRAPH_STATUS_NO_MEMORY on allocation failure
 *   GRAPH_STATUS_NULL_ARGUMENT if any argument is NULL
 */
GraphStatus graph_insert(Graph *g, void *payload);

/* ============================================================
 * CREATE: Edge Creation Between Payloads
 * ============================================================ */

/*
 * Add a directed edge from payload 'from' to payload 'to'.
 *
 * Returns:
 *   GRAPH_STATUS_OK on success
 *   GRAPH_STATUS_NOT_FOUND if either payload is not found
 *   GRAPH_STATUS_DUPLICATE_EDGE if the edge already exists
 *   GRAPH_STATUS_NULL_ARGUMENT if any argument is NULL
 */
GraphStatus graph_connect(Graph *g, const void *from, const void *to);

/* ============================================================
 * READ: Querying neighbours
 * ============================================================ */

/*
 * Retrieve neighbours of a payload.
 *
 * neighbours_out:
 *   On success, receives a pointer to an array of payload pointers.
 *
 * count_out:
 *   On success, receives the number of neighbours.
 *
 * Returns:
 *   GRAPH_STATUS_OK on success
 *   GRAPH_STATUS_NOT_FOUND if payload is not found
 *   GRAPH_STATUS_NULL_ARGUMENT if any argument is NULL
 */
GraphStatus graph_get_neighbours(const Graph *g,
                                const void *payload,
                                const void * const **neighbours_out,
                                int *count_out);

/* ============================================================
 * READ: Payload Lookup
 * ============================================================ */

/*
 * Retrieve the stored payload matching key.
 *
 * Returns:
 *   pointer to payload if found
 *   NULL if not found or if arguments are invalid
 */
const void *graph_get_payload(const Graph *g, const void *key);

/* ============================================================
 * READ: Graph Inspection & Queries
 * ============================================================ */

/*
 * Return the number of payload nodes stored in the graph.
 *
 * Returns:
 *   Number of nodes, or 0 if g is NULL
 */
int graph_size(const Graph *g);

/*
 * Determine whether a payload exists in the graph.
 *
 * Returns:
 *   true if payload exists
 *   false otherwise or if g is NULL
 */
bool graph_contains(const Graph *g, const void *payload);

/*
 * Return the number of outgoing edges from payload.
 *
 * Returns:
 *   Number of outgoing edges, or 0 if payload not found or g is NULL
 */
int graph_outdegree(const Graph *g, const void *payload);

/*
 * Check whether a directed edge exists from -> to.
 *
 * Returns:
 *   true if the edge exists
 *   false otherwise or if g is NULL
 */
bool graph_has_edge(const Graph *g, const void *from, const void *to);

/*
 * Return the number of incoming edges to payload.
 *
 * Returns:
 *   Number of incoming edges, or 0 if payload not found or g is NULL
 */
int graph_indegree(const Graph *g, const void *payload);

/*
 * Return the total number of edges in the graph.
 *
 * Returns:
 *   Total edge count, or 0 if g is NULL
 */
int graph_edge_count(const Graph *g);

/*
 * Retrieve all payloads in insertion order.
 *
 * Payload pointers remain valid until the graph is modified.
 *
 * Returns:
 *   GRAPH_STATUS_OK on success
 *   GRAPH_STATUS_NULL_ARGUMENT if any argument is NULL
 */
GraphStatus graph_get_all_payloads(const Graph *g,
                                   const void * const **payloads_out,
                                   int *count_out);

/*
 * Determine if payload 'to' is reachable from payload 'from'.
 *
 * Returns:
 *   true if 'to' is reachable from 'from'
 *   false otherwise or if g is NULL or payloads not found
 */
bool graph_reachable(const Graph *g, const void *from, const void *to);

/*
 * Determine whether the graph contains at least one cycle.
 *
 * Returns:
 *   true if the graph has at least one cycle
 *   false otherwise or if g is NULL
 */
bool graph_has_cycle(const Graph *g);

/*
 * Determine whether the graph is fully connected.
 *
 * A graph is considered connected if all payloads are reachable
 * from the first inserted payload, following directed edges.
 *
 * Returns:
 *   true if the graph is connected
 *   false otherwise or if g is NULL or empty
 */
bool graph_is_connected(const Graph *g);

/* ============================================================
 * DELETE: Removal Helpers
 * ============================================================ */

/*
 * Remove a directed edge from payload 'from' to payload 'to'.
 *
 * Returns:
 *   GRAPH_STATUS_OK if the edge was removed
 *   GRAPH_STATUS_NOT_FOUND if the edge or either payload is missing
 *   GRAPH_STATUS_NULL_ARGUMENT if any argument is NULL
 */
GraphStatus graph_disconnect(Graph *g, const void *from, const void *to);

/*
 * Remove a payload and all incident edges from the graph.
 *
 * Returns:
 *   GRAPH_STATUS_OK if the payload existed and was removed
 *   GRAPH_STATUS_NOT_FOUND if the payload is missing
 *   GRAPH_STATUS_NULL_ARGUMENT if any argument is NULL
 */
GraphStatus graph_remove(Graph *g, const void *payload);

#endif /* GRAPH_H */
