#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
	char buf[256];
	int p[2];
	pipe(p);
	if (fork() == 0)
	{	// Get the child process to recieve the ping and write it to pipe
		//printf("%d: received ping\n", getpid());
		//close(0);
		//dup(p[1]);
		//write(0, buf, 1);	
		// read from p[0]
		// close p[0]
		// print recieved
		// write to p[1]
		// close p[1]
		
		//read(p[0], buf, 1);
		if (read(p[0], buf, 1) > 0)
		{
			printf("%d: Received ping\n", getpid());
		}
		
		//close(p[0]);
		write(p[1], buf, 1);
		
	}	else
	{
	
		// a parent and child process that runs the same 

		//write to p[1]
		//close p[1]

		//close(p[1]);
		//read(p[0], "hi", 1);
		// close(p[0])	
		write(p[1], buf, 1);
		if (read(p[0], buf, 1) == 1)
		{
			printf("%d: received pong\n", getpid());				
		}
	
		
		//wr=
		
		
	}
	exit(0);
}

