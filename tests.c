#include "tests.h"
#include "allocator.h"
#include <assert.h>
#include <stdbool.h>

static void mem_alloc_init_test()
{
	printf("Mem_alloc_init testing...\n");
	bool first_init = mem_init(1000);
	bool second_init = mem_init(1000);
	assert(first_init != second_init);
	printf("Mem_alloc_init passed!\n");
	mem_drop();
};

static void mem_alloc_test()
{
	printf("Mem_alloc testing...\n");
	assert(mem_alloc(100) == NULL);
	printf("alloc without init passed!\n");

	mem_init(100);
	assert(mem_alloc(200) == NULL);
	printf("Not enough space passed!\n");
	mem_drop();

	mem_init(400);
	assert(mem_alloc(100) != NULL);
	assert(mem_alloc(100) != NULL);
	assert(mem_alloc(100) != NULL);
	assert(mem_alloc(100) != NULL);
	printf("Successful creation!\n");
	mem_drop();
};

static void mem_free_test()
{
	mem_init(2048);
	mem_alloc(256);
	mem_alloc(128);
	unsigned char *a1 = mem_alloc(128);
	unsigned char *a2 = mem_alloc(256);
	unsigned char *a3 = mem_alloc(512);
	mem_alloc(128);
	int count_before = mem_heap.blocks_count;
	mem_free(a2);
	mem_free(a1);
	mem_free(a3);
	assert(mem_heap.blocks_count == count_before-3);
	printf("Memory free Successful!\n");
	mem_drop();
};

static void mem_example_test() {
	mem_init(2024);

	unsigned char* a1 = mem_alloc(512);
	mem_alloc(256);
	unsigned char* a2 = mem_alloc(64);
	unsigned char* a4 = mem_alloc(256);
	mem_alloc(512);
	mem_alloc(128);
	unsigned char* a6 = mem_alloc(128);

	mem_free(a1);
	mem_free(a2);
	mem_free(a4);
	mem_free(a6);
	mem_dump();
};

void mem_run_tests()
{
	mem_alloc_init_test();
	mem_alloc_test();
	mem_free_test();
	mem_example_test();
};
