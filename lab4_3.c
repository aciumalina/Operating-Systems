#include <stdio.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <stdlib.h>

void collantz(int n)
{
	printf("%d ", n);
	if (n==1)
	{	
		printf("\n");
		return;
		}

	else if (n % 2 == 0)
	{
		collantz(n/2);
	}
	else {
		collantz(3*n+1);
	}
}

int main(int argc, char *argv[])
{
	printf("Starting Parent: %d\n" ,getpid());
	for (int i = 1; i < argc; i++)
	{
		pid_t pid = fork();
		
		if (pid < 0)
		{
			printf("Couldn't fork!\n");
			return errno;
		}
		
		else if (pid == 0)
		{
			int nr = atoi(argv[i]);
			printf("%d: ", nr);
			collantz(nr);
			printf("Done! Parent %d, Me %d\n", getppid(), getpid());
			break;	
		}
		
	}
	for (int i = 1; i < argc; i++)
	{
		wait(NULL);
	}

	return 0;
}


