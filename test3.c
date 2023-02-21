#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>


int main()
{

	pid_t pid = fork();
	
	int x = 0;
	
	printf("Ceva\n");
	
	if (pid < 0)
	{
		perror("Eroare la creare proces");
		return errno;
	}
	
	else if (pid == 0)
	{
	
	//child instructions
	printf("Copil First!\n");
	printf("Copil: eu sunt %d, parinte %d\n", getpid(), getppid());
	x ++;
	printf("Copil: x = %d\n" ,x);
	
	}
	else
	{
		//parent instructions
		wait(NULL);
		printf("Parent Last!\n");
		printf("Sunt pid %d si am copilul %d\n", getpid(), pid);
		printf("Parinte: x = %d\n" ,x);
	}
	return 0;
}
