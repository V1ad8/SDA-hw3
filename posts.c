#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "users.h"
#include "posts.h"

void handle_input_posts(char *input, ll_list_t *posts)
{
	char *commands = strdup(input);
	char *cmd = strtok(commands, "\n ");

	if (!cmd)
		return;

	char *user;
	char *text;

	if (!strcmp(cmd, "create")) {
		user = strtok(NULL, "\n ");
		text = read_text(input);

		create_post(posts, user, text);

	} else if (!strcmp(cmd, "repost")) {
		user = strtok(NULL, "\n ");
		text = commands + strlen(cmd) + strlen(user) + 2;

		repost_post(posts, user, text);
	} else if (!strcmp(cmd, "common-repost")) {
		text = commands + strlen(cmd) + 1;

		common_repost(posts, text);
	} else if (!strcmp(cmd, "like")) {
		user = strtok(NULL, "\n ");
		text = strtok(NULL, "\n ");

		like_post(posts, user, text);
	} else if (!strcmp(cmd, "ratio")) {
		(void)cmd;
		// TODO: Add function
	} else if (!strcmp(cmd, "delete")) {
		(void)cmd;
		// TODO: Add function
	} else if (!strcmp(cmd, "get-likes")) {
		text = commands + strlen(cmd) + 1;

		get_likes(posts, text);
	} else if (!strcmp(cmd, "get-reposts")) {
		text = strtok(NULL, "\n ");

		get_reposts(posts, text);
	}

	free(commands);
}

char *read_text(char *input)
{
	char *start = strchr(input, '\"');
	char *end = strrchr(input, '\"');

	if (!start || !end)
		return NULL;

	*end = '\0';

	return start + 1;
}

post_t *create_post(ll_list_t *posts, char *user, char *text)
{
	static unsigned int id = 0;
	id++;

	post_t *post = malloc(sizeof(post_t));
	DIE(!post, "Malloc failed while creating post");

	post->id = id;
	post->user_id = get_user_id(user);

	for (unsigned int i = 0; i < MAX_USERS; i++) {
		post->likes[i] = false;
	}

	if (text) {
		post->title = strdup(text);
		post->events = tr_create(sizeof(post_t));

		tr_insert_post(post->events, post, NULL);

		printf("Created \"%s\" for %s\n", text, user);

		ll_node_t *new_node = ll_add_nth_post(posts, ll_get_size(posts), post);

		free(post->title);
		free(post->events);
		free(post);

		return new_node->data;
	}

	post->title = NULL;
	post->events = NULL;

	return post;
}

post_t *get_post(ll_list_t *posts, unsigned int post_id)
{
	for (ll_node_t *curr = posts->head; curr != NULL; curr = curr->next) {
		if (((post_t *)curr->data)->id == post_id) {
			return curr->data;
		}
	}

	return NULL;
}

tr_node_t *get_post_from_kid(tr_node_t *node, unsigned int post_id)
{
	if (node == NULL || ll_get_size(node->kid) == 0) {
		return NULL;
	}

	for (ll_node_t *curr = node->kid->head; curr; curr = curr->next) {
		if (((post_t *)((tr_node_t *)curr->data)->data)->id == post_id) {
			return curr->data;
		}
	}

	return NULL;
}

unsigned int number_of_digits(unsigned int number)
{
	unsigned int digits = 0;

	while (number) {
		number /= 10;
		digits++;
	}

	return digits;
}

void repost_post(ll_list_t *posts, char *user, char *text)
{
	post_t *repost = create_post(posts, user, NULL);

	unsigned int post_id;
	sscanf(text, "%d", &post_id);
	text += number_of_digits(post_id);

	post_t *post = get_post(posts, post_id);

	if (!post) {
		return;
	}

	unsigned int repost_id = 0;
	tr_node_t *node = post->events->root;
	tr_node_t *tmp = NULL;

	if (sscanf(text, "%d", &repost_id)) {
		tmp = get_post_by_id(post->events, repost_id);

		if (tmp) {
			node = tmp;
		}
	}

	tr_insert_post(post->events, repost, node);

	repost_id = repost->id;

	free(repost);

	printf("Created repost #%d for %s\n", repost_id, user);
}

void like_post(ll_list_t *posts, char *user, char *text)
{
	unsigned int post_id;
	sscanf(text, "%d", &post_id);
	text += number_of_digits(post_id) + 1;

	post_t *post = get_post(posts, post_id);

	if (!post) {
		return;
	}

	unsigned int repost_id = 0;
	tr_node_t *node = post->events->root;
	tr_node_t *tmp = NULL;

	if (sscanf(text, "%d", &repost_id)) {
		tmp = get_post_by_id(post->events, repost_id);

		if (tmp) {
			node = tmp;
		}
	}

	printf("User %s ", user);

	((post_t *)node->data)->likes[get_user_id(user)] =
		!((post_t *)node->data)->likes[get_user_id(user)];

	if (!((post_t *)node->data)->likes[get_user_id(user)]) {
		printf("un");
	}

	printf("liked ");

	if (repost_id)
		printf("repost \"%s\"\n", post->title);
	else
		printf("post \"%s\"\n", ((post_t *)node->data)->title);
}

void __get_reposts(tr_node_t *node)
{
	post_t *post = node->data;

	if (!post) {
		return;
	}

	printf("Repost #%d by %s\n", post->id, get_user_name(post->user_id));

	for (ll_node_t *curr = node->kid->head; curr; curr = curr->next) {
		__get_reposts(curr->data);
	}
}

void get_reposts(ll_list_t *posts, char *text)
{
	unsigned int post_id;
	sscanf(text, "%d", &post_id);
	text += number_of_digits(post_id);

	post_t *post = get_post(posts, post_id);

	if (!post) {
		return;
	}

	unsigned int repost_id = 0;
	tr_node_t *node = post->events->root;
	tr_node_t *tmp = NULL;

	if (sscanf(text, "%d", &repost_id)) {
		tmp = get_post_by_id(post->events, repost_id);

		if (tmp) {
			node = tmp;
		}
	}

	printf("\"%s\" - Post by %s\n", post->title, get_user_name(post->user_id));

	for (ll_node_t *curr = node->kid->head; curr; curr = curr->next) {
		__get_reposts(curr->data);
	}
}

void common_repost(ll_list_t *posts, char *text)
{
	unsigned int post_id;
	unsigned int repost_id1;
	unsigned int repost_id2;
	sscanf(text, "%d %d %d", &post_id, &repost_id1, &repost_id2);

	post_t *post = get_post(posts, post_id);

	tr_node_t *node1 = get_post_by_id(post->events, repost_id1);
	tr_node_t *node2 = get_post_by_id(post->events, repost_id2);

	if (!node1 || !node2) {
		return;
	}

	tr_node_t *lca_node = lca(post->events, node1, node2);

	printf("The first common repost of %d and %d is %d\n", repost_id1,
		   repost_id2, ((post_t *)lca_node->data)->id);
}

void get_likes(ll_list_t *posts, char *text)
{
	unsigned int post_id;
	sscanf(text, "%d", &post_id);
	text += number_of_digits(post_id);

	post_t *post = get_post(posts, post_id);

	if (!post) {
		return;
	}

	unsigned int repost_id = 0;
	tr_node_t *node = post->events->root;
	tr_node_t *tmp = NULL;

	if (sscanf(text, "%d", &repost_id)) {
		tmp = get_post_by_id(post->events, repost_id);

		if (tmp) {
			node = tmp;
		}
	}

	unsigned int likes = 0;

	for (unsigned int i = 0; i < MAX_USERS; i++) {
		if (((post_t *)node->data)->likes[i]) {
			likes++;
		}
	}

	if (!repost_id)
		printf("Post \"%s\" has %d likes\n", post->title, likes);
	else
		printf("Repost #%d has %d likes\n", ((post_t *)node->data)->id, likes);
}
