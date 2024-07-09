#include "memory_management.h"

// Declare and instantiate the head of the linked list used to traverse the stored memory
memBlock *head = NULL;


void* partition(memBlock* partitionBlock, int size)
{ // Parition()

	// Take in a block of memory that is big enough to store the size
	// Seperate the the freeMemory to be returned and the excess memory to add into the linked list
	// Take into account the creation of an extra header when calculating the size of partitions

	memBlock *freeMemory = NULL;
	memBlock *excessMemory = NULL;	
	
	// Set the address of the extra memory to be the address of the
	// partitionBlock + a header amount of bytes along + the byte size of the free memory block that is being returned.
	// This will position the extra memory directly after the free block in memory
	excessMemory = (memBlock*) ((void*) partitionBlock + sizeof(memBlock) + size);
	excessMemory->size = partitionBlock->size - size - (sizeof(partitionBlock) * 2);
	excessMemory->next = partitionBlock->next;	
	excessMemory->free = 0;

	freeMemory = partitionBlock;
	freeMemory->next = excessMemory;
	freeMemory->size = partitionBlock->size - excessMemory->size - (sizeof(partitionBlock) * 2);
	freeMemory->free = 1;

	return freeMemory;
} //Partition()

void* allocateMemory()
{ // AllocateMemory()

	// Creates a new memory block and then calls the sbrk system call to move the heap breakpoint, allowing for more
	// memory access which is allocated to a linked list.
	// sbrK asks for a PAGESIZE (4096) worth of bytes as this is the standard minimum of memory system calls allocate in XV6
	memBlock *newMemoryBlock = NULL;
	void *temp = sbrk(PAGESIZE);

	if (temp == (void*) -1)
		return NULL; // sbrk failed to get memory
	else 	
	{
		// The new memory block is a PAGESIZE - sizeof(memBlock) which is size of the metadata required to store
		// information about the linked list nodes or blocks of memory
		newMemoryBlock = temp;
		newMemoryBlock->size = PAGESIZE - sizeof(memBlock);
		newMemoryBlock->next = NULL;
		newMemoryBlock->free = 0;
	}

	return newMemoryBlock;
} //AllocateMemory()

void* _malloc(int size)
{ // _malloc()
	
	// Initialise freeMemory, this will be returned from _malloc
	memBlock *freeMemory = NULL;
	
	// If the user requested for no size or less than 0 return NULL
	if (size <= 0)
		return NULL;

	// The first time the program is ran head will be NULL and should be initialised	
	if (head == NULL)
	{
		head = allocateMemory();
	}

	// Need to trace the memory blocks to find when it has no more space and will require allocation of a new block
	freeMemory = head; 
	while (freeMemory != NULL)
	{
		if (freeMemory->free == 0 && freeMemory->size >= size + (2 * sizeof(memBlock)))
		{
			// If there is enough space for the requested size, partition the block to seperate free space
			freeMemory = partition(freeMemory, size);
			break;
		}	
		else
		{
			freeMemory = freeMemory->next;
		}
	}

	// Free memory is NULL when no adequate space is found as it will go to the last node, which points to NULL
	if (freeMemory == NULL)
	{
		// Search all unfit blocks of memory until you reach the tail
		freeMemory = head;
		while (freeMemory->next != NULL)
		{
			freeMemory = freeMemory->next;
		}

		// Here you are at the tail of the nodes of memory blocks, attach a new node to it
		freeMemory->next = allocateMemory();
		// My implementation does not currently work with MALLOC larger than a pagesize + node metadata
		// It will check if size can be adequatly partioned, if not it will attempt to allocate more memory
		// without any use of partitioning, this may not be enough memory.	
		if (size + (2 * sizeof(memBlock)) < PAGESIZE)
			freeMemory = partition(freeMemory->next, size); // Partition the new block with the size requested
		else 
			freeMemory->next = allocateMemory();
	}	
	
	// Return the freeMemory + 1 as this will return the address of freeMemory + an offset of ( 1 * metadata)
	// Which will be addressed directly at the memory we want to use for the process called
	return freeMemory + 1;
} //_malloc()

void _free(void *ptr)
{ // _free()
	if (!ptr)
		return; // Exit the function, no operation if the ptr is NULL;
	
	if (((memBlock*) ptr - 1)->free == 0)
		return; // Exit the function, the ptr is already free
	
	((memBlock*) ptr -1)->free = 0; // Set it to free if it is not already

	
	// --- BELOW ---
	// This section of code will take the linked list of memory blocks and make them ordered, not free -> free
	// This is to help stop internal fragmentation as once all memory locations are at the end, free blocks are merged

	memBlock *tail = NULL;
	tail = head;	
	int counter = 0; // Used to count the free memory blocks
	int fullCounter = 0; // Used to count the full memory blocks
	
	// Search through the linked list, for every memory block encountered add to its respective counter for free or not
	while (tail != NULL)
	{
		if (tail->free == 0)
			counter++;
		else if (tail->free == 1)
			fullCounter++;
		tail = tail->next;
	}

	// Initialising arrays to store each instance of the free blocks and full blocks	
	memBlock *freeBlocks[counter];
	memBlock *fullBlocks[fullCounter];

	// Resetting counters as it will search through the list again	
	counter = 0;
	fullCounter = 0;

	// Trace through the linked list and then assign any nodes to the respective array based on if they are free or not	
	tail = head;
	while (tail != NULL)
	{
		if (tail->free == 0)
		{
			freeBlocks[counter] = tail;
			counter++;
		}
		
		if (tail->free == 1)
		{
			fullBlocks[fullCounter] = tail;
			fullCounter++;
		}
			
		tail = tail->next;
	}
	
	// Trace every free memory block
	// Link the free nodes together
	// Make sure the last node points to NULL
	for (int i = 0; i < counter; i++)
	{
		freeBlocks[i]->next = freeBlocks[i+1];
	}
	
	freeBlocks[counter-1]->next = NULL;	
	
	// Trace every full memory block
	// Link the full nodes together
	for (int i = 0; i < fullCounter; i++)
	{
		fullBlocks[i]->next = fullBlocks[i+1];
	}
	
	// Set the head to the first full memory block and make the last full memory point to the first free memory
	head = fullBlocks[0];
	fullBlocks[fullCounter-1]->next = freeBlocks[0];

		
	// Merge all the free blocks into a single block now
	// Calculate the total size of the free blocks, make the first free block point to NULL now
	int freeSize = 0;
		
	for (int i = 0; i < counter ; i++)
	{
		freeSize = freeSize + freeBlocks[i]->size + sizeof(memBlock);
	}
	
	// Reduce the free size by one memory block as the initial block never got merged
	freeSize = freeSize - sizeof(memBlock); 
	freeBlocks[0]->size = freeSize;	
	freeBlocks[0]->next = NULL;	
	
} //free()
