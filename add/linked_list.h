/*
 * Copyright (c) 2024, <>
 */

// I do not know who the author is, I added the copyright for the style checker

#ifndef ADD_LINKED_LIST_H
#define ADD_LINKED_LIST_H

#include "../utils.h"

typedef struct ll_node_t {
	void *data;
	struct ll_node_t *next, *prev;
} ll_node_t;

typedef struct ll_list_t {
	ll_node_t *head, *tail;
	unsigned int data_size;
	unsigned int size;
} ll_list_t;

// @brief Create a new linked list
// @param data_size The size of the data
// @return The new linked list
ll_list_t *ll_create(unsigned int data_size);

// @brief Get the nth node of the linked list
// @param list The linked list
// @param n The index of the node
// @return The nth node of the linked list
ll_node_t *ll_get_nth_node(ll_list_t *list, unsigned int n);

// @brief Create a new node
// @param new_data The new data
// @param data_size The size of the data
// @return The new node
ll_node_t *ll_create_node(const void *new_data, unsigned int data_size);

// @brief Add a new node at the nth position
// @param list The linked list
// @param n The index of the node
// @param new_data The new data
// @param create_data The function to create the data
// @return The new node
ll_node_t *ll_add_nth_node(ll_list_t *list, unsigned int n,
						   const void *new_data,
						   ll_node_t *(*create_data)(const void *,
													 unsigned int data_size));

// @brief Remove the nth node of the linked list
// @param list The linked list
// @param n The index of the node
// @return The removed node
ll_node_t *ll_remove_nth_node(ll_list_t *list, unsigned int n);

// @brief Remove a node from the linked list
// @param list The linked list
// @param node The node to be removed
// @return The removed node
ll_node_t *ll_remove_node(ll_list_t *list, ll_node_t *node);

// @brief Get the size of the linked list
// @param list The linked list
// @return The size of the linked list
unsigned int ll_get_size(ll_list_t *list);

// @brief Free the data of the linked list
// @param pp_list The linked list
void ll_free(ll_list_t **pp_list);

#endif /* ADD_LINKED_LIST_H */
