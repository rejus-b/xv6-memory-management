#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
	printf("Total Uptime: %d\n", uptime());
	exit(0);	
}
