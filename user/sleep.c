
#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[])
{

	// If no input
	if (argc == 1)
	{
		printf("Invalid input");
		exit(1);	
	}

	// Convert the time from a string to a number
	int time = atoi(argv[1]);	
	sleep(time);
	
	exit(0);

}

