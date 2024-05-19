
#include "../users.h"
#include "specific.h"

ll_node_t *ll_add_nth_post(ll_list_t *list, unsigned int n, post_t *new_data)
{
	ll_node_t *new_node, *prev_node;

	if (!list)
		return NULL;

	if (n > list->size)
		return NULL;

	new_node = calloc(1, sizeof(ll_node_t));
	DIE(!new_node, "malloc node");

	new_node->data = malloc(sizeof(post_t));
	DIE(!new_node->data, "malloc data");

	if (new_data->title) {
		((post_t *)new_node->data)->events = malloc(sizeof(tree_t));
		memcpy(((post_t *)new_node->data)->events, new_data->events,
			   sizeof(tree_t));

		((post_t *)new_node->data)->title = strdup(new_data->title);
	}

	((post_t *)new_node->data)->id = new_data->id;
	((post_t *)new_node->data)->user_id = new_data->user_id;

	if (n == 0) {
		new_node->next = list->head;
		if (list->head)
			list->head->prev = new_node;
		list->head = new_node;
		if (!list->tail)
			list->tail = new_node;
	} else if (n == list->size) {
		new_node->prev = list->tail;
		list->tail->next = new_node;
		list->tail = new_node;
	} else {
		prev_node = ll_get_nth_node(list, n - 1);
		new_node->prev = prev_node;
		new_node->next = prev_node->next;
		prev_node->next->prev = new_node;
		prev_node->next = new_node;
	}

	++list->size;

	return new_node;
}

void tr_insert_post(tree_t *tree, post_t *data, tr_node_t *parent)
{
	if (!parent) {
		tree->root = malloc(sizeof(tr_node_t));
		tree->root->data = calloc(1, sizeof(post_t));

		((post_t *)tree->root->data)->events =
			(tree_t *)strdup((char *)data->events);
		((post_t *)tree->root->data)->title = strdup(data->title);

		((post_t *)tree->root->data)->id = data->id;
		((post_t *)tree->root->data)->user_id = data->user_id;

		tree->root->kid = ll_create(sizeof(tr_node_t));
		tree->root->par = NULL;
		return;
	}

	tr_node_t *node = malloc(sizeof(tr_node_t));
	DIE(node == NULL, "malloc");

	node->data = calloc(1, sizeof(post_t));
	DIE(!node->data, "malloc data");

	((post_t *)node->data)->events = NULL;
	((post_t *)node->data)->title = NULL;

	((post_t *)node->data)->id = data->id;
	((post_t *)node->data)->user_id = data->user_id;

	node->kid = ll_create(sizeof(tr_node_t));
	node->par = parent;

	ll_add_nth_node(parent->kid, ll_get_size(parent->kid), node);

	// TO DO: free(node->data); uncommenting this breaks the program
	// free(node->data);
	free(node);
}

void tr_remove_hard(tr_node_t *node)
{
	for (ll_node_t *curr = node->kid->head; curr; curr = curr->next) {
		tr_remove_hard(curr->data);
	}

	ll_free(&(node->kid));
	free(((post_t *)node->data)->events);
	free(((post_t *)node->data)->title);

	if (!node->par) {
		free(node->data);
		free(node);
	}
}

void tr_destroy(tree_t **tree)
{
	tr_remove_hard((*tree)->root);
	free(*tree);
	*tree = NULL;
}

static tr_node_t *__get_post_by_id(tr_node_t *node, unsigned int post_id)
{
	if (!node)
		return NULL;

	if (((post_t *)node->data)->id == post_id)
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
	return __get_post_by_id(tree->root, post_id);
}

tr_node_t *lca(tree_t *tree, tr_node_t *node1, tr_node_t *node2)
{
	if (node1 == NULL || node2 == NULL) {
		return NULL;
	}

	unsigned int level1 = get_level(tree, node1);
	unsigned int level2 = get_level(tree, node2);

	for (; level1 > level2; level1--) {
		node1 = node1->par;
	}

	for (; level2 > level1; level2--) {
		node2 = node2->par;
	}

	while (((post_t *)node1->data)->id != ((post_t *)node2->data)->id) {
		node1 = node1->par;
		node2 = node2->par;
	}

	return node1;
}