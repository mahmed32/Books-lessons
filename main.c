/* 
* Created by:
    ___                                      _   __                         
   /   |  ____ ___  ____ ___  ____ ______   / | / /___ ______________  _____
  / /| | / __ `__ \/ __ `__ \/ __ `/ ___/  /  |/ / __ `/ ___/ ___/ _ \/ ___/
 / ___ |/ / / / / / / / / / / /_/ / /     / /|  / /_/ (__  |__  )  __/ /    
/_/  |_/_/ /_/ /_/_/ /_/ /_/\__,_/_/     /_/ |_/\__,_/____/____/\___/_/   
	on 30/10/21
*/

#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef char BOOL;

/*
** The second parameter is used to identify if the caller is a child or
** a grandchild. The first parameter is ignored when sender is a grandchild.
** In the case of child the orderOfCreation is the order in which the child 
** process is created by its parent process.
*/
void printState(int orderOfCreation, char *sender)
{
	/* strcmp() return 0 if equal */
	BOOL isChild = !strcmp(sender, "child");

	printf("I am the %s ",
		isChild ? "child": "grandchild");
	
	if( isChild )
		printf("number %d ", orderOfCreation); 

	printf("and my pid is %d "
		   "and my parent pid is %d\n",
		    getpid(),
		    getppid());

	return;
}

int main(int argc, char** argv)
{
	int i, status, nOfChilds;


	if( argc != 2 )
	{
		fprintf(stderr, "Usage: %s nOfChilds\n", argv[0]);
		return -1;
	}

	nOfChilds = atoi(argv[1]);
	if(!isdigit(nOfChilds))
		fprintf(stderr, "Invaild Input or number of childs is zero\n");

	for( i = 0; i < nOfChilds ; ++i)
	{
		if( fork() == 0 )
		{
			printState(i+1, "child");
			if( fork() == 0)
			{
				printState(0, "grandchild");
				_exit( i + 1 ); /* exit with the same code as its parent */
			}
			wait(&status);
			if(WIFEXITED(status))
				printf("A grandchild created by child# %d "
					   "has exited with a state : %d\n", i+1,WEXITSTATUS(status));
			_exit(i + 1);
		}
	}

	for( i = 0; i < nOfChilds; ++i)
	{
		wait(&status);
		if(WIFEXITED(status))
			printf("A child has exited with a state : %d\n", WEXITSTATUS(status));
	}

	return 0;
}

