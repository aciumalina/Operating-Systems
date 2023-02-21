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

int main(int argc, char* argv[])
{
	int n = atoi(argv[1]);

	pid_t pid = fork();
	
	if (pid < 0)
	{
		printf("Couldn't create process!\n");
		return errno;
	}
	
	else if (pid == 0)
	{
		collantz(n);
	
	}
	
	else{
		wait(NULL);
		printf("Child %d finished!\n", pid);
		printf("Parent PID = %d, Child PID = %d\n", getpid(), pid);
	}

	return 0;
}
