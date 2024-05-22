#ifndef FEED_H
#define FEED_H

#include "add/graph.h"

typedef enum check_t { NOT_VISITED, IN_CLIQUE, NOT_IN_CLIQUE } check_t;

/**
 * Function that handles the calling of every command from task 3
 *
 * Please add any necessary parameters to the functions
*/
void handle_input_feed(char *input, list_graph_t *users_graph);

void clique(list_graph_t *users_graph, unsigned int user);

#endif // FEED_H
