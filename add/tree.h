#ifndef TREE_H
#define TREE_H

#include "linked_list.h"

typedef struct tr_node_t {
	struct tr_node_t *par;
	void *data;
	ll_list_t *kid;
} tr_node_t;

typedef struct tree_t {
	tr_node_t *root;
	unsigned int data_size;
} tree_t;

// @brief Create a tree with a given data size
// @param data_size The size of the data
// @return The tree
tree_t *tr_create(unsigned int data_size);

// @brief Destroy a tree
// @param tree The tree to destroy
// @param free_data The function to free the data in the tree
void tr_destroy(tree_t **tree, void (*free_data)(void *data));

// @brief Search for a node in the tree recursively
// @param tree The tree to search in
// @param data The data to search for
// @param compare The function to compare the data
// @return The node if found, NULL otherwise
tr_node_t *tr_search(tree_t *tree, void *data,
					 int (*compare)(void *, void *, size_t));

// @brief Insert a node in the tree
// @param tree The tree to insert in
// @param data The data to insert
// @param parent The parent of the node
// @param tr_create_data The function to create the data in the tree
// @param ll_create_data The function to create the data in the linked list
void tr_insert(tree_t *tree, void *data, tr_node_t *parent,
			   tr_node_t *(*tr_create_data)(const void *,
											unsigned int data_size),
			   ll_node_t *(*ll_create_data)(const void *,
											unsigned int data_size));

// @brief Remove a node from the tree and all its children recursively
// @param node The node to remove
// @param free_data The function to free the data
void tr_remove(tr_node_t *node, void (*free_data)(void *data));

// @brief Get the level of a node in the tree recursively
// @param tree The tree to get the level from
// @param node The node to get the level of
// @return The level of the node
unsigned int get_level(tree_t *tree, tr_node_t *node);

// @brief Find the lowest common ancestor of two nodes in the tree
// @param tree The tree to find the lowest common ancestor in
// @param node1 The first node
// @param node2 The second node
// @return The lowest common ancestor
tr_node_t *lca(tree_t *tree, tr_node_t *node1, tr_node_t *node2);

#endif // TREE_H
