#ifndef FRIENDS_H
#define FRIENDS_H

#define MAX_COMMAND_LEN 100
#define MAX_PEOPLE 550

#include "lab_implem.h"

/**
 * Function that handles the calling of every command from task 1
 *
 * Please add any necessary parameters to the functions
*/
void handle_input_friends(char *input, list_graph_t *users_graph);
void add_friend(list_graph_t *users, char *name1, char *name2);
void remove_friend(list_graph_t *users, char *name1, char *name2);
int nr_of_friends(list_graph_t *users, char *name);
void common_friends(list_graph_t *users, char *name1, char *name2);
void suggestions(list_graph_t *users, char *name);
void popular(list_graph_t *users, char *name);
void bfs(list_graph_t* lg, int start, int* visited, int* dist);
void friend_distance(list_graph_t *users, char *name1, char *name2);

#endif // FRIENDS_H
