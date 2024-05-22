#ifndef FEED_H
#define FEED_H

#include "friends.h"
#include "posts.h"

/**
 * Function that handles the calling of every command from task 3
 *
 * Please add any necessary parameters to the functions
*/
void handle_input_feed(char *input, list_graph_t *users_graph,
					   ll_list_t *posts);

void show_feed(list_graph_t *users_graph, ll_list_t *posts,
			   char *name, int feed_size);

void traverse_reposts(tr_node_t *node, list_graph_t *users,
					  int *friends, unsigned int user_id);

void friends_repost(list_graph_t *users_graph, ll_list_t *posts,
					char *name, unsigned int post_id);

#endif // FEED_H
