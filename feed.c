#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "feed.h"
#include "users.h"
#include "friends.h"
#include "posts.h"

void handle_input_feed(char *input, list_graph_t *users_graph,
					   ll_list_t *posts)
{
	char *commands = strdup(input);
	char *cmd = strtok(commands, "\n ");

	if (!cmd)
		return;

	if (!strcmp(cmd, "feed")) {
		char *name = strtok(NULL, "\n ");
		char *size = strtok(NULL, "\n ");
		int feed_size = atoi(size);
		show_feed(users_graph, posts, name, feed_size);
	} else if (!strcmp(cmd, "view-profile")) {
		(void)cmd;
	} else if (!strcmp(cmd, "friends-repost")) {
		char *name = strtok(NULL, "\n ");
		char *id = strtok(NULL, "\n ");
		unsigned int post_id = atoi(id);
		friends_repost(users_graph, posts, name, post_id);
	} else if (!strcmp(cmd, "common-groups")) {
		(void)cmd;
		// TODO: Add function
	}

	free(commands);
}

void show_feed(list_graph_t *users_graph, ll_list_t *posts,
			   char *name, int feed_size)
{
	// get the given user's id
	unsigned int id = get_user_id(name);

	// go trough the list of posts backwards to get the most recent ones
	for (ll_node_t *post = posts->tail; post && feed_size; post = post->prev) {
		post_t *curr_post = (post_t *)post->data;

		// check if the current post is made by the given user or a friend
		if (curr_post->user_id == id ||
			lg_has_edge(users_graph, id, curr_post->user_id)) {
			printf("%s: \"%s\"\n", get_user_name(curr_post->user_id),
				   curr_post->title);

			// decrease the number of posts left to be shown
			feed_size--;
		}
	}
}

void traverse_reposts(tr_node_t *node, list_graph_t *users,
					  int *friends, unsigned int user_id)
{
	// check if the node in NULL
	if (!node)
		return;

	// check if the current repost is made by a friend
	if (lg_has_edge(users, ((post_t *)node->data)->user_id, user_id))
		// mark the friend in the frequency array
		friends[((post_t *)node->data)->user_id]++;

	// go trough the children of the current node recursively
	for (ll_node_t *curr = node->kid->head; curr; curr = curr->next)
		traverse_reposts(curr->data, users, friends, user_id);
}

void friends_repost(list_graph_t *users_graph, ll_list_t *posts,
					char *name, unsigned int post_id)
{
	// get the post with the given id
	post_t *post = get_post(posts, post_id);

	// get the given user's id
	unsigned int id = get_user_id(name);

	// frequency array for the freinds that reposted
	int *friends = calloc(users_graph->nodes, sizeof(int));
	DIE(!friends, "calloc friends_repost");

	traverse_reposts(post->events->root, users_graph, friends, id);

	// printf("Friends that reposted:\n");
	for (int i = 0; i < users_graph->nodes; i++)
		if (friends[i])
			printf("%s\n", get_user_name(i));

	free(friends);
}
