#ifndef POSTS_ADD_H
#define POSTS_ADD_H

// @brief Function to read a string placed in between quotation marks
// @param input The file from which to read the block of memory
// @return The string read from the file
char *read_text(char *input);

// @brief Function to get a post from a list of posts
// @param posts The list of posts
// @param post_id The id of the post to get
// @return The post with the given id
post_t *get_post(ll_list_t *posts, unsigned int post_id);

// @brief Function to count the number of digits in a number
// @param number The number to count the digits of
// @return The number of digits in the number
unsigned int number_of_digits(unsigned int number);

// @brief Recursive function to print the reposts of a post
// @param node The node to get the reposts from
void __get_reposts(tr_node_t *node);

// @brief Function to count the number of likes a post has
// @param post The post to count the likes of
// @return The number of likes the post has
unsigned int count_likes(post_t *post);

// @brief Recursive function to get the post with the most likes from a tree
// node
// @param node The node to get the top post from
// @param top_likes The maximum number of likes found
// @return The post with the most likes
tr_node_t *get_top_post(tr_node_t *node, unsigned int *top_likes);

#endif // POSTS_ADD_H