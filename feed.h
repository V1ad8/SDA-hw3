#ifndef FEED_H
#define FEED_H

#include "utils.h"
#include "users.h"
#include "friends.h"
#include "add/graph.h"
#include "add/hashtable.h"

// Enumeration for the visit status of a user in the clique
typedef enum visit_t { NOT_VISITED, IN_CLIQUE, NOT_IN_CLIQUE } visit_t;

#include "feed_add.h"

/**
 * Function that handles the calling of every command from task 3
 *
 * Please add any necessary parameters to the functions
*/
void handle_input_feed(char *input, list_graph_t *users_graph);

// @brief Function that finds the maximum clique of a user
// @param users_graph: The graph of users
// @param user: The user to find the maximum clique for
void clique(list_graph_t *users_graph, unsigned int user);

#endif // FEED_H
