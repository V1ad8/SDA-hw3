#ifndef POSTS_H
#define POSTS_H

#include "add/tree.h"
#include "utils.h"

#define MAX_USERS 518

typedef struct post_t {
	unsigned int id;
	char *title;
	unsigned int user_id;
	tree_t *events;

	bool likes[MAX_USERS];
} post_t;

#include "add/specific.h"

#define MAX_TITLE 280

/**
 * Function that handles the calling of every command from task 2
 *
 * Please add any necessary parameters to the functions
*/
void handle_input_posts(char *input, ll_list_t *posts);

// @brief Function to read a block of memory placed in between quotation marks
// @return The block of memory read
char *read_text(char *input);

post_t *create_post(ll_list_t *posts, char *user, char *text);

void repost_post(ll_list_t *posts, char *user, char *text);

void like_post(ll_list_t *posts, char *user, char *text);

void get_reposts(ll_list_t *posts, char *text);

void common_repost(ll_list_t *posts, char *text);

void get_likes(ll_list_t *posts, char *text);

void ratio(ll_list_t *posts, char *text);

void delete_post(ll_list_t *posts, char *text);

#endif // POSTS_H
