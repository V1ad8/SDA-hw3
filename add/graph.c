#include "graph.h"

int is_node_in_graph(int n, int nodes)
{
	// Check if the node is in the graph
	return n >= 0 && n < nodes;
}

list_graph_t *lg_create(int nodes)
{
	// Allocate memory for the graph
	list_graph_t *g = malloc(sizeof(*g));
	DIE(!g, "malloc graph failed");

	// Allocate memory for the neighbors
	g->neighbors = malloc(nodes * sizeof(*g->neighbors));
	DIE(!g->neighbors, "malloc neighbours failed");

	// Initialize the neighbors
	for (int i = 0; i != nodes; ++i)
		g->neighbors[i] = ll_create(sizeof(int));

	// Initialize the number of nodes
	g->nodes = nodes;

	// Return the graph
	return g;
}

void lg_add_edge(list_graph_t *graph, int src, int dest)
{
	// Check if the graph is NULL or if the nodes are invalid
	if (!graph || !graph->neighbors || !is_node_in_graph(src, graph->nodes) ||
		!is_node_in_graph(dest, graph->nodes))
		return;

	// Add the edge
	ll_add_nth_node(graph->neighbors[src], graph->neighbors[src]->size, &dest,
					ll_create_node);
}

ll_node_t *find_node(ll_list_t *ll, int node, unsigned int *pos)
{
	// Helper variables
	ll_node_t *crt = ll->head;
	unsigned int i;

	// Find the node
	for (i = 0; i != ll->size; ++i) {
		if (node == *(int *)crt->data) {
			*pos = i;
			return crt;
		}

		crt = crt->next;
	}

	// Return NULL if the node was not found
	return NULL;
}

int lg_has_edge(list_graph_t *graph, int src, int dest)
{
	// Helper variable
	unsigned int pos;

	// Check if the graph is NULL or if the nodes are invalid
	if (!graph || !graph->neighbors || !is_node_in_graph(src, graph->nodes) ||
		!is_node_in_graph(dest, graph->nodes))
		return 0;

	// Check if the edge exists
	// Weird return statement because of the style checker
	return !(!find_node(graph->neighbors[src], dest, &pos));
}

ll_list_t *lg_get_neighbours(list_graph_t *graph, int node)
{
	// Check if the graph is NULL or if the nodes are invalid
	if (!graph || !graph->neighbors || !is_node_in_graph(node, graph->nodes))
		return NULL;

	// Return the neighbors of the node
	return graph->neighbors[node];
}

void lg_remove_edge(list_graph_t *graph, int src, int dest)
{
	// Helper variable
	unsigned int pos;

	// Check if the graph is NULL or if the nodes are invalid
	if (!graph || !graph->neighbors || !is_node_in_graph(src, graph->nodes) ||
		!is_node_in_graph(dest, graph->nodes))
		return;

	// Check if the edge exists
	if (!find_node(graph->neighbors[src], dest, &pos))
		return;

	// Remove the edge and free the data
	ll_node_t *removed = ll_remove_nth_node(graph->neighbors[src], pos);
	free(removed->data);
	free(removed);
}

void lg_free(list_graph_t *graph)
{
	// Check if the graph is NULL
	if (!graph)
		return;

	// Free the neighbors
	for (int i = 0; i != graph->nodes; ++i)
		ll_free(graph->neighbors + i);

	// Free the graph
	free(graph->neighbors);
	free(graph);
}
