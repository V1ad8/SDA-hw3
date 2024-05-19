#include "tree.h"

tree_t *tr_create(unsigned int data_size)
{
	tree_t *tree = malloc(sizeof(tree_t));
	tree->data_size = data_size;
	tree->root = NULL;
	return tree;
}

static tr_node_t *__tr_search(tr_node_t *node, void *data,
							  unsigned int data_size)
{
	if (node == NULL || data == NULL || node->kid == NULL ||
		ll_get_size(node->kid) == 0) {
		return NULL;
	}

	if (memcmp(node->data, data, data_size) == 0) {
		return node;
	}

	for (ll_node_t *curr = node->kid->head; curr; curr = curr->next) {
		tr_node_t *found = __tr_search(curr->data, data, data_size);
		if (found != NULL) {
			return found;
		}
	}

	return NULL;
}

tr_node_t *tr_search(tree_t *tree, void *data)
{
	return __tr_search(tree->root, data, tree->data_size);
}

void tr_insert(tree_t *tree, void *data, tr_node_t *parent)
{
	tr_node_t *node = malloc(sizeof(tr_node_t));
	DIE(node == NULL, "malloc");

	node->data = malloc(tree->data_size);
	DIE(node->data == NULL, "malloc");

	memcpy(node->data, data, tree->data_size);
	node->kid = ll_create(sizeof(tr_node_t));
	node->par = parent;

	if (parent == NULL) {
		tree->root = node;
		return;
	}

	ll_add_nth_node(parent->kid, ll_get_size(parent->kid), node);
}

void tr_remove_soft(tree_t *tree, void *data)
{
	tr_node_t *node = tr_search(tree, data);

	if (node == NULL) {
		return;
	}

	tr_node_t *parent = node->par;
	ll_node_t *curr = parent->kid->head;
	for (; curr; curr = curr->next) {
		if (curr->data == node) {
			break;
		}
	}

	DIE(curr == NULL, "search");

	free(ll_remove_node(parent->kid, curr)->data);
	free(ll_remove_node(parent->kid, curr));

	for (ll_node_t *curr = node->kid->head; curr; curr = curr->next) {
		((tr_node_t *)curr->data)->par = parent;
		ll_add_nth_node(parent->kid, ll_get_size(parent->kid), curr);
	}

	free(node->data);
	ll_free(&(node->kid));
	free(node);
}

static unsigned int __get_level(tr_node_t *root, tr_node_t *node,
								unsigned int level)
{
	if (root == NULL || node == NULL) {
		return 0;
	}

	if (root == node) {
		return level;
	}

	for (ll_node_t *curr = root->kid->head; curr; curr = curr->next) {
		unsigned int lev = __get_level(curr->data, node, level + 1);
		if (lev) {
			return lev;
		}
	}

	return 0;
}

unsigned int get_level(tree_t *tree, tr_node_t *node)
{
	return __get_level(tree->root, node, 1);
}
