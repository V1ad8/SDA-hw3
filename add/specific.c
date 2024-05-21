
#include "../users.h"
#include "specific.h"

ll_node_t *ll_create_post(const void *new_data, unsigned int data_size)
{
	(void)data_size;

	ll_node_t *node = calloc(1, sizeof(ll_node_t));
	DIE(!node, "malloc node");

	node->data = malloc(sizeof(post_t));
	DIE(!node->data, "malloc data");

	if (((post_t *)new_data)->title) {
		((post_t *)node->data)->events = malloc(sizeof(tree_t));
		memcpy(((post_t *)node->data)->events, ((post_t *)new_data)->events,
			   sizeof(tree_t));

		((post_t *)node->data)->title = strdup(((post_t *)new_data)->title);
	}

	((post_t *)node->data)->id = ((post_t *)new_data)->id;
	((post_t *)node->data)->user_id = ((post_t *)new_data)->user_id;

	return node;
}

ll_node_t *ll_create_tree_post(const void *new_data, unsigned int data_size)
{
	(void)data_size;

	ll_node_t *node = calloc(1, sizeof(ll_node_t));
	DIE(!node, "malloc node");

	node->data = malloc(sizeof(tr_node_t));
	DIE(!node->data, "malloc data");

	((tr_node_t *)node->data)->kid = ((tr_node_t *)new_data)->kid;
	((tr_node_t *)node->data)->par = ((tr_node_t *)new_data)->par;

	((tr_node_t *)node->data)->data = calloc(1, sizeof(post_t));
	DIE(!((tr_node_t *)node->data)->data, "malloc data");

	if (((post_t *)((tr_node_t *)new_data)->data)->title) {
		((post_t *)((tr_node_t *)node->data)->data)->events = (tree_t *)strdup(
			(char *)((post_t *)((tr_node_t *)new_data)->data)->events);
		((post_t *)((tr_node_t *)node->data)->data)->title =
			strdup(((post_t *)((tr_node_t *)new_data)->data)->title);
	}

	((post_t *)((tr_node_t *)node->data)->data)->id =
		((post_t *)((tr_node_t *)new_data)->data)->id;
	((post_t *)((tr_node_t *)node->data)->data)->user_id =
		((post_t *)((tr_node_t *)new_data)->data)->user_id;

	return node;
}

tr_node_t *tr_create_post(const void *data, unsigned int data_size)
{
	(void)data_size;

	tr_node_t *node = malloc(sizeof(tr_node_t));
	DIE(!node, "malloc");

	node->data = calloc(1, sizeof(post_t));
	DIE(!node->data, "malloc data");

	if (((post_t *)data)->title) {
		((post_t *)node->data)->events =
			(tree_t *)strdup((char *)((post_t *)data)->events);
		((post_t *)node->data)->title = strdup(((post_t *)data)->title);
	}

	((post_t *)node->data)->id = ((post_t *)data)->id;
	((post_t *)node->data)->user_id = ((post_t *)data)->user_id;

	node->kid = ll_create(sizeof(tr_node_t));
	node->par = NULL;

	return node;
}

void free_post(void *data)
{
	free(((post_t *)data)->events);
	free(((post_t *)data)->title);
	free(data);
}

int compare_post(void *data1, void *data2, size_t data_size)
{
	(void)data_size;
	return (((post_t *)data1)->id - *(unsigned int *)data2);
}

static tr_node_t *__get_post_by_id(tr_node_t *node, unsigned int post_id)
{
	if (!node || !post_id || !node->kid || !ll_get_size(node->kid))
		return NULL;

	if (compare_post(node->data, &post_id, sizeof(post_id)) == 0)
		return node;

	for (ll_node_t *curr = node->kid->head; curr; curr = curr->next) {
		tr_node_t *found = __get_post_by_id(curr->data, post_id);
		if (found)
			return found;
	}

	return NULL;
}

tr_node_t *get_post_by_id(tree_t *tree, unsigned int post_id)
{
	return tr_search(tree, &post_id, compare_post);
	return __get_post_by_id(tree->root, post_id);
}
