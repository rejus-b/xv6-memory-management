#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
	//int pid = fork();
	//printf (pid);
	int pid1 = fork();
	printf ("%d ", getpid());
	kill(pid1);
	printf ("%d ", getpid());
	exit(0);	
}

