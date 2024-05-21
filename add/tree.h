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

tree_t *tr_create(unsigned int data_size);

void tr_destroy(tree_t **tree, void (*free_data)(void *data));

tr_node_t *tr_search(tree_t *tree, void *data,
					 int (*compare)(void *, void *, size_t));

void tr_insert(tree_t *tree, void *data, tr_node_t *parent,
			   tr_node_t *(*tr_create_data)(const void *,
											unsigned int data_size),
			   ll_node_t *(*ll_create_data)(const void *,
											unsigned int data_size));

void tr_remove_soft(tr_node_t *node);

void tr_remove_hard(tr_node_t *node, void (*free_data)(void *data));

unsigned int get_level(tree_t *tree, tr_node_t *node);

tr_node_t *lca(tree_t *tree, tr_node_t *node1, tr_node_t *node2);

#endif // TREE_H
