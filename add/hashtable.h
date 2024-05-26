/*
 * Copyright (c) 2024, <>
 */

// I do not know who the author is, I added the copyright for the style checker

#ifndef ADD_HASHTABLE_H
#define ADD_HASHTABLE_H

#include "linked_list.h"
#include "../utils.h"

typedef struct info_t {
	void *key;
	void *value;
} info_t;

typedef struct hashtable_t {
	ll_list_t **buckets;
	unsigned int size;
	unsigned int hmax;
	unsigned int (*hash_function)(void *data);
	int (*compare_function)(void *a, void *b);
} hashtable_t;

// @brief Hash a uint key
// @param key The key
// @return The hash of the key
unsigned int hash_uint(void *key);

// @brief Hash a string key
// @param key The key
// @return The hash of the key
unsigned int hash_string(void *key);

// @brief Compare two strings
// @param a The first string
// @param b The second string
// @return 0 if the strings are equal, 1 if the first string is greater, -1 if
// the second string is greater
int compare_strings(void *a, void *b);

// @brief Create a new hashtable
// @param hmax The maximum number of buckets
// @param hash_function The hash function
// @param compare_function The compare function
// @return The new hashtable
hashtable_t *ht_create(unsigned int hmax, unsigned int (*hash_function)(void *),
					   int (*compare_function)(void *, void *));

// @brief Check if the hashtable has a key
// @param ht The hashtable
// @param key The key
// @return 1 if the key is in the hashtable, 0 otherwise
int ht_has_key(hashtable_t *ht, void *key);

// @brief Get the value of a key
// @param ht The hashtable
// @param key The key
// @return The value of the key
void *ht_get(hashtable_t *ht, void *key);

// @brief Put a key-value pair in the hashtable
// @param ht The hashtable
// @param key The key
// @param key_size The size of the key
// @param value The value
// @param value_size The size of the value
void ht_put(hashtable_t *ht, void *key, unsigned int key_size, void *value,
			unsigned int value_size);

// @brief Remove an entry from the hashtable
// @param ht The hashtable
// @param key The key
void ht_remove_entry(hashtable_t *ht, void *key);

// @brief Free the hashtable
// @param ht The hashtable
void ht_free(hashtable_t *ht);

// @brief Get the size of the hashtable
// @param ht The hashtable
// @return The size of the hashtable
unsigned int ht_get_size(hashtable_t *ht);

// @brief Get the maximum number of buckets of the hashtable
// @param ht The hashtable
// @return The maximum number of buckets of the hashtable
unsigned int ht_get_hmax(hashtable_t *ht);

#endif /* ADD_HASHTABLE_H */
