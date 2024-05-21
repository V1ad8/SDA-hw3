/*
 * Copyright (c) 2024, <>
 */

// I do not know who the author is, I added the copyright for the style checker

#include "linked_list.h"

ll_list_t *ll_create(unsigned int data_size)
{
	// Allocate memory for the linked list
	ll_list_t *ll = calloc(1, sizeof(*ll));
	DIE(!ll, "calloc list");

	// Initialize the fields of the linked list
	ll->data_size = data_size;

	// Return the linked list
	return ll;
}

ll_node_t *ll_get_nth_node(ll_list_t *list, unsigned int n)
{
	// Check if the list is NULL or if the index is out of bounds
	if (!list || n >= list->size)
		return NULL;

	// Helper variables
	unsigned int len = list->size - 1;
	unsigned int i;
	ll_node_t *node;

	// Check if the index is closer to the head or the tail
	if (n <= len / 2) {
		// Iterate from the head
		node = list->head;
		for (i = 0; i < n; ++i)
			node = node->next;
	} else {
		// Iterate backwards from the tail
		node = list->tail;
		for (i = len; i > n; --i)
			node = node->prev;
	}

	// Return the node
	return node;
}

ll_node_t *ll_create_node(const void *new_data, unsigned int data_size)
{
	// Allocate memory for the node
	ll_node_t *node = calloc(1, sizeof(*node));
	DIE(!node, "calloc node");

	// Allocate memory for the data
	node->data = malloc(data_size);
	DIE(!node->data, "malloc data");

	// Copy the data
	memcpy(node->data, new_data, data_size);

	// Return the node
	return node;
}

ll_node_t *
ll_add_nth_node(ll_list_t *list, unsigned int n, const void *new_data,
				ll_node_t *(*create_data)(const void *, unsigned int data_size))
{
	// Check if the list is NULL or if the index is out of bounds
	if (!list || n > list->size)
		return NULL;

	// Helper variables
	ll_node_t *new_node, *prev_node;

	// Create the new node
	new_node = create_data(new_data, list->data_size);
	DIE(!new_node, "create data");

	// Check where to add the new node
	if (n == 0) {
		// Add the new node at the head
		new_node->next = list->head;

		// Reconnect the pointers
		if (list->head)
			list->head->prev = new_node;
		list->head = new_node;
		if (!list->tail)
			list->tail = new_node;
	} else if (n == list->size) {
		// Add the new node at the tail
		new_node->prev = list->tail;

		// Reconnect the pointers
		list->tail->next = new_node;
		list->tail = new_node;
	} else {
		// Add the new node in the middle

		// Get the previous node
		prev_node = ll_get_nth_node(list, n - 1);

		// Reconnect the pointers
		new_node->prev = prev_node;
		new_node->next = prev_node->next;
		prev_node->next->prev = new_node;
		prev_node->next = new_node;
	}

	// Update the size of the list
	++list->size;

	// Return the new node
	return new_node;
}

ll_node_t *ll_remove_nth_node(ll_list_t *list, unsigned int n)
{
	// Check if the list is NULL or if the index is out of bounds
	if (!list || !list->size || n >= list->size)
		return NULL;

	// Helper variables
	ll_node_t *prev_node, *removed_node;

	// Check where to remove the node
	if (n == 0) {
		// Remove the node from the head
		removed_node = list->head;

		// Reconnect the pointers
		list->head = removed_node->next;
		if (list->head)
			list->head->prev = NULL;
		if (removed_node == list->tail)
			list->tail = NULL;
	} else if (n == list->size - 1) {
		// Remove the node from the tail
		removed_node = list->tail;

		// Reconnect the pointers
		list->tail = removed_node->prev;
		list->tail->next = NULL;
	} else {
		// Remove the node from the middle
		prev_node = ll_get_nth_node(list, n - 1);

		// Reconnect the pointers
		removed_node = prev_node->next;
		prev_node->next = removed_node->next;
		prev_node->next->prev = prev_node;
	}

	// Update the size of the list
	--list->size;

	// Return the removed node
	return removed_node;
}

ll_node_t *ll_remove_node(ll_list_t *list, ll_node_t *node)
{
	// Check if the list is NULL or if the node is NULL
	if (!list || !node)
		return NULL;

	// Reconnect the pointers before removing the node
	if (node == list->head) {
		list->head = node->next;
		if (list->head)
			list->head->prev = NULL;
	} else {
		node->prev->next = node->next;
	}

	// Reconnect the pointers after removing the node
	if (node == list->tail) {
		list->tail = node->prev;
		if (list->tail)
			list->tail->next = NULL;
	} else {
		node->next->prev = node->prev;
	}

	// Update the size of the list
	list->size--;

	// Return the removed node
	return node;
}

unsigned int ll_get_size(ll_list_t *list)
{
	// Return the size of the list
	return !list ? 0 : list->size;
}

void ll_free(ll_list_t **pp_list)
{
	// Check if the list is NULL
	if (!pp_list || !*pp_list)
		return;

	// Helper variable
	ll_node_t *node;

	// Remove all the nodes and free their data
	while ((*pp_list)->size) {
		node = ll_remove_nth_node(*pp_list, 0);
		free(node->data);
		free(node);
	}

	// Free the list
	free(*pp_list);
	*pp_list = NULL;
}
