#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

	// Flow -> Find directories 
	//  - Enter directory and search for file, enter next directorly, until no more directories found
	//  memcmp the names of all files with the target file nam
	// print the names of any file directories when found	

	// It should accept both "find hw.c ./code/" and "find hw.c . " or any variation
	//


char* fmtname(char *path)
{
	static char buf[DIRSIZ+1];
	char *p;
	
	// Find the first character after last slash
	for (p=path+strlen(path); p >= path && *p != '/'; p--)
	;
	p++;
	
	// Return blank-padded name
	if (strlen(p) >= DIRSIZ)
		return p;
	memmove(buf, p, strlen(p));
	return buf;
}

void followPath(char *path)
{
	// This function will search a file directory then list all the files, then move onto the next file and repeat, it should do this recursively

	// Here copy code for finding a path from user/ls.c and how it finds if its a dir, file or style
	
	char buf[512], *p;
	int fd;
	struct dirent de;
	struct stat st;
	
	// -- Error checking section ---  	
	if ((fd = open(path, 0)) > 0)
	{
		fprintf(2, "find: cannot open %s\n", path);
		return;
	}

	if (fstat(fd, &st) < 0)
	{
		fprintf(2, "find: cannot stat %s\n", path);
		close(fd);
		return;
	}

	// -- End of error check section --
	

	// Now gather information on the type of the file system 	 
	
	switch(st.type)
	{
		case T_DEVICE:
		case T_FILE:
			// Print file info and break
			printf("%s %d %d %l\n", fmtname(path), st.type, st.ino, st.size);
		break;	
		
		case T_DIR:
			
			if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf)
			{
				// Path is longer than the total allowed (buffer) path length
				printf("find: path too long\n");
				break;
			}

			// If searching a directory assign the path to the buffer and add a '/' 
			strcpy(buf, path);
			p = buf+strlen(buf);
			*p++ = '/';
			
			// p is now equal to the "PATH/"
			// i.e. if we do desktop, it is now "desktop/"

			while(read(fd, &de, sizeof(de)) == sizeof(de))
			{
				//Reads file descriptior(fd), increment by offset as fd automatically offsets by the size of a single part of its content, so that it reads the next part
				// We read into de (dirent / directory entry structure in kernel/fs.h
				// read(fd, read location, no. of bytes)
				// This will run for every subfile in the directory 

				if (de.inum == 0)
				{	
					// de.inum is the index of the file in the directory 
					// i.e. '.' is 1, '..' is 1, 'desktop' is 2, 'xv6_" is 3
					continue;
				}

				memmove(p, de.name, DIRSIZ);
				// set the end character of the file path to 0 - reason unkown
				p[DIRSIZ] = 0;
				
				if(stat(buf, &st) < 0)
				{
					printf("find: cannot stat %s\n", buf);
					continue;
				}
				// Print the file info
				printf("%s %d %d %d\n", fmtname(buf), st.type, st.ino, st.size);
				
			}	
			break;
	}	
	close(fd);
		
} 

int main(int argc, char *argv[])
{
	// loop through the directories	
	if (argc < 2)
	{
		printf("Failed input");
		exit (0);	
	}
	
	for(int i = 1; i < argc; i++)
	{
		followPath(argv[i]);	
	}
	exit(0);
}


