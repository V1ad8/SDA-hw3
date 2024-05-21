#ifndef LABIMPLEM_H
#define LABIMPLEM_H

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "linked_list.h"
#include "queue.h"
#include "../utils.h"

// Minimum macro
#define MIN(x, y)           \
	({                      \
		typeof(x) _x = (x); \
		typeof(y) _y = (y); \
		_x < _y ? _x : _y;  \
	})

// Infinity constant
#define INF (1 << 30)

typedef struct list_graph_t {
	ll_list_t **neighbors;
	int nodes;
} list_graph_t;

// @brief Create a new list graph
// @param nodes The number of nodes
// @return The new list graph
list_graph_t *lg_create(int nodes);

// @brief Add an edge to the list graph
// @param graph The list graph
// @param src The source node
// @param dest The destination node
void lg_add_edge(list_graph_t *graph, int src, int dest);

// @brief Find a node in the linked list
// @param ll The linked list
// @param node The node to be found
// @param pos The position of the node (output)
// @return The node if it was found, NULL otherwise
ll_node_t *find_node(ll_list_t *ll, int node, unsigned int *pos);

// @brief Check if an edge exists in the list graph
// @param graph The list graph
// @param src The source node
// @param dest The destination node
// @return 1 if the edge exists, 0 otherwise
int lg_has_edge(list_graph_t *graph, int src, int dest);

// @brief Get the neighbors of a node
// @param graph The list graph
// @param node The node
// @return The neighbors of the node
ll_list_t *lg_get_neighbours(list_graph_t *graph, int node);

// @brief Remove an edge from the list graph
// @param graph The list graph
// @param src The source node
// @param dest The destination node
void lg_remove_edge(list_graph_t *graph, int src, int dest);

// @brief Free the list graph
// @param graph The list graph
void lg_free(list_graph_t *graph);

#endif
