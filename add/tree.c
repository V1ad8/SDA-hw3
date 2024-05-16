#include "tree.h"

tree_t *tree_create(unsigned int data_size)
{
	tree_t *tree = malloc(sizeof(tree_t));
	tree->data_size = data_size;
	tree->root = NULL;
	return tree;
}

void tree_destroy(tree_t *tree)
{
	tree_remove_hard(tree, tree->root->data);
	free(tree);
}

static tr_node_t *__tree_search(tr_node_t *node, void *data,
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
		tr_node_t *found = __tree_search(curr->data, data, data_size);
		if (found != NULL) {
			return found;
		}
	}

	return NULL;
}

tree_t *tree_search(tree_t *tree, void *data)
{
	return __tree_search(tree->root, data, tree->data_size);
}

void tree_insert(tree_t *tree, void *data)
{
	tr_node_t *node = malloc(sizeof(tr_node_t));
	DIE(node == NULL, "malloc");

	node->data = malloc(tree->data_size);
	DIE(node->data == NULL, "malloc");

	memcpy(node->data, data, tree->data_size);
	node->kid = ll_create(tree->data_size);
	node->par = NULL;

	if (tree->root == NULL) {
		tree->root = node;
		return;
	}

	tr_node_t *parent = tree_search(tree, data);
	DIE(parent == NULL, "search");

	parent = parent->par;

	node->par = parent;
	ll_add_nth(parent->kid, ll_get_size(parent->kid), node);
}

void tree_remove_soft(tree_t *tree, void *data)
{
	tr_node_t *node = tree_search(tree, data);
	DIE(node == NULL, "search");

	tr_node_t *parent = node->par;
	ll_node_t *curr = parent->kid->head;
	for (; curr; curr = curr->next) {
		if (curr->data == node) {
			break;
		}
	}

	DIE(curr == NULL, "search");

	free(ll_get_nth_node(parent->kid, curr)->data);
	free(ll_remove_node(parent->kid, curr));

	for (ll_node_t *curr = node->kid->head; curr; curr = curr->next) {
		((tr_node_t *)curr->data)->par = parent;
		ll_add_nth_node(parent->kid, ll_get_size(parent->kid), curr);
	}

	free(node->data);
	ll_free(node->kid);
	free(node);
}

void tree_remove_hard(tree_t *tree, void *data)
{
	tr_node_t *node = tree_search(tree, data);
	DIE(node == NULL, "search");

	tr_node_t *parent = node->par;
	ll_node_t *curr = parent->kid->head;
	for (; curr; curr = curr->next) {
		if (curr->data == node) {
			break;
		}
	}

	DIE(curr == NULL, "search");

	free(ll_get_nth_node(parent->kid, curr)->data);
	free(ll_remove_node(parent->kid, curr));

	while (ll_get_size(node->kid) > 0) {
		tr_node_t *kid = ll_remove_node(node->kid, 0)->data;
		tree_remove_hard(tree, kid->data);
		free(kid);
	}

	free(node->data);
	ll_free(node->kid);
	free(node);
}
