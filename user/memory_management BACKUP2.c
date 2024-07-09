#include "memory_management.h"

/*
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
*/

memBlock *head = NULL;

void* partition(memBlock* partitionBlock, int size)
{
	// Take in a size, scan every node until you find one with size big enough
	// if it is big enough rearrange the linked list so that there is a node of
	// sizeof (size) and then return the memory location of this node

	printf("stage 6\n");	
	memBlock *freeMemory = NULL;
	memBlock *excessMemory = NULL;	
	
	//printf("%d\n", sizeof(partitionBlock) * 2);
	//printf("%d\n", partitionBlock->size);
	//printf("%d\n", size);

//	if (partitionBlock ->size < size)
//	{
//		printf("Cannot malloc more than a pagesize at once");
//		return NULL;
//	}
	excessMemory = (memBlock*) ((void*) partitionBlock + sizeof(memBlock) + size);
	printf("stage 7\n");
	excessMemory->size = partitionBlock->size - size - (sizeof(partitionBlock) * 2);
	printf("stage 7.1\n");
	excessMemory->next = partitionBlock->next;	
	printf("stage 7.2\n");
	excessMemory->free = 0;

	printf("stage 8\n");

	freeMemory = partitionBlock;
	freeMemory->next = excessMemory;
	freeMemory->size = partitionBlock->size - excessMemory->size - (sizeof(partitionBlock) * 2);
	freeMemory->free = 1;

	printf("stage 9\n");
	
	return freeMemory;
}

void* allocateMemory()
{
	memBlock *newMemoryBlock = NULL;
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
	/*if (size > PAGESIZE - (sizeof(memBlock) * 2))	
	{
		printf("yo\n");
		size = size - PAGESIZE;
		printf("2\n");
		_malloc(PAGESIZE - (sizeof(memBlock) * 2));
		printf("3\n");
	} */
	
	memBlock *freeMemory = NULL;
	printf("stage 1\n");	
	if (size <= 0)
		return NULL;


	printf("stage 2\n");

	if (head == NULL)
	{
		printf("stage 3\n");
		head = allocateMemory();
		//printf("%d", head->size);
	}

		printf("stage 4\n");
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
	
	// Need to trace the memory blocks to find when it has no more space
	freeMemory = head; 
	while (freeMemory != NULL)
	{
		if (freeMemory->free == 0 && freeMemory->size >= size + (2 * sizeof(memBlock)))
		{
			printf("stage 5\n");
			freeMemory = partition(freeMemory, size);
			break; // Memory is available implement PARTITONING here
			printf("stage 5.1\n");
		}	
		else //if (freeMemory->next != NULL)
		{
			printf("stage 5.5\n");
			freeMemory = freeMemory->next;
		}
	}

	// Free memory is NULL when no adequate space is found as it will go to the last node, which points to NULL
	if (freeMemory == NULL)
	{
		// No valid memory block so ask for more memory and attach this to the tail
		freeMemory = head;
		while (freeMemory->next != NULL)
		{
			freeMemory = freeMemory->next;
		}

		// Here you are at the tail of the nodes of memory blocks, attach a new node to it
		freeMemory->next = allocateMemory();
		if (size + (2 * sizeof(memBlock)) < PAGESIZE)
			freeMemory = partition(freeMemory->next, size); // Partition the new block with the size requested
		else
			return NULL; // My implementation does not currently work with MALLOC larger than pagesize + headers	
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
	printf("\n\n\n\n RETURNED MEMROY SIZE %d\n\n", freeMemory->size);
	return freeMemory + 1;
}

void _free(void *ptr)
{
	printf("stage 50\n");
	if (!ptr)
		return; // Exit the function, no operation if the ptr is NULL;
	
	printf("stage 51 %d\n", ((memBlock*) ptr-1)->free);
	if (((memBlock*) ptr - 1)->free == 0)
		return; // Exit the function, the ptr is already free
	
	printf("stage 52\n");
	((memBlock*) ptr -1)->free = 0; // Set it to free if it is not already
	printf("\n\nShould be free\n");
		
	
	// Collect all the free blocks into block at the end of the list
		
	memBlock *tail = NULL;
	tail = head;	
	int counter = 0;
	int fullCounter = 0;	
	printf("no u\n");
	while (tail != NULL)
	{
		printf("y4s u \n");

		printf("final one\n");
		if (tail->free == 0)
			counter++;
		else if (tail->free == 1)
{		
	fullCounter++;
			printf("ok this is size of the whole list %d, %d \n\n\n", tail->size, tail->free);
}	
	//	printf("ok this is size of the whole list %d, %d \n\n\n", tail->size, tail->free);
		tail = tail->next;
		printf("full counter legit is %d\n", fullCounter);
	}
	printf("say sike rn\n");
	
	memBlock *freeBlocks[counter];
	memBlock *fullBlocks[fullCounter];
	
	//memBlock *tempFull = NULL;
	//tempFull = head;
	fullCounter = 0;

	memBlock *tempFree = NULL;
	tempFree = head;
	counter = 0;
	
	printf("IM IN UR WALLS \n");

	while (tempFree != NULL)
	{
		if (tempFree->free == 0)
		{
			//printf("Ok clown time %d\n\n", tempFree->size);	
			freeBlocks[counter] = tempFree;
			counter++;
		}
		
		if (tempFree->free == 1)
		{
			fullBlocks[fullCounter] = tempFree;
			fullCounter++;
		}
			
		tempFree = tempFree->next;
	}
	printf("no it was me\n");	
	for (int i = 0; i < counter; i++)
	{
		if (i == 0)
		{		
			printf("printf %d", tail->size);
			tail = freeBlocks[i];
			printf(" wass \n");
		}
		else
			freeBlocks[i]->next = freeBlocks[i+1];
		printf(" nyet \n");
		if (i == counter-1)
			freeBlocks[counter-1]->next = NULL;	
	}

	//freeBlocks[counter-1]->next = NULL;	

	printf("\n\n L variable counter is : %d\n\n", fullCounter);	
	

/*	memBlock *tempFull = NULL;
	tempFull = head;
	fullCounter = 0;
	
	while (tempFull != NULL)
	{
//		printf("\n\n L variable counter is : %d\n\n", fullCounter);	
		if (tempFull->free == 1)
		{

			printf("temp full is rNRNrnnrnnrnrnn %d\n\n", tempFull->size);
			fullBlocks[fullCounter] = tempFull;
			fullCounter++;
		}
		tempFull = tempFull->next;
	}*/

	printf("wjat is full counter %d\n\n", fullCounter);
	for (int i = 0; i < fullCounter; i++)
	{
		if (i == 0)
		{		
			printf("printf %d", tail->size);
			tail = fullBlocks[i];
			printf(" wass \n");
		}
		else
			fullBlocks[i]->next = fullBlocks[i+1];
		printf(" nyet \n");
		printf(" this is the current blocks %d \n\n", fullBlocks[i]->size);
	}

	head = fullBlocks[0];
	printf("head size %d\n\n", head->size);
	fullBlocks[fullCounter-1]->next = freeBlocks[0];	
}

int main()
{
	memBlock *testMem = NULL;
	//testMem = _malloc(500);
	//printf("TEST 2\n");
	//testMem = _malloc(4000);
	printf("TEST 3\n");	
	//testMem = _malloc(5000);
	//testMem = _malloc(1300);
	//	testMem = _malloc(4000);	
		
	testMem = testMem - 1;
	
/*	while ( testMem != NULL)
{
	printf("%d size\n", testMem->size);
	printf("%d free\n", testMem->free);
	testMem = testMem->next;
} */
	_malloc(300);
	_malloc(2999);
	_malloc(4000);
	printf("\n\n MALLOC RETURN %d \n\n" , _malloc(3000));
	testMem = _malloc(3000);
	testMem = testMem - 1;
	printf("%d MALLOC FREE \n", testMem->free);
	_free(_malloc(500));	
	//_malloc(PAGESIZE - sizeof(memBlock) * 1);
	//_malloc(9096);	
	//_malloc(PAGESIZE - sizeof(memBlock));


	testMem = head;
	while ( testMem != NULL)
{
	printf("%d size\n", testMem->size);
	printf("%d free\n", testMem->free);
	testMem = testMem->next;
}
/*
	printf("TEST FREE\n");
	_free(testMem);
	printf("hello");
	
	while ( testMem != NULL)
{
	printf("%d size\n", testMem->size);
	printf("%d free\n", testMem->free);
	testMem = testMem->next;
} */
	if (testMem != NULL)
		return 0;
	else
		return 1;	
}

