#include "kernel/types.h"
#include "user/user.h"

#define PAGESIZE 4096 // In bytes
#define NULL (void*) 0 // Making NULL a void ptr to 0 as XV6 treats 0 as NULLi

typedef struct memory_block {
	unsigned int size; // The amount of memory available in this block
	struct memory_block *next; // Pointer to next memBlock
	int free; // If the block is free then free = 0, if not, free = 1
} memBlock;

void* partition(memBlock* partitionBlock, int size);
void* allocateMemory();
void* _malloc(int size);
void _free(void *ptr);
