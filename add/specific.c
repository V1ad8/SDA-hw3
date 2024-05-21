
#include "specific.h"

ll_node_t *ll_create_post(const void *new_data, unsigned int data_size)
{
	// Unused parameter
	(void)data_size;

	// Allocate memory for the node
	ll_node_t *node = calloc(1, sizeof(ll_node_t));
	DIE(!node, "malloc node");

	// Allocate memory for the data of the node
	node->data = malloc(sizeof(post_t));
	DIE(!node->data, "malloc data");

	// Check if the post is a repost
	if (((post_t *)new_data)->title) {
		// Allocate memory for the events
		((post_t *)node->data)->events = malloc(sizeof(tree_t));
		DIE(!((post_t *)node->data)->events, "malloc events");

		// Copy the events and the title
		memcpy(((post_t *)node->data)->events, ((post_t *)new_data)->events,
			   sizeof(tree_t));
		((post_t *)node->data)->title = strdup(((post_t *)new_data)->title);
		DIE(!((post_t *)node->data)->title, "strdup title");
	}

	// Copy the id and the user_id
	((post_t *)node->data)->id = ((post_t *)new_data)->id;
	((post_t *)node->data)->user_id = ((post_t *)new_data)->user_id;

	// Return the node
	return node;
}

ll_node_t *ll_create_tree_post(const void *new_data, unsigned int data_size)
{
	// Unused parameter
	(void)data_size;

	// Allocate memory for the node
	ll_node_t *node = calloc(1, sizeof(ll_node_t));
	DIE(!node, "malloc node");

	// Allocate memory for the data of the node
	node->data = malloc(sizeof(tr_node_t));
	DIE(!node->data, "malloc data");

	// Copy the fields of the tree node
	((tr_node_t *)node->data)->kid = ((tr_node_t *)new_data)->kid;
	((tr_node_t *)node->data)->par = ((tr_node_t *)new_data)->par;

	// Allocate memory for the data of the tree node
	((tr_node_t *)node->data)->data = calloc(1, sizeof(post_t));
	DIE(!((tr_node_t *)node->data)->data, "malloc data");

	// Check if the post is a repost
	if (((post_t *)((tr_node_t *)new_data)->data)->title) {
		// Copy the events
		((post_t *)((tr_node_t *)node->data)->data)->events =
			(tree_t *)strdup((char *)((post_t *)((tr_node_t *)new_data)
			->data)->events);
		DIE(!((post_t *)((tr_node_t *)node->data)->data)->events,
			"strdup events");

		// Copy the title
		((post_t *)((tr_node_t *)node->data)->data)->title =
			strdup(((post_t *)((tr_node_t *)new_data)->data)->title);
		DIE(!((post_t *)((tr_node_t *)node->data)->data)->title,
			"strdup title");
	}

	// Copy the id and the user_id
	((post_t *)((tr_node_t *)node->data)->data)->id =
		((post_t *)((tr_node_t *)new_data)->data)->id;
	((post_t *)((tr_node_t *)node->data)->data)->user_id =
		((post_t *)((tr_node_t *)new_data)->data)->user_id;

	// Return the node
	return node;
}

tr_node_t *tr_create_post(const void *data, unsigned int data_size)
{
	// Unused parameter
	(void)data_size;

	// Allocate memory for the node
	tr_node_t *node = calloc(1, sizeof(tr_node_t));
	DIE(!node, "malloc");

	// Allocate memory for the data of the node
	node->data = calloc(1, sizeof(post_t));
	DIE(!node->data, "malloc data");

	// Check if the post is a repost
	if (((post_t *)data)->title) {
		// Copy the events
		((post_t *)node->data)->events =
			(tree_t *)strdup((char *)((post_t *)data)->events);
		DIE(!((post_t *)node->data)->events, "strdup events");

		// Copy the title
		((post_t *)node->data)->title = strdup(((post_t *)data)->title);
		DIE(!((post_t *)node->data)->title, "strdup title");
	}

	// Copy the id and the user_id
	((post_t *)node->data)->id = ((post_t *)data)->id;
	((post_t *)node->data)->user_id = ((post_t *)data)->user_id;

	// Initialize the children
	node->kid = ll_create(sizeof(tr_node_t));
	DIE(!node->kid, "ll_create");

	// Return the node
	return node;
}

void free_post(void *data)
{
	// Check if the data is NULL
	if (!data)
		return;

	// Free the title and the events
	free(((post_t *)data)->events);
	free(((post_t *)data)->title);

	// Free the data
	free(data);
}

int compare_post(void *post, void *id, size_t data_size)
{
	// Unused parameter
	(void)data_size;

	// Return the difference between the ids
	return (((post_t *)post)->id - *(unsigned int *)id);
}
