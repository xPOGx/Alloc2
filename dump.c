#include <stdio.h>
#include "allocator.h"
#include "dump.h"
#include <string.h>
void PrintTree(TreeNode* node, int indent)
{
	if (node) {
		if (node->right) PrintTree(node->right, indent + 4);
		if (indent) {
			for (int i = 0; i < indent; i++)printf(" ");
		}
		if (node->right) {
			printf("/\n");
			for (int i = 0; i < indent; i++) printf(" ");
		}
		printf("%c(%d)\n", node->status,node->size);
		if (node->left) {
			for (int i = 0; i < indent; i++) printf(" ");
			printf("\\\n");
			PrintTree(node->left, indent + 4);
		}
	}
}


void mem_dump() {
	printf("\n========= Memory Dump =========\n");
	TreeNode * node = mem_blocks.start;
	printf("Header Size: \t        ");
	printf("%d\n", sizeof(TreeNode));
	printf("Heap size: \t\t");
	printf("%d\n", mem_heap.size);
	printf("Available: \t\t");
	printf("%d\n", (size_t)mem_heap.bytes_end - (size_t)mem_heap.last_node_address);
	printf("Allocated: \t\t");
	printf("%d\n", mem_heap.size - ((size_t)mem_heap.bytes_end - (size_t)mem_heap.last_node_address));
	printf("Allocated blocks count: ");
	printf("%d\n", mem_heap.blocks_count);
	printf("Blocks info:\n\n");
	PrintTree(node, 0);

	printf("\n======= Memory Dump End =======\n");

};
