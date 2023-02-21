
#include <stdio.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <unistd.h>


int main()
{
	pid_t pid = fork();
	
	if (pid < 0)
	{
	
		printf("Couldn't create process\n");
		return errno;
	}
	
	else if (pid == 0)
	{
		char *argv[] = {"ls", NULL};
		execve("/usr/bin/ls", argv, NULL);
		printf("Execve failed!\n");
		perror(NULL);
	
	}
	else{
		wait(NULL);
		printf("Child %d finished\n", pid);
		printf("Parent: My PID - %d, Child PID - %d\n", getpid(), pid);
	}
	return 0;
	
}
