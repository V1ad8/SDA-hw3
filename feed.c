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
		char *name = strtok(NULL, "\n ");
		show_profile(posts, name);
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
	// check if the node is NULL
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

	// look trough all reposts
	traverse_reposts(post->events->root, users_graph, friends, id);

	// print every friend that reposted
	for (int i = 0; i < users_graph->nodes; i++)
		if (friends[i])
			printf("%s\n", get_user_name(i));

	// free the frequency array
	free(friends);
}

void insert_sorted(ll_list_t *profile_list, void *data, char *title)
{
	// create the node
	ll_node_t *new_node = calloc(1, sizeof(ll_node_t));
	DIE(!new_node, "malloc node");

	// create the node data
	new_node->data = calloc(1, sizeof(profile_post_t));
	DIE(!new_node->data, "malloc data");

	post_t *aux_post = (post_t *)data;

	// fill the id and title
	((profile_post_t *)new_node->data)->id = aux_post->id;
	((profile_post_t *)new_node->data)->title = title;

	// check if the post is a repost
	if (!aux_post->title)
		((profile_post_t *)new_node->data)->is_repost = true;
	else
		((profile_post_t *)new_node->data)->is_repost = false;

	// if it's the first element
	if (!profile_list->size) {
		profile_list->head = new_node;
		profile_list->tail = new_node;
		profile_list->size++;
		return;
	}

	// at the start of the list
	if (aux_post->id < ((profile_post_t *)profile_list->head->data)->id) {
		profile_list->head->prev = new_node;
		new_node->next = profile_list->head;
		profile_list->head = new_node;
		profile_list->size++;
		return;
	}

	// at the end of the list
	if (aux_post->id > ((profile_post_t *)profile_list->tail->data)->id) {
		profile_list->tail->next = new_node;
		new_node->prev = profile_list->tail;
		profile_list->tail = new_node;
		profile_list->size++;
		return;
	}

	// anywhere else in the list
	ll_node_t *curr = profile_list->head;
	while (((profile_post_t *)curr->data)->id < aux_post->id)
		curr = curr->next;
	new_node->next = curr;
	new_node->prev = curr->prev;
	curr->prev->next = new_node;
	curr->prev = new_node;
	profile_list->size++;
}

void look_for_reposts(ll_list_t *profile_list, tr_node_t *node,
					  unsigned int user_id, char *title)
{
	if (!node)
		return;

	// if the post belongs to the user add it to the profile list
	if (((post_t *)node->data)->user_id == user_id)
		insert_sorted(profile_list, node->data, title);

	// go trough the children of the current node recursively
	for (ll_node_t *curr = node->kid->head; curr; curr = curr->next)
		look_for_reposts(profile_list, curr->data, user_id, title);
}

void show_profile(ll_list_t *posts, char *name)
{
	// get the given user's id
	unsigned int id = get_user_id(name);

	// create a list that contains every post and repost of the given user
	ll_list_t *profile_list = ll_create(sizeof(profile_post_t));

	for (ll_node_t *post = posts->head; post; post = post->next) {
		post_t *curr_post = (post_t *)post->data;
		// look trough the current post if it's made by the given user or
		// if it has been reposted by the user
		look_for_reposts(profile_list, curr_post->events->root,
						 id, curr_post->title);
	}

	for (ll_node_t *post = profile_list->head; post; post = post->next) {
		profile_post_t *curr_post = (profile_post_t *)post->data;
		if (curr_post->is_repost)
			printf("Reposted: \"%s\"\n", curr_post->title);
		else
			printf("Posted: \"%s\"\n", curr_post->title);
	}

	// free the profile list
	ll_free(&profile_list);
}
