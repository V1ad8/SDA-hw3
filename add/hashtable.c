/*
 * Copyright (c) 2024, <>
 */

// I do not know who the author is, I added the copyright for the style checker

#include "hashtable.h"

unsigned int hash_uint(void *key)
{
	// Cast the key to an unsigned int
	unsigned int uint_key = *((unsigned int *)key);

	// Hash the key
	uint_key = ((uint_key >> 16u) ^ uint_key) * 0x45d9f3b;
	uint_key = ((uint_key >> 16u) ^ uint_key) * 0x45d9f3b;
	uint_key = (uint_key >> 16u) ^ uint_key;

	// Return the hash
	return uint_key;
}

unsigned int hash_string(void *key)
{
	// Cast the key to an unsigned char
	unsigned char *key_string = (unsigned char *)key;
	unsigned int hash = 5381;
	int c;

	// Hash the key
	while ((c = *key_string++))
		hash = ((hash << 5u) + hash) + c;

	// Return the hash
	return hash;
}

int compare_strings(void *a, void *b)
{
	// Cast the keys to strings
	char *str_a = (char *)a;
	char *str_b = (char *)b;

	// Compare the strings
	return strcmp(str_a, str_b);
}

hashtable_t *ht_create(unsigned int hmax, unsigned int (*hash_function)(void *),
					   int (*compare_function)(void *, void *))
{
	// Check if the parameters are valid
	if (!hash_function || !compare_function)
		return NULL;

	// Allocate memory for the hashtable
	hashtable_t *map = malloc(sizeof(hashtable_t));
	DIE(!map, "Error");

	// Initialize the hashtable
	map->size = 0;
	map->hmax = hmax;
	map->hash_function = hash_function;
	map->compare_function = compare_function;

	// Allocate memory for the buckets
	map->buckets = malloc(map->hmax * sizeof(ll_list_t *));
	DIE(!map->buckets, "Error");
	for (unsigned int i = 0; i < map->hmax; ++i)
		map->buckets[i] = ll_create(sizeof(info_t));

	// Return the hashtable
	return map;
}

int ht_has_key(hashtable_t *ht, void *key)
{
	// Check if the parameters are valid
	if (!ht || !key)
		return -1;

	// Compute the hash index
	int hash_index = ht->hash_function(key) % ht->hmax;
	ll_node_t *node = ht->buckets[hash_index]->head;

	// Search for the key in the hashtable
	while (node) {
		info_t *data_info = (info_t *)node->data;
		if (!ht->compare_function(data_info->key, key))
			return 1;
		node = node->next;
	}

	// Return 0 if the key is not found
	return 0;
}

void *ht_get(hashtable_t *ht, void *key)
{
	// Check if the parameters are valid
	if (!ht || !key || ht_has_key(ht, key) != 1)
		return NULL;

	// Compute the hash index
	int hash_index = ht->hash_function(key) % ht->hmax;
	ll_node_t *node = ht->buckets[hash_index]->head;

	// Search for the key in the hashtable
	while (node) {
		info_t *data_info = (info_t *)node->data;
		if (!ht->compare_function(data_info->key, key))
			return data_info->value;
		node = node->next;
	}

	// Return NULL if the key is not found
	return NULL;
}

void ht_put(hashtable_t *ht, void *key, unsigned int key_size, void *value,
			unsigned int value_size)
{
	// Check if the parameters are valid
	if (!ht || !key || !value)
		return;

	// Compute the hash index
	int hash_index = ht->hash_function(key) % ht->hmax;

	// Update the value if the key is already in the hashtable
	if (ht_has_key(ht, key) == 1) {
		ll_node_t *node = ht->buckets[hash_index]->head;

		// Search for the key in the hashtable
		while (node) {
			info_t *data_info = node->data;

			// Update the value when the key is found
			if (!ht->compare_function(data_info->key, key)) {
				free(data_info->value);

				data_info->value = malloc(value_size);
				DIE(!data_info, "Error");

				memcpy(data_info->value, value, value_size);
				return;
			}

			node = node->next;
		}
	}

	// Add the key-value pair to the hashtable

	// Allocate memory for the key and value
	info_t *data_info = malloc(sizeof(info_t));
	DIE(!data_info, "Error");

	data_info->key = malloc(key_size);
	DIE(!data_info->key, "Error");
	data_info->value = malloc(value_size);
	DIE(!data_info->value, "Error");

	// Copy the key and value
	memcpy(data_info->key, key, key_size);
	memcpy(data_info->value, value, value_size);

	// Add the key-value pair to the hashtable
	ll_add_nth_node(ht->buckets[hash_index], 0, data_info, ll_create_node);
	ht->size++;

	// Free the memory
	free(data_info);
}

void ht_remove_entry(hashtable_t *ht, void *key)
{
	// Check if the parameters are valid
	if (!ht || !key || ht_has_key(ht, key) != 1)
		return;

	// Compute the hash index
	int hash_index = ht->hash_function(key) % ht->hmax;
	ll_node_t *node = ht->buckets[hash_index]->head;

	// Initialize the node number for the linked list
	unsigned int node_nr = 0;

	// Search for the key in the hashtable
	while (node) {
		info_t *data_info = (info_t *)node->data;

		// Remove the key-value pair when the key is found
		if (!ht->compare_function(data_info->key, key)) {
			free(data_info->key);
			free(data_info->value);
			free(data_info);

			// Remove the node from the linked list
			ll_node_t *deleted_node =
				ll_remove_nth_node(ht->buckets[hash_index], node_nr);
			free(deleted_node);

			// Update the size of the hashtable
			ht->size--;
			return;
		}

		node = node->next;
		node_nr++;
	}
}

void ht_free(hashtable_t *ht)
{
	// Check if the hashtable is valid
	if (!ht)
		return;

	// Iterate through the buckets and free the memory
	for (unsigned int i = 0; i < ht->hmax; ++i) {
		ll_node_t *node = ht->buckets[i]->head;

		// Free the memory for the key and value
		while (node) {
			info_t *data_info = (info_t *)node->data;
			free(data_info->key);
			free(data_info->value);
			node = node->next;
		}

		// Free the memory for the linked list
		ll_free(&ht->buckets[i]);
	}

	// Free the memory for the buckets and hashtable
	free(ht->buckets);
	free(ht);
}

unsigned int ht_get_size(hashtable_t *ht)
{
	// Check if the hashtable is valid
	if (!ht)
		return 0;

	// Return the size of the hashtable
	return ht->size;
}

unsigned int ht_get_hmax(hashtable_t *ht)
{
	// Check if the hashtable is valid
	if (!ht)
		return 0;

	// Return the maximum number of buckets
	return ht->hmax;
}
