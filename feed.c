#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "feed.h"
#include "users.h"
#include "friends.h"
#include "add/hashtable.h"

void handle_input_feed(char *input, list_graph_t *users_graph)
{
	char *commands = strdup(input);
	char *cmd = strtok(commands, "\n ");

	if (!cmd)
		return;

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
		char *user = strtok(NULL, "\n ");

		(void)users_graph;
		clique(users_graph, get_user_id(user));
	}
	free(commands);
}

bool valid_clique(list_graph_t *users_graph, check_t visited[MAX_PEOPLE])
{
	for (unsigned int i = 0; i < MAX_PEOPLE; i++)
		if (visited[i] == IN_CLIQUE)
			for (unsigned int j = i + 1; j < MAX_PEOPLE; j++)
				if (visited[j] == IN_CLIQUE && !lg_has_edge(users_graph, i, j))
					return false;

	return true;
}

unsigned int count_clique(check_t visited[MAX_PEOPLE])
{
	unsigned int count = 0;
	for (unsigned int i = 0; i < MAX_PEOPLE; i++)
		if (visited[i] == IN_CLIQUE)
			count++;

	return count;
}

bool unvisited(check_t visited[MAX_PEOPLE])
{
	for (unsigned int i = 0; i < MAX_PEOPLE; i++)
		if (visited[i] == NOT_VISITED)
			return true;

	return false;
}

void backtracking(list_graph_t *users_graph, unsigned int user,
				  check_t *visited, unsigned int *max_clique,
				  check_t *max_visited, hashtable_t *checked)
{
	char *text = (char *)malloc((MAX_PEOPLE + 1) * sizeof(char));
	for (unsigned int i = 0; i < MAX_PEOPLE; i++)
		sprintf(text + i, "%d", visited[i]);

	if (ht_has_key(checked, text)) {
		free(text);
		return;
	}

	int one = 1;
	ht_put(checked, text, strlen(text) + 1, &one, sizeof(one));

	free(text);

	for (unsigned int i = 0; i < MAX_PEOPLE; i++) {
		if (visited[i] == NOT_VISITED && lg_has_edge(users_graph, user, i)) {
			visited[i] = IN_CLIQUE;

			if (count_clique(visited) > count_clique(max_visited)) {
				*max_clique = count_clique(visited);
				memcpy(max_visited, visited, sizeof(check_t) * MAX_PEOPLE);

				for (unsigned int j = 0; j < MAX_PEOPLE; j++)
					if (ll_get_size(lg_get_neighbours(users_graph, j)) <
							*max_clique &&
						visited[j] == NOT_VISITED) {
						if (*max_clique > 2) {
							return;
						}
					}
			}

			if (valid_clique(users_graph, visited))
				backtracking(users_graph, i, visited, max_clique, max_visited,
							 checked);

			visited[i] = NOT_VISITED;
		}
	}
}

void clique(list_graph_t *users_graph, unsigned int user)
{
	check_t visited[MAX_PEOPLE];

	for (unsigned int i = 0; i < MAX_PEOPLE; i++)
		if (lg_has_edge(users_graph, user, i))
			visited[i] = NOT_VISITED;
		else
			visited[i] = NOT_IN_CLIQUE;

	visited[user] = IN_CLIQUE;

	unsigned int max_clique = 0;
	check_t max_visited[MAX_PEOPLE];
	for (unsigned int i = 0; i < MAX_PEOPLE; i++)
		max_visited[i] = NOT_VISITED;

	visited[user] = IN_CLIQUE;

	hashtable_t *ht = ht_create(MAX_PEOPLE, hash_string, compare_strings);

	backtracking(users_graph, user, visited, &max_clique, max_visited, ht);

	printf("The closest friend group of %s is:\n", get_user_name(user));

	for (unsigned int i = 0; i < MAX_PEOPLE; i++) {
		if (max_visited[i] == IN_CLIQUE)
			printf("%s\n", get_user_name(i));
	}

	ht_free(ht);
}
