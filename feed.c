#include "feed.h"

void handle_input_feed(char *input, list_graph_t *users_graph)
{
	// Duplicate the input string and get the command
	char *commands = strdup(input);
	DIE(!commands, "Malloc failed while handling input");

	char *cmd = strtok(commands, "\n ");
	DIE(!cmd, "No command found");

	if (!strcmp(cmd, "feed")) {
		(void)cmd;
		// TODO: Add function
	} else if (!strcmp(cmd, "view-profile")) {
		(void)cmd;
		// TODO: Add function
	} else if (!strcmp(cmd, "friends-repost")) {
		(void)cmd;
		// TODO: Add function
	} else if (!strcmp(cmd, "common-group")) {
		// Get the user
		char *user = strtok(NULL, "\n ");

		// Find the maximum clique
		clique(users_graph, get_user_id(user));
	}

	// Free the commands
	free(commands);
}

void clique(list_graph_t *users_graph, unsigned int user)
{
	// Declare the visited array
	visit_t visited[MAX_PEOPLE];

	// Initialize the visited array
	for (unsigned int i = 0; i < MAX_PEOPLE; i++)
		if (lg_has_edge(users_graph, user, i))
			visited[i] = NOT_VISITED;
		else
			visited[i] = NOT_IN_CLIQUE;
	visited[user] = IN_CLIQUE;

	// Declare the maximum clique and its visited array
	unsigned int max_clique = 1;
	visit_t max_visited[MAX_PEOPLE];
	for (unsigned int i = 0; i < MAX_PEOPLE; i++)
		max_visited[i] = NOT_VISITED;
	visited[user] = IN_CLIQUE;

	// Create the hashtable for the checked cliques
	hashtable_t *ht = ht_create(MAX_PEOPLE, hash_string, compare_strings);

	// Start the backtracking
	backtracking(users_graph, user, visited, &max_clique, max_visited, ht);

	// Print the maximum clique
	printf("The closest friend group of %s is:\n", get_user_name(user));

	// Print the users in the maximum clique
	for (unsigned int i = 0; i < MAX_PEOPLE; i++) {
		if (max_visited[i] == IN_CLIQUE)
			printf("%s\n", get_user_name(i));
	}

	// Free the hashtable
	ht_free(ht);
}
