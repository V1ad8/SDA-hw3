#ifndef SPECIFIC_H
#define SPECIFIC_H

#include "../posts.h"
#include "tree.h"

ll_node_t *ll_add_nth_post(ll_list_t *list, unsigned int n,
						   const post_t *new_data);

void tr_insert_post(tree_t *tree, post_t *data, tr_node_t *parent);

void tr_remove_hard(tr_node_t *node);

void tr_destroy(tree_t **tree);

tr_node_t *get_post_by_id(tree_t *tree, unsigned int post_id);

ll_node_t *ll_add_nth_tree(ll_list_t *list, unsigned int n,
						   const tr_node_t *new_data);

#endif // SPECIFIC_LL_H
