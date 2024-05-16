#ifndef TREE_H
#define TREE_H

#include "linked_list.h"

typedef struct tr_node_t {
	struct tree_t *par;
	void *data;
	ll_list_t *kid;
} tr_node_t;

typedef struct tree_t {
	tr_node_t *root;
	unsigned int data_size;
} tree_t;

tree_t *tree_create(unsigned int data_size);

void tree_destroy(tree_t *tree);

tree_t *tree_search(tree_t *tree, void *data);

void tree_insert(tree_t *tree, void *data);

void tree_remove_soft(tree_t *tree, void *data);

void tree_remove_hard(tree_t *tree, void *data);

#endif // TREE_H