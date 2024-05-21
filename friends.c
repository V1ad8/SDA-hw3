#include "friends.h"
#include "users.h"

void handle_input_friends(char *input, list_graph_t *users_graph)
{
	char *commands = strdup(input);
	char *cmd = strtok(commands, "\n ");

	if (!cmd)
		return;

	if (!strcmp(cmd, "add")) {
		char *name1 = strtok(NULL, "\n ");
		char *name2 = strtok(NULL, "\n ");
		add_friend(users_graph, name1, name2);
	} else if (!strcmp(cmd, "remove")) {
		char *name1 = strtok(NULL, "\n ");
		char *name2 = strtok(NULL, "\n ");
		remove_friend(users_graph, name1, name2);
	} else if (!strcmp(cmd, "suggestions")) {
		char *name = strtok(NULL, "\n ");
		suggestions(users_graph, name);
	} else if (!strcmp(cmd, "distance")) {
		char *name1 = strtok(NULL, "\n ");
		char *name2 = strtok(NULL, "\n ");
		friend_distance(users_graph, name1, name2);
	} else if (!strcmp(cmd, "common")) {
		char *name1 = strtok(NULL, "\n ");
		char *name2 = strtok(NULL, "\n ");
		common_friends(users_graph, name1, name2);
	} else if (!strcmp(cmd, "friends")) {
		char *name = strtok(NULL, "\n ");
		int nr_fr = nr_of_friends(users_graph, name);
		printf("%s has %d friends\n", name, nr_fr);
	} else if (!strcmp(cmd, "popular")) {
		char *name = strtok(NULL, "\n ");
		popular(users_graph, name);
	}

	free(commands);
}

void add_friend(list_graph_t *users, char *name1, char *name2)
{
	// get the id's of both people
	int name1_id = get_user_id(name1);
	int name2_id = get_user_id(name2);

	// add an edge from each friend to the other, because the graph is oriented
	lg_add_edge(users, name1_id, name2_id);
	lg_add_edge(users, name2_id, name1_id);

	printf("Added connection %s - %s\n", name1, name2);
}

void remove_friend(list_graph_t *users, char *name1, char *name2)
{
	// get the id's of both people
	int name1_id = get_user_id(name1);
	int name2_id = get_user_id(name2);

	// add the edge from each friend to the other, because the graph is oriented
	lg_remove_edge(users, name1_id, name2_id);
	lg_remove_edge(users, name2_id, name1_id);

	printf("Removed connection %s - %s\n", name1, name2);
}

int nr_of_friends(list_graph_t *users, char *name)
{
	int id = get_user_id(name);
	return users->neighbors[id]->size;
}

void common_friends(list_graph_t *users, char *name1, char *name2)
{
	int id1 = get_user_id(name1);
	int id2 = get_user_id(name2);

	// frequency array for both users
	int *common = calloc(users->nodes, sizeof(int));
	DIE(!common, "common friends calloc failed");

	// check if there are common friends
	int exists = 0;

	// store the friends of the first users in the array
	for (ll_node_t *node = users->neighbors[id1]->head; node; node = node->next)
		common[*(int *)node->data]++;

	// store the friends of the second user in the array
	for (ll_node_t *node = users->neighbors[id2]->head; node;
		 node = node->next) {
		common[*(int *)node->data]++;
		if (common[*(int *)node->data] == 2)
			exists = 1;
	}

	if (!exists) {
		printf("No common friends for %s and %s\n", name1, name2);
		free(common);
		return;
	}

	// print the common friends of the users in order of their id's
	printf("The common friends between %s and %s are:\n", name1, name2);
	for (int i = 0; i < users->nodes; i++) {
		if (common[i] == 2) {
			char *name = get_user_name(i);
			printf("%s\n", name);
		}
	}
	free(common);
}

void suggestions(list_graph_t *users, char *name)
{
	int id = get_user_id(name);

	// frequency array to avoid writing names twice
	int *found = calloc(users->nodes, sizeof(int));
	DIE(!found, "suggestions calloc");

	// check if there are suggestions
	int exists = 0;

	for (ll_node_t *node = users->neighbors[id]->head; node;
		 node = node->next) {
		int friend_id = *(int *)node->data;
		// go trough the friends of each friend of the given user
		for (ll_node_t *frnd = users->neighbors[friend_id]->head; frnd;
			 frnd = frnd->next) {
			int fof_id = *(int *)frnd->data; // friend of friend's id
			if (fof_id != id && !lg_has_edge(users, id, fof_id)) {
				found[fof_id]++;
				exists = 1;
			}
		}
	}

	if (!exists) {
		printf("There are no suggestions for %s\n", name);
		free(found);
		return;
	}
	
	printf("Suggestions for %s:\n", name);
	for (int i = 0; i < users->nodes; i++) {
		if (found[i]) {
			char *suggested_name = get_user_name(i);
			printf("%s\n", suggested_name);
		}
	}
	free(found);
}

void popular(list_graph_t *users, char *name)
{
	int id = get_user_id(name);
	unsigned int max_friends = users->neighbors[id]->size;
	int popular_id = id; // id of the most popular friend

	for (ll_node_t *node = users->neighbors[id]->head; node;
		 node = node->next) {
		int fr_id = *(int *)node->data;
		if (users->neighbors[fr_id]->size > max_friends) {
			max_friends = users->neighbors[fr_id]->size;
			popular_id = fr_id;
		} else if (users->neighbors[fr_id]->size == max_friends) {
			if (popular_id != id && fr_id < popular_id)
				popular_id = fr_id; // choose the friend with the lower id
		}
	}

	if (popular_id == id) {
		printf("%s is the most popular\n", name);
	} else {
		char *popular_name = get_user_name(popular_id);
		printf("%s is the most popular friend of %s\n", popular_name, name);
	}
}

// Breadth First Search traversal of a graph that saves the distance
// from a given node to every othe node in the graph
void bfs(list_graph_t *lg, int start, int *visited, int *dist)
{
	visited[start] = 1;
	dist[start] = 0;
	queue_t *q = q_create(sizeof(int), lg->nodes);
	q_enqueue(q, &start);

	while (!q_is_empty(q)) {
		int v = *(int *)q_front(q);
		q_dequeue(q);
		for (ll_node_t *node = lg->neighbors[v]->head; node; node = node->next)
			if (!visited[*(int *)node->data]) {
				visited[*(int *)node->data] = 1;
				dist[*(int *)node->data] = dist[v] + 1;
				q_enqueue(q, node->data);
			}
	}

	q_free(q);
}

void friend_distance(list_graph_t *users, char *name1, char *name2)
{
	int *visited = calloc(users->nodes, sizeof(int));
	DIE(!visited, "calloc() failed\n");

	int *dist = calloc(users->nodes, sizeof(int));
	DIE(!dist, "calloc() failed\n");

	for (int i = 0; i < users->nodes; i++)
		dist[i] = INF;

	int id1 = get_user_id(name1);
	int id2 = get_user_id(name2);

	bfs(users, id1, visited, dist);

	if (dist[id2] == INF)
		printf("There is no way to get from %s to %s\n", name1, name2);
	else
		printf("The distance between %s - %s is %d\n", name1, name2, dist[id2]);

	free(visited);
	free(dist);
}
