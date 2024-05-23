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
void insert_sorted(ll_list_t *profile_list, void *data,
				   ll_node_t *(*create_data)(const void *, unsigned int data_size), char *title);
void look_for_reposts(ll_list_t *profile_list, tr_node_t *node,
					  unsigned int user_id, char *title);
void show_profile(ll_list_t *posts, char *name);
#endif // FEED_H
