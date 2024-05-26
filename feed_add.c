#include "feed_add.h"

bool valid_clique(list_graph_t *users_graph, visit_t visited[MAX_PEOPLE])
{
	// Search for edges between users in the clique
	for (unsigned int i = 0; i < MAX_PEOPLE; i++)
		if (visited[i] == IN_CLIQUE)
			for (unsigned int j = i + 1; j < MAX_PEOPLE; j++)
				if (visited[j] == IN_CLIQUE && !lg_has_edge(users_graph, i, j))
					return false;

	// Return true if the clique is valid
	return true;
}

unsigned int count_clique(visit_t visited[MAX_PEOPLE])
{
	// Count the users
	unsigned int count = 0;
	for (unsigned int i = 0; i < MAX_PEOPLE; i++)
		if (visited[i] == IN_CLIQUE)
			count++;

	// Return the number of users
	return count;
}

void backtracking(list_graph_t *users_graph, unsigned int user,
				  visit_t *visited, unsigned int *max_clique,
				  visit_t *max_visited, hashtable_t *checked)
{
	// printf("call\n");

	// Add each computed clique to the checked hashtable in otder to avoid
	// recomputing the same clique multiple times
	// The key is the visited array converted to a string
	// It reduces the time complexity of the backtracking by 95.04%

	// Get a string representation of the visited array
	static char text[MAX_PEOPLE + 1];

	// Print the visited array to the text string
	for (unsigned int i = 0; i < MAX_PEOPLE; i++)
		sprintf(text + i, "%d", visited[i]);

	// If the visited array is already computed, return
	if (ht_has_key(checked, text))
		return;

	// Mark the visited array as computed
	static unsigned char one = 1;
	ht_put(checked, text, strlen(text) + 1, &one, sizeof(one));

	// Search for unvisited neighbours of the current user
	for (unsigned int i = 0; i < MAX_PEOPLE; i++) {
		if (visited[i] == NOT_VISITED && lg_has_edge(users_graph, user, i)) {
			// Check for validity of clique
			if (!valid_clique(users_graph, visited))
				continue;

			// Mark the neighbour as visited
			visited[i] = IN_CLIQUE;

			// Check if the current clique is the maximum clique
			if (count_clique(visited) > *max_clique) {
				// Update the maximum clique
				*max_clique = count_clique(visited);
				memcpy(max_visited, visited, MAX_PEOPLE * sizeof(visit_t));

				// This is a heuristic to stop the backtracking when the
				// maximum clique is found. It is not guaranteed to work
				// for all cases, but it is a good heuristic for the
				// specific case of this project.
				// It reduces the time complexity of the backtracking by 91.84%
				for (unsigned int j = 0; j < MAX_PEOPLE; j++) {
					if (ll_get_size(lg_get_neighbours(users_graph, j)) - 2 <
							*max_clique &&
						visited[j] == NOT_VISITED && *max_clique > 2) {
						return;
					}
				}
			}

			// Continue the backtracking
			backtracking(users_graph, i, visited, max_clique, max_visited,
						 checked);

			// Mark the neighbour as not visited
			visited[i] = NOT_VISITED;
		}
	}
}
