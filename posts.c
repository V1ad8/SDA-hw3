#include "posts.h"

void handle_input_posts(char *input, ll_list_t *posts)
{
	// Duplicate the input string and get the command
	char *commands = strdup(input);
	char *cmd = strtok(commands, "\n ");
	DIE(!cmd, "No command found");

	// Initialize the variables for the command
	char *user;
	char *text;

	// Initialize the variables for the command
	unsigned int value1 = 0;
	unsigned int value2 = 0;
	unsigned int value3 = 0;

	// Save the sscanf return value for the style checker
	unsigned int trash = 0;

	if (!strcmp(cmd, "create")) {
		// Get the user and the text
		user = strtok(NULL, "\n ");
		text = read_text(input);

		// Create a new post
		create_post(posts, user, text);
	} else if (!strcmp(cmd, "repost")) {
		// Read the user and the values
		user = strtok(NULL, "\n ");
		trash = sscanf(commands + strlen(cmd) + strlen(user) + 2, "%d %d",
					   &value1, &value2);

		// Repost the post
		repost_post(posts, user, value1, value2);
	} else if (!strcmp(cmd, "common-repost")) {
		// Read the values
		trash = sscanf(commands + strlen(cmd) + 1, "%d %d %d", &value1, &value2,
					   &value3);

		// Find the common repost
		common_repost(posts, value1, value2, value3);
	} else if (!strcmp(cmd, "like")) {
		// Read the user and the values
		user = strtok(NULL, "\n ");
		trash = sscanf(commands + strlen(cmd) + strlen(user) + 2, "%d %d",
					   &value1, &value2);

		// Like the post
		like_post(posts, user, value1, value2);
	} else if (!strcmp(cmd, "ratio")) {
		// Read the value
		trash = sscanf(commands + strlen(cmd) + 1, "%d", &value1);

		// Get the ratio
		ratio(posts, value1);
	} else if (!strcmp(cmd, "delete")) {
		// Read the values
		trash = sscanf(commands + strlen(cmd) + 1, "%d %d", &value1, &value2);

		// Delete the post
		delete_post(posts, value1, value2);
	} else if (!strcmp(cmd, "get-likes")) {
		// Read the values
		trash = sscanf(commands + strlen(cmd) + 1, "%d %d", &value1, &value2);

		// Get the likes
		get_likes(posts, value1, value2);
	} else if (!strcmp(cmd, "get-reposts")) {
		// Read the values
		trash = sscanf(commands + strlen(cmd) + 1, "%d %d", &value1, &value2);

		// Get the reposts
		get_reposts(posts, value1, value2);
	}

	// Free the memory
	free(commands);

	// Use the trash variable for the style checker
	(void)trash;
}

post_t *create_post(ll_list_t *posts, char *user, char *title)
{
	// Give a unique id to watch post
	static unsigned int id = 1;

	// Create a new post
	post_t *post = malloc(sizeof(post_t));
	DIE(!post, "Malloc failed while creating post");

	// Fill the post's fields
	post->id = id;
	post->user_id = get_user_id(user);

	// Initialize the likes array
	for (unsigned int i = 0; i < MAX_USERS; i++)
		post->likes[i] = false;

	// If the title is not NULL, create a new post
	if (title) {
		// Copy the title
		post->title = strdup(title);
		DIE(!post->title, "Malloc failed while creating post");

		// Create a new tree for the post
		post->events = tr_create(sizeof(post_t));
		DIE(!post->events, "Malloc failed while creating post");

		// Insert the post into the tree
		tr_insert_post(post->events, post, NULL);

		// Print a message
		printf("Created \"%s\" for %s\n", title, user);

		// Add the post to the list of posts
		ll_node_t *new_node = ll_add_nth_post(posts, ll_get_size(posts), post);

		// Free the memory
		free(post->title);
		free(post->events);
		free(post);

		// Increment the id to keep it unique
		id++;

		// Return the post
		return new_node->data;
	}

	// If the title is NULL, create a new repost

	// Give the repost null title and events
	post->title = NULL;
	post->events = NULL;

	// Increment the id to keep it unique
	id++;

	// Return the repost
	return post;
}

void repost_post(ll_list_t *posts, char *user, unsigned int post_id,
				 unsigned int repost_id)
{
	// Create a new repost
	post_t *repost = create_post(posts, user, NULL);

	// Get the post to repost
	post_t *post = get_post(posts, post_id);
	DIE(!post, "Post not found");

	// Get the parent node for the repost
	tr_node_t *node = post->events->root;
	if (repost_id)
		node = get_post_by_id(post->events, repost_id);

	// Insert the repost into the tree
	tr_insert_post(post->events, repost, node);

	// Print a message
	printf("Created repost #%d for %s\n", repost->id, user);

	// Free the repost
	free(repost);
}

void common_repost(ll_list_t *posts, unsigned int post_id,
				   unsigned int repost_id1, unsigned int repost_id2)
{
	// Get the post to search in
	post_t *post = get_post(posts, post_id);

	// Get the nodes for the reposts
	tr_node_t *node1 = get_post_by_id(post->events, repost_id1);
	tr_node_t *node2 = get_post_by_id(post->events, repost_id2);
	DIE(!node1 || !node2, "Repost not found");

	// Get the lowest common ancestor
	tr_node_t *lca_node = lca(post->events, node1, node2);

	// Print the message
	printf("The first common repost of %d and %d is %d\n", repost_id1,
		   repost_id2, ((post_t *)lca_node->data)->id);
}

void like_post(ll_list_t *posts, char *user, unsigned int post_id,
			   unsigned int repost_id)
{
	// Get the post to like
	post_t *post = get_post(posts, post_id);
	DIE(!post, "Post not found");

	// Get the node for the repost to like
	tr_node_t *node = post->events->root;
	if (repost_id)
		node = get_post_by_id(post->events, repost_id);

	// Print the message piece by piece
	printf("User %s ", user);

	// Like the post
	((post_t *)node->data)->likes[get_user_id(user)] =
		!((post_t *)node->data)->likes[get_user_id(user)];

	// Check if the post was liked or unliked
	if (!((post_t *)node->data)->likes[get_user_id(user)])
		printf("un");

	// Print the message piece by piece
	printf("liked ");

	// Check if the post is a repost
	if (repost_id)
		printf("re");

	// Print the message piece by piece
	printf("post \"%s\"\n", post->title);
}

void ratio(ll_list_t *posts, unsigned int post_id)
{
	// Get the post to search in
	post_t *post = get_post(posts, post_id);

	// Get the number of likes for the original post
	unsigned int op_likes = count_likes(post->events->root->data);
	unsigned int likes = op_likes;

	// Get the highest rated post
	tr_node_t *node = get_top_post(post->events->root, &likes);

	// Print the message
	if (op_likes == likes) {
		printf("The original post is the highest rated\n");
	} else {
		printf("Post %d got ratio'd by repost %d\n", post_id,
			   ((post_t *)node->data)->id);
	}
}

void delete_post(ll_list_t *posts, unsigned int post_id, unsigned int repost_id)
{
	// Get the post to delete
	post_t *post = get_post(posts, post_id);
	DIE(!post, "Post not found");

	// Get the node for the repost to delete
	tr_node_t *node = post->events->root;
	if (repost_id)
		node = get_post_by_id(post->events, repost_id);

	// Save the removed node
	ll_node_t *removed;

	// Check if the post is a repost
	if (repost_id) {
		// Remove the repost from the parent's children
		for (ll_node_t *curr = node->par->kid->head; curr; curr = curr->next) {
			if (((post_t *)((tr_node_t *)curr->data)->data)->id ==
				((post_t *)node->data)->id) {
				removed = ll_remove_node(node->par->kid, curr);
				break;
			}
		}

		// Print the repost message
		printf("Deleted repost #%d of post \"%s\"\n",
			   ((post_t *)node->data)->id, post->title);
	} else {
		// Remove the post from the list of posts
		for (ll_node_t *curr = posts->head; curr; curr = curr->next) {
			if (((post_t *)curr->data)->id == post_id) {
				removed = ll_remove_node(posts, curr);
				break;
			}
		}

		// Print the post message
		printf("Deleted post \"%s\"\n", post->title);
	}

	// Remove the post and its reposts from the tree
	tr_remove_hard(node);

	// Free the memory
	free(removed->data);
	free(removed);
}

void get_likes(ll_list_t *posts, unsigned int post_id, unsigned int repost_id)
{
	// Get the post to search in
	post_t *post = get_post(posts, post_id);
	DIE(!post, "Post not found");

	// Get the node for the repost to search in
	tr_node_t *node = post->events->root;
	if (repost_id)
		node = get_post_by_id(post->events, repost_id);

	// Get the number of likes
	unsigned int likes = count_likes(node->data);

	// Print the message
	if (!repost_id)
		printf("Post \"%s\" has %d likes\n", post->title, likes);
	else
		printf("Repost #%d has %d likes\n", ((post_t *)node->data)->id, likes);
}

void get_reposts(ll_list_t *posts, unsigned int post_id, unsigned int repost_id)
{
	// Get the post to search in
	post_t *post = get_post(posts, post_id);
	DIE(!post, "Post not found");

	// Get the node for the repost to search in
	tr_node_t *node = post->events->root;
	if (repost_id)
		node = get_post_by_id(post->events, repost_id);

	// Check if the post is a repost and print the message
	if (!repost_id)
		printf("\"%s\" - Post by %s\n", post->title,
			   get_user_name(post->user_id));
	else
		printf("Repost #%d by %s\n", ((post_t *)node->data)->id,
			   get_user_name(((post_t *)node->data)->user_id));

	// Print the reposts recursively
	for (ll_node_t *curr = node->kid->head; curr; curr = curr->next)
		__get_reposts(curr->data);
}
