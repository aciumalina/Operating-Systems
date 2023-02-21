#include <stdio.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>

void collantz(long int n, int *memory_used, long int* shm_ptr)
{
	*(shm_ptr + *memory_used + 1) = n;
        (*memory_used)++;
        if (*memory_used == getpagesize()/sizeof(long int)-1)
        {
        	*(shm_ptr + *memory_used) = -1;
        	return;
        	}
        	
	if (n==1)
	{	
		return;
		}

	else if (n % 2 == 0)
	{
		collantz(n/2, memory_used, shm_ptr);
	}
	else {
		collantz(3*n+1, memory_used, shm_ptr);
	}
}

int main(int argc, char* argv[])
{
	char shm_name[] = "lab5";
	
	size_t shm_size = argc * getpagesize();
	int nr_bytes = getpagesize();
	
	int shm_fd = shm_open(shm_name, O_CREAT|O_RDWR, S_IRUSR|S_IWUSR);		//creare zona de memorie partajata
	
	if (shm_fd < 0)
	{	printf("Couldn't open!\n");
		return errno;
	}
	
	if (ftruncate(shm_fd, shm_size) == -1)	//definire dimensiune
	{
		printf("Couldn't truncate!\n");
		shm_unlink(shm_name);
		return errno;
	}
	
	printf("Start parent: %d\n", getpid());

	int memory_used = 0;
	
	for(int i = 1; i < argc; i++)
	{
	
		pid_t pid = fork();
		
		if (pid < 0)
		{
			printf("Couldn't fork!\n");
			return errno;
		}
		
		else if (pid == 0)
		{
			long int nr = atol(argv[i]);
			int nr_bytes = getpagesize();
            		long int *shm_ptr = mmap(0, nr_bytes, PROT_WRITE, MAP_SHARED, shm_fd, (i-1) * nr_bytes);	//memoria partajata este incarcata in spatiul procesului
			if(shm_ptr == MAP_FAILED)
			{
				perror(NULL);
				shm_unlink(shm_name);
				return errno;
			}
			
			printf("%ld: ", nr);
			collantz(nr, &memory_used, shm_ptr);
			shm_ptr[0] = memory_used;
                	                	
			printf("Done! Parent %d, Me %d\n", getppid(), getpid());
			munmap(shm_ptr, nr_bytes);		//dezalocare zona de memorie
			
			exit(0);	
		}
	}
	for(int i = 1; i < argc; i++){
		wait(NULL);
		
	}
	 
	for(int i = 1; i < argc; i++)
	{
		long int *shm_ptr = mmap(0,nr_bytes,PROT_READ | PROT_WRITE,MAP_SHARED,shm_fd, (i-1) * nr_bytes);
		int memorie = shm_ptr[0];
		for(int j = 1; j <= memorie; j++)	
                	{
                		printf("%ld ", shm_ptr[j]);
                	}
                printf("\n");
                munmap(shm_ptr, nr_bytes);
	}
	
	shm_unlink(shm_name);

	return 0;
}
