#ifndef SPECIFIC_H
#define SPECIFIC_H

#include "../posts.h"
#include "tree.h"

ll_node_t *ll_add_nth_post(ll_list_t *list, unsigned int n, post_t *new_data);

void tr_insert_post(tree_t *tree, post_t *data, tr_node_t *parent);

#endif // SPECIFIC_LL_H
