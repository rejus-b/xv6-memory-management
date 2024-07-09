#include "memory_management.h"

/*
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>

*/

memBlock *head = NULL;

void* partition(int size)
{
	// Take in a size, scan every node until you find one with size big enough
	// if it is big enough rearrange the linked list so that there is a node of
	// sizeof (size) and then return the memory location of this node
}

void* allocateMemory(memBlock* newMemoryBlock)
{
	void *temp = sbrk(PAGESIZE);

	if (temp == (void*) -1)
		return NULL; // sbrk failed to get memory
	else 	
	{
		newMemoryBlock = temp;
		newMemoryBlock->size = PAGESIZE - sizeof(memBlock);
		newMemoryBlock->next = NULL;
		newMemoryBlock->free = 0;
	}

	return newMemoryBlock;
}

void* _malloc(int size)
{	
	memBlock *freeMemory = NULL;
	
	if (size <= 0)
		return NULL;

	if (head == NULL)
	{
		head = allocateMemory(head);
	}
//		void *temp = sbrk(PAGESIZE);
//
//		if (temp == (void*) -1)
//			return NULL; // sbrk failed to get memory
//		else 	
//		{
//			head = temp;
//			head->size = PAGESIZE - sizeof(memBlock);
//			head->next = NULL;
//			head->free = 0;		
//		}	
//	}
	
	// Need to trace the tail to find when it has no more space
	freeMemory = head; 
	while (freeMemory != NULL)
	{
		if (freeMemory->free == 0 && freeMemory->size >= size + (2 * sizeof(memBlock)))
		{
			break; // Memory is available implement PARTITONING here
		}	
		else //if (freeMemory->next != NULL)
		{
			freeMemory = freeMemory->next;
		}
	}

	if (freeMemory == NULL)
	{
		// No valid memory block so ask for more memory and attach this to the tail
		freeMemory = head;
		while (freeMemory->next != NULL)
		{
			freeMemory = freeMemory->next;
		}

		// Here you are at the tail of the nodes of memory blocks, attach a new node to it
		freeMemory->next = allocateMemory(freeMemory->next);	
	}
//		void* temp = sbrk(PAGESIZE);
//
//		if (temp == (void*) -1)
//			return NULL; // sbrk failed to get memory
//		else 	
//		{
//			freeMemory->next = temp;
//			freeMemory->next->size = PAGESIZE - sizeof(memBlock);
//			freeMemory->next->next = NULL;
//			freeMemory->next->free = 0;
//		}
//	}


	return freeMemory;
}

void _free(void *ptr)
{
//	return NULL;
}

/*
int main()
{
	_malloc(PAGESIZE);
	return 0;
}
*/
