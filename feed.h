#ifndef FEED_H
#define FEED_H

#include "friends.h"
#include "posts.h"
#include "add/hashtable.h"

// Enumeration for the visit status of a user in the clique
typedef enum visit_t { NOT_VISITED, IN_CLIQUE, NOT_IN_CLIQUE } visit_t;

#include "feed_add.h"

/**
 * Function that handles the calling of every command from task 3
 *
 * Please add any necessary parameters to the functions
*/
void handle_input_feed(char *input, list_graph_t *users_graph);

/**
 * @brief Shows the feed of a given user
 *
 * @param users_graph The graph with all users
 * @param posts The list of posts
 * @param name The name of the users whose feed is to be shown
 * @param feed_size The number of posts to be shown
 */
void show_feed(list_graph_t *users_graph, ll_list_t *posts, char *name,
			   int feed_size);

/**
 * @brief Go trough all reposts of a post
 *
 * @param node The current node in the search
 * @param users The graph with all users
 * @param friends Frequency array for the friends that reposted
 * @param user_id The id of the user whose friends we are looking for
 */
void traverse_reposts(tr_node_t *node, list_graph_t *users, int *friends,
					  unsigned int user_id);

/**
 * @brief Shows the name of all friends that reposted a certain post
 *
 * @param users_graph The graph with all users
 * @param posts The list of posts
 * @param name The name of the user whose friends we are looking for
 * @param post_id The id of the post
 */
void friends_repost(list_graph_t *users_graph, ll_list_t *posts, char *name,
					unsigned int post_id);

/**
 * @brief Insert a post in a list sorted by the id
 *
 * @param profile_list The list with the posts and reposts of a user
 * @param data The post that we are using to copy the data
 * @param title The title of the current post
 */
void insert_sorted(ll_list_t *profile_list, void *data, char *title);

/**
 * @brief Goes trough the reposts of a post recursively and stores those
 * belonging to a user in a list of posts and reposts of that user
 *
 * @param profile_list The list of posts and reposts of a user
 * @param node The current node int the search
 * @param user_id The id of the user whose posts and reposts we are looking for
 * @param title The title of the current post
 */
void look_for_reposts(ll_list_t *profile_list, tr_node_t *node,
					  unsigned int user_id, char *title);

/**
 * @brief Shows every post and repost of a user in order of the id
 *
 * @param posts The list of posts
 * @param name The name
 */
void show_profile(ll_list_t *posts, char *name);

// @brief Function that finds the maximum clique of a user
// @param users_graph: The graph of users
// @param user: The user to find the maximum clique for
void clique(list_graph_t *users_graph, unsigned int user);

#endif // FEED_H
