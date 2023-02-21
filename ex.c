#include <stdio.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
	int count = 1;
	printf("%d\n", count);
	for(int i = 0; i <=2; i++){
		count ++;
		int pid = fork();
		if (pid == 0)
		{
		printf("copil ");
		printf("%d\n", count);
		if (i==0 || i == 1)
		{
		
			int pid2 = fork();
			if (pid2 == 0) {
				printf("copilul lui %d (grandchild)\n", count);
			}
		}
		break;	
		}
		else{
			wait(NULL);
		}
	}
	return 0;
}
