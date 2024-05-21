#ifndef SPECIFIC_H
#define SPECIFIC_H

#include "../posts.h"
#include "../users.h"
#include "tree.h"

// @brief Create a linked list node containing a post
// @param new_data: The data to be added to the node
// @param data_size: The size of the data
// @return The created node
ll_node_t *ll_create_post(const void *new_data, unsigned int data_size);

// @brief Create a linked list node containing a tree containing a post
// @param new_data: The data to be added to the node
// @param data_size: The size of the data
// @return The created node
ll_node_t *ll_create_tree_post(const void *new_data, unsigned int data_size);

// @brief Create a tree node containing a post
// @param data: The data to be added to the node
// @param data_size: The size of the data
// @return The created node
tr_node_t *tr_create_post(const void *data, unsigned int data_size);

// @brief Free the data of a post
// @param data: The data to be freed
void free_post(void *data);

// @brief Compare a post with a given id
// @param data1: The post to compare
// @param data2: The id to compare with
// @param data_size: The size of the data
// @return 1, 0 or -1 if the post is greater, equal or smaller
int compare_post(void *post, void *id, size_t data_size);

#endif // SPECIFIC_LL_H
