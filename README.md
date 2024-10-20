# **SDA-homework-3: Social-Media**

**Marinescu Andrei-Bogdan 315CA**

**Ungureanu Vlad-Marin 315CA**

```
SDA Homework 3 Team:
amarinescu0804
vlad.ungureanu1808
```

## Task Description

The program is a simplified social network, written in C, that allows users to add each other as friends, post texts and repost other posts. Every user can view their own feed composed of their posts and their friend's posts, as well as viewing one's profile, meaning they can see every post and repost of a certain user. The program is composed of three separate subprograms: the first one is related to users and the interactions between them, the second one is related to posts and reposts and the third one is related to the feed of each user.

## Functionalities
The program provides the following functionalities:

User realted:
* **add**: Adds a connection between two users
* **remove**: Removes the connection between two users
* **distance**: Calculates the distance between two users on the platform
* **suggestions**: Lists every friend suggestion for a user (all friends of the user's friends that are not already connected with the user)
* **common**: Lists the common friends between two users
* **friends**: Shows the number of friends of a user
* **popular**: Shows the user with the most friends between a given user and their friends

Posts and reposts related:
* **create**: Creates a post for a user
* **repost**: Reposts a post or another repost
* **common_repost**: Finds the first common repost of two reposts
* **like**: Likes/Unlikes a post by a given user
* **ratio**: Shows if a post was ratio'd by a repost or not (ratio means a repost got more likes than the original post)
* **delete**: Deletes a post or repost (every repost attached to the deleted post/repost will also be deleted)
* **get-likes**: Shows the number of likes a post or repost got
* **get-reposts**: Shows the hierarchy of reposts for a post or repost

Feed related:
* **feed**: Shows the most recent posts made by a user or their friends
* **view-profile**: Shows every post and repost made by a user in chronological order
* **friends-repost**: Shows every friend of a user that reposted a certain post
* **common-group**: Shows the biggest friends group containing a given user

## Commands
The described functionalities work by receiving the following inputs:

User related:
* **add** <*name_1*> <*name_2*>
* **remove** <*name_1*> <*name_2*>
* **distance** <*name_1*> <*name_2*>
* **suggestions** <*name*>
* **common** <*name_1*> <*name_2*>
* **friends** <*name*>
* **popular** <*name*>

Posts and reposts related:
* **create** <*name*> <*title*>
* **repost** <*name*> <*post_id*> [*repost_id*]
* **common_repost** <*post_id*> <*repost_id_1*> <*repost_id_2*>
* **like** <*name*> <*post_id*> [*repost_id*]
* **ratio** <*post_id*>
* **delete** <*post_id*> [*repost_id*]
* **get-likes** <*post_id*> [*repost_id*]
* **get-reposts** <*post_id*> [*repost_id*]

Feed related:
* **feed** <*name*> <*feed_size*>
* **view-profile** <*name*>
* **friends-repost** <*name*> <*post_id*>
* **common-group** <*name*>

## Usage
To use the program, follow these steps:
* Compile the program with the *`make build`* rule within the provided Makefile
```bash
vlad@laptop:~SDA/hws/hw3/$ make build
gcc -Wall -Wextra -Werror -g   -c -o users.o users.c
gcc -Wall -Wextra -Werror -g   -c -o friends.o friends.c
gcc -Wall -Wextra -Werror -g -c -D TASK_1 -o social_media_friends.o social_media.c
gcc -Wall -Wextra -Werror -g -o friends users.o add/graph.c add/linked_list.c add/queue.c add/specific.c add/tree.c posts_add.c friends.o social_media_friends.o
gcc -Wall -Wextra -Werror -g   -c -o posts.o posts.c
gcc -Wall -Wextra -Werror -g -c -D TASK_2 -o social_media_posts.o social_media.c
gcc -Wall -Wextra -Werror -g -o posts users.o add/graph.c add/linked_list.c add/queue.c add/specific.c add/tree.c posts_add.c posts.o social_media_posts.o
gcc -Wall -Wextra -Werror -g   -c -o feed.o feed.c
gcc -Wall -Wextra -Werror -g -c -D TASK_1 -D TASK_2 -D TASK_3 -o social_media_feed.o social_media.c
gcc -Wall -Wextra -Werror -g -o feed users.o add/graph.c add/linked_list.c add/queue.c add/specific.c add/tree.c posts_add.c posts.o friends.o feed.o social_media_feed.o
```
* Run the program (each task individually)
```bash
vlad@laptop:~SDA/hws/hw3$ ./friends
vlad@laptop:~SDA/hws/hw3$ ./posts
vlad@laptop:~SDA/hws/hw3$ ./feed
```
## Implementation Information
The code is spread throughout eleven C source files to make reading them individually easier. The functions are divided as follows:
 * `social_media.c`: initializes every task based on which task we are running; forwards the input to the right source file
 * `users.c`: contains the initialization of the users array, the function that frees the users array and two functions that connect a name and an id (get each one based on the other)
 * `friends.c`: contains the function that handles the input and the functions related to the users (adding friends, removing friends, etc.)
 * `posts.c`: contains the function that handles the input and the functions related to the posts (creating posts/reposts, liking etc.)
 * `posts_add.c`: contains additional functions needed by the functions in the previous file
 * `feed.c`: contains the function that handles the input and the functions related to the feed (showing the feed, viewing a profile, etc.)
 * `feed_add.c`: contains additional functions needed for the clique functionality
 * `add/graph.c`, `add/linked_list.c`, `add/queue.c`, `add/hashtable.c`: contain implementations of the data structures borrowed from the 7th lab [skeleton](https://ocw.cs.pub.ro/courses/_media/sd-ca/laboratoare/lab07_2024.zip)
 * `add/tree.c`: contains the implementation of a generic tree data structure made by Vlad
 * `add/specific.c`: contains functions for creating/deleting/comparing data with more complex structures; needed as parameters for some of the previous functions

 These source files are supported by eleven header files:
 * `add/graph.h`, `add/linked_list.h`, `add/queue.h`, `add/tree.h`, `add/specific.h`, `add/hashtable.h`, `users.h`, `friends.h`, `posts.h`, `posts_add.h`, `feed.h`, `feed_add.h`: contain the headers of the corresponding C files and the declarations of the functions used in them
 * `utils.h`: contains the declaration of the macro `DIE` and the C boolean data type

 ### Data Structures Used
We used the following data structures:
* linked list (doubly linked)
```c
// structure for a node in a linked list (doubly linked)
typedef struct ll_node_t {
	void *data;
	struct ll_node_t *next, *prev;
} ll_node_t;

// structure for the linked list (doubly linked)
typedef struct ll_list_t {
	ll_node_t *head, *tail;
	unsigned int data_size;
	unsigned int size;
} ll_list_t;
```
* graph
```c
// structure for a graph using adjacency lists
typedef struct list_graph_t {
	ll_list_t **neighbors;
	int nodes;
} list_graph_t;
```
* hashtable
```c
// structure for the data inside the hashtable
typedef struct info_t {
	void *key;
	void *value;
} info_t;

// structure for the hashtable
typedef struct hashtable_t {
	ll_list_t **buckets;
	unsigned int size;
	unsigned int hmax;
	unsigned int (*hash_function)(void *data);
	int (*compare_function)(void *a, void *b);
} hashtable_t;
```

* queue
```c
// queue using a circular array
typedef struct queue_t {
	unsigned int max_size;
	unsigned int size;
	unsigned int data_size;
	unsigned int read_idx;
	unsigned int write_idx;
	void **buff;
} queue_t;
```
* generic tree
```c
// generic tree node
typedef struct tr_node_t {
	struct tr_node_t *par;
	void *data;
	ll_list_t *kid;
} tr_node_t;

// generic tree
typedef struct tree_t {
	tr_node_t *root;
	unsigned int data_size;
} tree_t;
```

## Implementation
### `main()`
It selects the task we are currently running and initiates the users graph and posts list. It redirects the input to the right input handler (friends, posts or feed).

### `handle_input_friends()`
It takes the input related to the friends and completes the task accordingly:
* if the input is *`add`*, it creates a connection between two users in the graph
* if the input is *`remove`*, it removes the connection between two users in the graph
* if the input is *`distance`*, the distance between two users in the graph is shown (using a bfs traversal of the graph)
* if the input is *`suggestions`*, the program looks trough the friends of every friend of the given user and lists those who aren't already connected to the given user
* if the input is *`common`*, the program looks trough the list of friends of both users and shows only those who are friends with both of them
* if the input is *`friends`*, the program calculates the number of friends a given user has
* if the input is *`popular`*, the program looks trough the list of friends of a given user and shows the one with the most friends

### `handle_input_posts()`
It takes the input related to the posts and completes the task accordingly:
* if the input is *`create`*, the program creates a post for a certain user
* if the input is *`repost`*, the program reposts a post for a certain user
* if the input is *`common-repost`*, the program does the `LCA` problem for the tree of reposts
* if the input is *`like`*, the program likes/dislikes a post/repost
* if the input is *`ratio`*, the program finds if a post has less likes than one of its reposts
* if the input is *`delete`*, the program deletes a post/repost and all of its reposts
* if the input is *`get-likes`*, the program prints the number of likes for a post/repost
* if the input is *`get-reposts`*, the program prints all the reposts of a post/repost


### `handle_input_feed()`
It takes the input related to the feed and completes the task accordingly:
* if the input is *`feed`*, the program goes trough the list of posts backwards (to show the most recent posts first), and only shows the posts of the given user or their friends
* if the input is *`view-profile`*, the program goes trough the list of posts and reposts and creates a new list, sorted by the id, composed of only the posts and reposts of a certain user
* if the input is *`friends-repost`*, the program goes trough the tree of reposts of a given post and looks for friends of a given user that reposted that post, it then lists them in order of their id's
* if the input is *`common-group`*, the program starts a backtracking function that checks for all the possible configurations of the clique; it implements a hashtable to make the process faster and checks a weird condition for the same purpose
