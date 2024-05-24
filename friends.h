#ifndef FRIENDS_H
#define FRIENDS_H

#define MAX_COMMAND_LEN 300
#define MAX_PEOPLE 550

#include "add/graph.h"

/**
 * Function that handles the calling of every command from task 1
 *
 * Please add any necessary parameters to the functions
*/
void handle_input_friends(char *input, list_graph_t *users_graph);

/**
 * @brief Create a connection between two users
 *
 * @param users The graph with all users
 * @param name1 The name of the first user
 * @param name2 The name of the second user
 */
void add_friend(list_graph_t *users, char *name1, char *name2);

/**
 * @brief Removes the connection between two users
 *
 * @param users The graph with all users
 * @param name1 The name of the first user
 * @param name2 The name of the second user
 */
void remove_friend(list_graph_t *users, char *name1, char *name2);

/**
 * @brief Determines the number of friends of a users
 *
 * @param users The graph with all users
 * @param name The name of the user
 * @return The number of friends of a users
 */
int nr_of_friends(list_graph_t *users, char *name);

/**
 * @brief Lists the common friends of two users in order of their id's
 *
 * @param users The graph with all users
 * @param name1 The name of the first user
 * @param name2 The name of the second user
 */
void common_friends(list_graph_t *users, char *name1, char *name2);

/**
 * @brief Shows the friend suggestions of a given user
 *
 * @param users The graph with all users
 * @param name The name of the user
 */
void suggestions(list_graph_t *users, char *name);

/**
 * @brief Shows the most popular user out of a given user and their friends
 *
 * @param users The graph with all users
 * @param name The name of the user
 */
void popular(list_graph_t *users, char *name);

/**
 * @brief Breadth First Search traversal of a graph that saves the distance
 * from a given node to every other node in the graph
 *
 * @param lg The graph to be traversed
 * @param start The starting node of the traversal
 * @param visited Array that marks the nodes already visited
 * @param dist Array that saves the distance from the starting
 * point to every other node
 */
void bfs(list_graph_t *lg, int start, int *visited, int *dist);

/**
 * @brief Prints the distance between two users
 *
 * @param users The graph with all users
 * @param name1 The name of the first user
 * @param name2 The name of the second user
 */
void friend_distance(list_graph_t *users, char *name1, char *name2);

#endif // FRIENDS_H
