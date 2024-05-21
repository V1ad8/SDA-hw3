#ifndef SPECIFIC_H
#define SPECIFIC_H

#include "../posts.h"
#include "tree.h"

ll_node_t *ll_create_post(const void *new_data, unsigned int data_size);

ll_node_t *ll_create_tree_post(const void *new_data, unsigned int data_size);

tr_node_t *tr_create_post(const void *data, unsigned int data_size);

void free_post(void *data);

int compare_post(void *data1, void *data2, size_t data_size);

#endif // SPECIFIC_LL_H
