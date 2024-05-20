/**
 * The entrypoint of the homework. Every initialization must be done here
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "users.h"
#include "friends.h"
#include "posts.h"
#include "feed.h"

/**
 * Initializez every task based on which task we are running
*/
void init_tasks(void)
{
#ifdef TASK_1

#endif

#ifdef TASK_2

#endif

#ifdef TASK_3

#endif
}

/**
 * Entrypoint of the program, compiled with different defines for each task
*/
int main(void)
{
	init_users();

	init_tasks();

	list_graph_t *users_graph = lg_create(MAX_PEOPLE);
	ll_list_t *posts = ll_create(sizeof(post_t));

	posts->head = NULL;

	char *input = (char *)malloc(MAX_COMMAND_LEN);
	while (1) {
		// If fgets returns null, we reached EOF
		if (!fgets(input, MAX_COMMAND_LEN, stdin))
			break;

#ifdef TASK_1
		handle_input_friends(input, users_graph);
#endif

#ifdef TASK_2
		handle_input_posts(input, posts);
#endif

#ifdef TASK_3
		handle_input_feed(input);
#endif
	}

	free_users();
	free(input);
	lg_free(users_graph);

	for (ll_node_t *node = posts->head; node; node = node->next) {
		post_t *post = (post_t *)node->data;
		free(post->title);
		tr_destroy(&post->events);
	}

	ll_free(&posts);
	return 0;
}
