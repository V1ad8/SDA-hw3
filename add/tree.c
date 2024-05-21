#include "tree.h"

tree_t *tr_create(unsigned int data_size)
{
	tree_t *tree = malloc(sizeof(tree_t));
	tree->data_size = data_size;
	tree->root = NULL;
	return tree;
}

void tr_destroy(tree_t **tree, void (*free_data)(void *data))
{
	tr_remove_hard((*tree)->root, free_data);
	free(*tree);
	*tree = NULL;
}

static tr_node_t *__tr_search(tr_node_t *node, void *data,
							  unsigned int data_size,
							  int (*compare)(void *, void *, size_t))
{
	if (!node || !data)
		return NULL;

	if (compare(node->data, data, data_size) == 0)
		return node;

	for (ll_node_t *curr = node->kid->head; curr; curr = curr->next) {
		tr_node_t *found = __tr_search(curr->data, data, data_size, compare);
		if (found)
			return found;
	}

	return NULL;
}

tr_node_t *tr_search(tree_t *tree, void *data,
					 int (*compare)(void *, void *, size_t))
{
	return __tr_search(tree->root, data, tree->data_size, compare);
}

tr_node_t *tr_create_node(void *data, unsigned int data_size)
{
	tr_node_t *node = malloc(sizeof(tr_node_t));
	DIE(!node, "malloc");

	node->data = malloc(data_size);
	DIE(!node->data, "malloc");

	memcpy(node->data, data, data_size);
	node->kid = ll_create(sizeof(tr_node_t));
	node->par = NULL;

	return node;
}

void tr_insert(tree_t *tree, void *data, tr_node_t *parent,
			   tr_node_t *(*tr_create_data)(const void *,
											unsigned int data_size),
			   ll_node_t *(*ll_create_data)(const void *,
											unsigned int data_size))
{
	tr_node_t *node = tr_create_data(data, tree->data_size);
	node->par = parent;

	if (!parent) {
		tree->root = node;
		return;
	}

	ll_add_nth_node(parent->kid, ll_get_size(parent->kid), node,
					ll_create_data);

	free(node->data);
	free(node);
}

void tr_remove_soft(tr_node_t *node)
{
	if (!node)
		return;

	tr_node_t *parent = node->par;
	ll_node_t *curr = parent->kid->head;
	for (; curr; curr = curr->next) {
		if (curr->data == node)
			break;
	}

	DIE(!curr, "search");

	free(ll_remove_node(parent->kid, curr)->data);
	free(ll_remove_node(parent->kid, curr));

	for (ll_node_t *curr = node->kid->head; curr; curr = curr->next) {
		((tr_node_t *)curr->data)->par = parent;
		ll_add_nth_node(parent->kid, ll_get_size(parent->kid), curr,
						ll_create_node);
	}

	free(node->data);
	ll_free(&node->kid);
	free(node);
}

void tr_remove_hard(tr_node_t *node, void (*free_data)(void *data))
{
	for (ll_node_t *curr = node->kid->head; curr; curr = curr->next) {
		tr_remove_hard(curr->data, free_data);
	}

	ll_free(&(node->kid));
	free_data(node->data);

	if (!node->par) {
		free(node);
	}
}

static unsigned int __get_level(tr_node_t *root, tr_node_t *node,
								unsigned int level)
{
	if (!root || !node)
		return 0;

	if (root == node)
		return level;

	for (ll_node_t *curr = root->kid->head; curr; curr = curr->next) {
		unsigned int lev = __get_level(curr->data, node, level + 1);
		if (lev)
			return lev;
	}

	return 0;
}

unsigned int get_level(tree_t *tree, tr_node_t *node)
{
	return __get_level(tree->root, node, 1);
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

	while (node1 != node2) {
		node1 = node1->par;
		node2 = node2->par;
	}

	return node1;
}
