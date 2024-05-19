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

void tr_destroy(tree_t **tree);

tr_node_t *tr_search(tree_t *tree, void *data);

void tr_insert(tree_t *tree, void *data, tr_node_t *parent);

void tr_remove_soft(tree_t *tree, void *data);

void tr_remove_hard(tr_node_t *node);

#endif // TREE_H