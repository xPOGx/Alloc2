#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <stdlib.h>

#include <stdbool.h>
#define DEFAULT_MEM_SIZE 1024 // 1 MB

/*
 * Memory heap structure
 */
struct {
	unsigned char *bytes;
	size_t size;
	unsigned char *last_node_address;
	unsigned char *bytes_end;
	size_t blocks_count;
} mem_heap;

/*
 * Memory block header type
 */
typedef struct TreeNode TreeNode;
struct TreeNode {
	char status;
	char power;
	size_t size;
	unsigned char *addr;
	TreeNode *left;
	TreeNode *right;
	TreeNode *parent;
};

/*
 * List of memory block headers
 */
struct {
	//int index;
	TreeNode *start;
	//TreeNode*end;
} mem_blocks;

/*
 * Initialize heap
 */
int get_power(size_t size);
bool mem_init(size_t);
void mem_free(unsigned char* addr);
void mem_drop();
TreeNode * find_root();
TreeNode * find_node(TreeNode *node,unsigned char* addr);
TreeNode * find_best_node(size_t size, char found_power, TreeNode *root);
TreeNode* who_node_child_F(TreeNode * node);
TreeNode* who_node_child_ForS(TreeNode * node);
TreeNode * for_each_free(TreeNode * node);
TreeNode* merge_nodes(TreeNode *node);
void clearHelper(TreeNode *node);
bool is_node_child_F(TreeNode * node);
bool is_node_childs_A(TreeNode * node);
bool is_root_right_S_has_child_F(TreeNode * node);
bool is_node_child_S(TreeNode * node);
bool is_node_right_ForS(TreeNode * node);
bool is_node_neighbour_ForS(TreeNode * node);
bool is_node_neighbour_F(TreeNode * node);
TreeNode * create_node(TreeNode * n);
TreeNode * split_block(TreeNode* node);
/*
 * Free heap
 */
//void mem_drop();

/*
 * Allocate memory
 */
unsigned char *mem_alloc(size_t);


/*
 * Free memory
 */
//void mem_free(unsigned char *);


/*
 * Align given size by 2^
 */
size_t align(size_t);

#endif