#ifndef FEED_ADD_H
#define FEED_ADD_H

#include "feed.h"

// @brief Check if the current clique is valid
// @param users_graph: The graph of users
// @param visited: The visited array
// @return true if the clique is valid, false otherwise
bool valid_clique(list_graph_t *users_graph, visit_t visited[MAX_PEOPLE]);

// @brief Count the number of users in the clique
// @param visited: The visited array
// @return The number of users in the clique
unsigned int count_clique(visit_t visited[MAX_PEOPLE]);

// @brief Backtracking function to find the maximum clique
// @param users_graph: The graph of users
// @param user: The current user
// @param visited: The visited array
// @param max_clique: The maximum number of users in the clique
// @param max_visited: The visited array of the maximum clique
// @param checked: The hashtable of checked cliques
void backtracking(list_graph_t *users_graph, unsigned int user,
				  visit_t *visited, unsigned int *max_clique,
				  visit_t *max_visited, hashtable_t *checked);

#endif // FEED_ADD_H
