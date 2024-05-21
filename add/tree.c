#include "tree.h"

tree_t *tr_create(unsigned int data_size)
{
	// Allocate memory for the tree
	tree_t *tree = malloc(sizeof(tree_t));
	DIE(!tree, "malloc");

	// Initialize the tree
	tree->data_size = data_size;
	tree->root = NULL;

	// Return the tree
	return tree;
}

void tr_destroy(tree_t **tree, void (*free_data)(void *data))
{
	// Check if the tree is NULL
	if (!(*tree))
		return;

	// Remove the root recursively
	tr_remove((*tree)->root, free_data);

	// Free the tree
	free(*tree);
	*tree = NULL;
}

static tr_node_t *__tr_search(tr_node_t *node, void *data,
							  unsigned int data_size,
							  int (*compare)(void *, void *, size_t))
{
	// Check if the node is NULL
	if (!node || !data)
		return NULL;

	// Check if the data is the searched one
	if (compare(node->data, data, data_size) == 0)
		return node;

	// Search in the children recursively
	for (ll_node_t *curr = node->kid->head; curr; curr = curr->next) {
		tr_node_t *found = __tr_search(curr->data, data, data_size, compare);
		if (found)
			return found;
	}

	// Return NULL if the data was not found
	return NULL;
}

tr_node_t *tr_search(tree_t *tree, void *data,
					 int (*compare)(void *, void *, size_t))
{
	// Search recursively
	return __tr_search(tree->root, data, tree->data_size, compare);
}

tr_node_t *tr_create_node(void *data, unsigned int data_size)
{
	// Allocate memory for the node
	tr_node_t *node = calloc(1, sizeof(tr_node_t));
	DIE(!node, "malloc");

	// Allocate memory for the data of the node
	node->data = malloc(data_size);
	DIE(!node->data, "malloc");

	// Copy the data
	memcpy(node->data, data, data_size);

	// Initialize the children
	node->kid = ll_create(sizeof(tr_node_t));
	DIE(!node->kid, "ll_create");

	// Return the node
	return node;
}

void tr_insert(tree_t *tree, void *data, tr_node_t *parent,
			   tr_node_t *(*tr_create_data)(const void *,
											unsigned int data_size),
			   ll_node_t *(*ll_create_data)(const void *,
											unsigned int data_size))
{
	// Create the node and set the parent
	tr_node_t *node = tr_create_data(data, tree->data_size);
	node->par = parent;

	// Check if the parent is NULL
	if (!parent) {
		tree->root = node;
		return;
	}

	// Add the node to the parent's children
	ll_add_nth_node(parent->kid, ll_get_size(parent->kid), node,
					ll_create_data);

	// Free the memory
	free(node->data);
	free(node);
}

void tr_remove(tr_node_t *node, void (*free_data)(void *data))
{
	// Remove the children recursively
	for (ll_node_t *curr = node->kid->head; curr; curr = curr->next)
		tr_remove(curr->data, free_data);

	// Free the data and the children
	ll_free(&node->kid);
	free_data(node->data);

	// Free the node if it is the root
	if (!node->par)
		free(node);
}

static unsigned int __get_level(tr_node_t *root, tr_node_t *node,
								unsigned int level)
{
	// Check if the root or the node is NULL
	if (!root || !node)
		return 0;

	// Check if the root is the node
	if (root == node)
		return level;

	// Search in the children recursively
	for (ll_node_t *curr = root->kid->head; curr; curr = curr->next) {
		unsigned int lev = __get_level(curr->data, node, level + 1);
		if (lev)
			return lev;
	}

	// Return 0 if the node was not found
	return 0;
}

unsigned int get_level(tree_t *tree, tr_node_t *node)
{
	// Get the level recursively
	return __get_level(tree->root, node, 1);
}

tr_node_t *lca(tree_t *tree, tr_node_t *node1, tr_node_t *node2)
{
	// Check if the nodes are NULL
	if (!node1 || !node2)
		return NULL;

	// Get the levels of the nodes
	unsigned int level1 = get_level(tree, node1);
	unsigned int level2 = get_level(tree, node2);

	// Get the nodes at the same level
	for (; level1 > level2; level1--)
		node1 = node1->par;

	// Get the nodes at the same level
	for (; level2 > level1; level2--)
		node2 = node2->par;

	// Move the nodes up until they are the same
	while (node1 != node2) {
		node1 = node1->par;
		node2 = node2->par;
	}

	// Return the lowest common ancestor
	return node1;
}
