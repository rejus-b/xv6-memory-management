// Create a command XARGS, that converts input from std input into
// 	args to a command
//
//	e.g. 'echo hello too | xargs echo bye' should output ->
//		'bye hello too'
//
//	in regular Linux 'echo hello too | echo bye' would only output ->
//		'bye'
//
//
//
//
//
// When you use a pipe at the ned of the smnth it outputs to the std input and you can read frm the std input qwith the read comman
// once u processed the output of this you can process the fire part of the pipe
//
// XARGS takes a command based on what it is supplied, then takes the inputs from previous pipe stdinputs as additional args to the original command
//
//
// exec() kills a function after it runs, so you have to fork and run through it a child process


// You have to use multiple child processes to add the previous stdinput pipe when XARGing

#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"

int main (int argc, char** argv)
	{
		
	//printf("%d", argc);
	//for (int i = 0; i<argc; i++) {
	//	printf("%s", *(argv+i));
	//}
	//
	//
	// stdin stores in '0' which can be read from 

	
	char* command = argv[1];
	char* args[512];

	read(0, args, 512);
//	exec(command, args);		

	// The child process is going to run the echo command as it goes first, while the adult waits then XARGS the output
	if (fork() == 0)
	{
		//printf("hi");
		//printf(" %s \n", argv);

		//read(1, "previousArgs", MAXARG); 
		
		//exec(command, argv+1);	

		
		//read(0, "previousArgs", MAXARG); 
		
		exec(command, args);
		
		exit(0);
	}
	else
	{	
		//read(1, "previousArgs", MAXARG); 
		wait(0);
		// Run the command that has piped XARG into
		
		printf("This should run after the child process dies");		
			
	}

	return 0;			
}
