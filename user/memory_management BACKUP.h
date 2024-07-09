#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

#define PAGESIZE 4096 // In bytes
#define NULL (void*) 0 // Making NULL a void ptr to 0 as XV6 treats 0 as NULLi

void* _malloc(int size);
void _free(void *ptr);

typedef struct memory_block {
	unsigned int size; // The amount of memory available in this block
	struct memory_block *next; // Pointer to next memBlock
	int free; // If the block is free then free = 0, if not, free = 1
} memBlock;
