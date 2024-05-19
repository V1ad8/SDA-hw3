#ifndef LABIMPLEM_H
#define LABIMPLEM_H

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "add/linked_list.h"
#include "add/queue.h"
#include "utils.h"

#define MIN(x, y) ((x) < (y) ? (x) : (y))

#define INF 1 << 30

#define MAX_QUEUE_SIZE 100

typedef struct list_graph_t list_graph_t;

struct list_graph_t {
	ll_list_t **neighbors;
	int nodes;
};

list_graph_t *lg_create(int nodes);
void lg_add_edge(list_graph_t *graph, int src, int dest);
ll_node_t *find_node(ll_list_t *ll, int node, unsigned int *pos);
int lg_has_edge(list_graph_t *graph, int src, int dest);
ll_list_t *lg_get_neighbours(list_graph_t *graph, int node);
void lg_remove_edge(list_graph_t *graph, int src, int dest);
void lg_free(list_graph_t *graph);

#endif
