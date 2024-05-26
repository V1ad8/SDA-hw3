#ifndef POSTS_H
#define POSTS_H

#include "add/tree.h"
#include "users.h"
#include "utils.h"
#include "friends.h"

typedef struct post_t {
	unsigned int id;
	char *title;
	unsigned int user_id;
	tree_t *events;

	bool likes[MAX_PEOPLE];
} post_t;

typedef struct profile_post_t {
	// true if the post is a repost false if it's a normal post
	bool is_repost;
	char *title;
	unsigned int id;
} profile_post_t;

#include "add/specific.h"
#include "posts_add.h"

/**
 * Function that handles the calling of every command from task 2
 *
 * Please add any necessary parameters to the functions
*/
void handle_input_posts(char *input, ll_list_t *posts);

// @brief Function to create a post and add it to the list of posts
// @param posts: The list of posts
// @param user: The user that creates the post
// @param title: The title of the post
// @return The created post
post_t *create_post(ll_list_t *posts, char *user, char *title);

// Function to make a repost of a post and add it to the tree of reposts
// @param posts: The list of posts
// @param user: The user that makes the repost
// @param post_id: The id of the post to repost
// @param repost_id: The id of the repost to repost [optional]
void repost_post(ll_list_t *posts, char *user, unsigned int post_id,
				 unsigned int repost_id);

// @brief Function to find the common repost of two reposts, basically lca
// @param posts: The list of posts
// @param post_id: The id of the post
// @param repost_id1: The id of the first repost
// @param repost_id2: The id of the second repost
void common_repost(ll_list_t *posts, unsigned int post_id,
				   unsigned int repost_id1, unsigned int repost_id2);

// @brief Function to like a post
// @param posts: The list of posts
// @param user: The user that likes the post
// @param post_id: The id of the post
// @param repost_id: The id of the repost [optional]
void like_post(ll_list_t *posts, char *user, unsigned int post_id,
			   unsigned int repost_id);

// @brief Function to find a ratio for a post
// @param posts: The list of posts
// @param post_id: The id of the post
void ratio(ll_list_t *posts, unsigned int post_id);

// @brief Function to delete a post and all its reposts
// @param posts: The list of posts
// @param post_id: The id of the post
// @param repost_id: The id of the repost [optional]
void delete_post(ll_list_t *posts, unsigned int post_id,
				 unsigned int repost_id);

// @brief Function to get the likes of a post
// @param posts: The list of posts
// @param post_id: The id of the post
// @param repost_id: The id of the repost [optional]
void get_likes(ll_list_t *posts, unsigned int post_id, unsigned int repost_id);

// @brief Function to get the reposts of a post
// @param posts: The list of posts
// @param post_id: The id of the post
// @param repost_id: The id of the repost [optional]
void get_reposts(ll_list_t *posts, unsigned int post_id,
				 unsigned int repost_id);

#endif // POSTS_H
