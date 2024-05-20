#include "posts.h"
#include "posts_add.h"

char *read_text(char *input)
{
    // Get the start and the end of the string
	char *start = strchr(input, '\"');
	char *end = strrchr(input, '\"');

    // Check for null string
	if (!start || !end)
		return NULL;

    // Place the string end over the last character (")
	*end = '\0';

    // Skip the first character (")
	return start + 1;
}

post_t *get_post(ll_list_t *posts, unsigned int post_id)
{
    // Check for the searched id and return the post
	for (ll_node_t *curr = posts->head; curr != NULL; curr = curr->next) {
		if (((post_t *)curr->data)->id == post_id) {
			return curr->data;
		}
	}

    // Return NULL if the post is not found
	return NULL;
}

unsigned int number_of_digits(unsigned int number)
{
    // Initialize the number of digits
	unsigned int digits = 0;

    // Count the number of digits
	while (number) {
		number /= 10;
		digits++;
	}

    // Return the number of digits
	return digits;
}

void __get_reposts(tr_node_t *node)
{
    // Get the post from the node
	post_t *post = node->data;

    // Check for null post
	if (!post) {
		return;
	}

    // Print the repost information
	printf("Repost #%d by %s\n", post->id, get_user_name(post->user_id));

    // Call the function recursively for the children of the post
	for (ll_node_t *curr = node->kid->head; curr; curr = curr->next) {
		__get_reposts(curr->data);
	}
}

unsigned int count_likes(post_t *post)
{
    // Initialize the number of likes
	unsigned int likes = 0;

    // Count the number of likes
	for (unsigned int i = 0; i < MAX_USERS; i++) {
		if (post->likes[i]) {
			likes++;
		}
	}

    // Return the number of likes
	return likes;
}

tr_node_t *get_top_post(tr_node_t *node, unsigned int *top_likes)
{
    // Check for null node
	if (!node)
		return NULL;

    // Get the number of likes for the current post
	unsigned int likes = count_likes(node->data);

    // Update the top likes if necessary
	if (likes > *top_likes) {
		*top_likes = likes;
	}

    // Call the function recursively for the children of the post
	for (ll_node_t *curr = node->kid->head; curr; curr = curr->next) {
		tr_node_t *found = get_top_post(curr->data, top_likes);
		if (found)
			return found;
	}

    // Return the post with the most likes
	if (likes == *top_likes) {
		return node;
	}

    // Return NULL if the post is not found
	return NULL;
}
