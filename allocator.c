#include <stdio.h>
#include <string.h>
#include "allocator.h"
#include <math.h>
typedef unsigned char byte;


static const char S = 'S'; //splitted 
static const char F = 'F'; //free
static const char A = 'A'; //allocated

static byte *memory; // pointer to start of allocator memory
static size_t memory_size;   // number of bytes malloc'd in memory[]

// void checkAddressRange()
bool checkAddressRange(void *_object)
{
	uintptr_t object = (uintptr_t)_object;
	if (object >= (uintptr_t)memory && object <= ((uintptr_t)memory + (uintptr_t)memory_size)) {
		//printf("it's Valid Address\n");
		return true;
	}
	else {
		fprintf(stderr, "checkAddressRange: It's Invalid Address, exit\n");
		return false;
		//exit(EXIT_FAILURE);
	}
}

bool mem_init(size_t size) {
	if (mem_heap.bytes != NULL) return 0;
	size_t size_aligned = align(size);
	mem_heap.bytes = malloc(size_aligned);
	mem_heap.last_node_address = mem_heap.bytes;
	memory = mem_heap.bytes;
	memory_size = size_aligned;
	mem_heap.size = size_aligned;
	mem_heap.bytes_end = (unsigned char *)((size_t)mem_heap.bytes + size_aligned);


	TreeNode *node =
		(TreeNode *)malloc(sizeof(TreeNode));
	node->addr = memory;
	node->parent = NULL;
	node->left = NULL;
	node->right = NULL;
	node->status = F;
	node->power = (char)get_power(size_aligned);
	node->size = size_aligned;
	mem_heap.blocks_count = 0;
	mem_blocks.start = node;
	return 1;
}

unsigned char *mem_alloc(size_t size) {
	size_t size_aligned = align(size);
	TreeNode *root = find_root();
	if (root == NULL) {
		return NULL;
	}
	if (size_aligned == root->size) return root->addr;
	if (size_aligned > root->size || mem_heap.bytes_end==mem_heap.last_node_address) {
		printf("Can not alloc size more than init\n");
		return NULL;
	}
		char found_power = (char)get_power(size_aligned);
		TreeNode *node = find_best_node(size_aligned, found_power,root);
		if (node) {
			node->status = A;
			//node->addr = malloc(size_aligned);
			mem_heap.last_node_address= (unsigned char *)((size_t)mem_heap.last_node_address + size_aligned);
			node->addr = mem_heap.last_node_address;
			//node->addr = (unsigned char *)((size_t)mem_heap.last_node_address + size_aligned);
			
			if (!checkAddressRange(node->addr)) {
				getchar();
				exit(EXIT_FAILURE);
			};
			mem_heap.blocks_count += 1;
			return node->addr;
		}
		else return NULL;
	
}

TreeNode * find_best_node(size_t size, char found_power,TreeNode *root) {
	TreeNode* node = root;
	if (!node->left && !node->right && found_power == node->power && node->status == F) {
		return node;
	}
	 else if (!node->left && !node->right && found_power == node->power && node->status == A) {
		 printf(" node not Free!!");
		 getchar();
		 return NULL;
	 }
	else if (!node->left && !node->right && found_power!= node->power) {
		node = split_block(node);
		find_best_node(size, found_power, node);
	}
	else if (node->left && node->right) {
		if (is_node_childs_A(node) && !is_node_right_ForS(mem_blocks.start)) {
			printf("Can not alloc %d bytes, need more memory\n", (int)size);
			return NULL;
		}
		 if (node->left->power != found_power && (is_node_child_S(node) || is_node_child_F(node)))
			return find_best_node(size, found_power,who_node_child_ForS(node));
		else if (node->left->power == found_power && is_node_child_F(node))
			return who_node_child_F(node);
		else {
			if (is_node_neighbour_ForS(node)) {

				return find_best_node(size, found_power, node->parent->right);
			}
			else {
				if (is_node_child_F(mem_blocks.start)) {
					find_best_node(size, found_power, who_node_child_F(mem_blocks.start));
				}
				else if (is_node_neighbour_ForS(node->parent)) {
					return find_best_node(size, found_power, node->parent->parent->right);
				}
				else if (is_root_right_S_has_child_F(mem_blocks.start)) {
					
					find_best_node(size, found_power, mem_blocks.start->right);
			
				}
				else {
					printf("Can not alloc %d bytes, need more memory\n", (int)size);
					return NULL;
				}
			}
		}
	}
}
bool is_node_child_F(TreeNode * node) {
	if (node->left->status == F || node->right->status == F) return 1;
	else return 0;
}
bool is_node_right_ForS(TreeNode * node) {
	if (node->right->status == F || node->right->status == S) return 1;
	else return 0;
}
bool is_node_childs_A(TreeNode * node) {
	if (node->left->status == A && node->right->status == A) return 1;
	else return 0;
}
bool is_root_right_S_has_child_F(TreeNode * node) {
	if (node->right->status==S && (is_node_child_F(node->right) || is_node_child_S(node->right))) return 1;
	else return 0;
}
bool is_node_neighbour_ForS(TreeNode * node) {
	if (node == node->parent->right) return 0;
	if (node->parent->right->status == F || node->parent->right->status == S) return 1;
	else return 0;
}


bool is_node_child_S(TreeNode * node) {
	if (node->left->status == S || node->right->status == S) return 1;
	else return 0;
}

TreeNode* who_node_child_F(TreeNode * node) {
	if (node->left->status == F) return node->left;
	else return node->right;
}
TreeNode* who_node_child_ForS(TreeNode * node) {
	if (node->left->status == F || node->left->status ==S) return node->left;
	else return node->right;
}

TreeNode * create_node(TreeNode * n) {
	TreeNode * node =
		(TreeNode * )malloc(sizeof(TreeNode));
	node->parent = n;
	node->addr = NULL;
	node->left = node->right = NULL;
	node->size = n->size / 2;
	node->status = F;
	node->power = (char)get_power(node->size);
	return node;
}

int get_power(size_t size) {
	return (int)(log(size) / log(2));
}

TreeNode* split_block(TreeNode* node) {
	node->status = S;
	node->left = create_node(node);
	node->right = create_node(node);
	return node;
}

TreeNode * find_root() {
	return mem_blocks.start;
}
bool is_node_neighbour_F(TreeNode* node) {
	if (node == node->parent->left) {
		if (node->parent->right->status == F) return 1;
		else return 0;
	}
	else
		if (node->parent->left->status == F) return 1;
	else return 0;
}
void mem_free(unsigned char* addr) {
	TreeNode * root = find_root();
	TreeNode *node = find_node(root,addr);
	if (node ==NULL || node->addr == NULL) return;
	//free(node->addr);
	node->addr = NULL;
	mem_heap.blocks_count -= 1;
	node->status = F;
	mem_heap.last_node_address= (unsigned char *)((size_t)mem_heap.last_node_address - node->size);
	merge_nodes(node);

}
void mem_drop(){
	if (mem_heap.bytes == NULL) {
		return;
	}
	TreeNode * root = find_root();
	for_each_free(root->left);
	clearHelper(root);
	mem_heap.bytes = NULL;
	mem_heap.last_node_address = NULL;
	mem_heap.blocks_count = 0;
	mem_blocks.start = NULL;
	root = NULL;

}

void clearHelper(TreeNode *node)
{
	if (!node) { printf("A tree already empty\n"); return; }
	if (!node->left && !node->right) {
		if (!node->parent) {
			free(mem_heap.bytes);
			//free(node->addr);
			node->addr = NULL;
			mem_heap.bytes = NULL;
		
			node->status = F;
		}
		else {
			//int tmp = node->data;
			node = node->parent;
			if (node->left ) node->left = NULL;
			if (node->right ) node->right = NULL;
			clearHelper(node);
		}
	}
	else {
		if (node->left) clearHelper(node->left);
		if (node->right) clearHelper(node->right);
	}
}

TreeNode * for_each_free(TreeNode * node) {
	if (node == NULL) return node;

	if (node->addr != NULL && node->parent!=NULL) {
		//free(node->addr);
		mem_heap.last_node_address = (unsigned char *)((size_t)mem_heap.last_node_address - node->size);

		mem_heap.blocks_count -= 1;
		node->status = F;
		node->addr = NULL;
	}
	else if (node->parent == NULL) {
		node->status = F;
	}
	TreeNode* node1 = for_each_free(node->left);

	if (node1) return node1; 

	TreeNode* node2 = for_each_free(node->right);

	return node2;
}
TreeNode* merge_nodes(TreeNode *node) {
	if (node->parent == NULL) return node;

	if (is_node_neighbour_F(node)) {
		TreeNode *new_node = node->parent;
		free(new_node->left);
		free(new_node->right);
		new_node->left = NULL;
		new_node->right = NULL;
		new_node->status = F;
		merge_nodes(new_node);
	}
	else return node;
}
TreeNode * find_node(TreeNode* node, unsigned char* addr) {
	if (node == NULL) return node;

	if (node->addr == addr) return node;

	/* then recur on left subtree */
	TreeNode* node1 = find_node(node->left, addr);

	if (node1) return node1; // node found, no need to look further 

	/* node is not found in left, so recur on right subtree */
	TreeNode* node2 = find_node(node->right, addr);

	return node2;
}

size_t align(size_t size) {
	int i = 1;
	while ((int)size > i) {
		i *= 2;
	}
	return i;
}