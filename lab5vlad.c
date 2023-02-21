#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/syscall.h>

int String_to_Number(char *s)
{
    int x = 0;
    for (int i = 0; i < strlen(s); i++)
    {
        x = x * 10 + s[i] - '0';
    }
    return x;
}

int main(int argc, void **argv)
{
    //printf("%s",s);
    //printf("%d: ",nr_collatz);
    char shm_name[] = "shm_collatz";
    int shm_fd;
    shm_fd = shm_open(shm_name, O_CREAT|O_RDWR, S_IRUSR|S_IWUSR);
    if(shm_fd < 0)
    {
        perror(NULL);
        return errno;
    }
    size_t shm_size = getpagesize() * argc;
    if(ftruncate(shm_fd, shm_size) == -1)
    {
        perror(NULL);
        shm_unlink(shm_name);
        return errno;
    }
    pid_t pids[argc];
    for (int i = 1; i <= argc; i++)
    {
        char *s = argv[i];
        int nr_collatz = String_to_Number(s);
        pids[i] = fork();
        if (pids[i] < 0)
        {
            return errno;
        }
        else if (pids[i] == 0)
        {
            int nr_bytes = getpagesize();
            int *shm_ptr = mmap(0,nr_bytes,PROT_READ | PROT_WRITE,MAP_SHARED,shm_fd,(i - 1) * nr_bytes);
            if(shm_ptr == MAP_FAILED)
            {
                perror(NULL);
                shm_unlink(shm_name);
                return errno;
            }
            printf("%d: ", nr_collatz);
            int memory_used = 0;
            while (nr_collatz != 1)
            {
                if (nr_collatz % 2 == 0)
                {
                    nr_collatz = nr_collatz / 2;
                }
                else
                {
                    nr_collatz = nr_collatz * 3 + 1;
                }
                //printf("%d ", nr_collatz);
                *(shm_ptr + memory_used) = nr_collatz;
                memory_used += 4;
            }
            
            for(int j = 0 ; j < memory_used ; j = j + 4)
                printf("%d ", shm_ptr[j]);
            printf("\n");
            perror(NULL);
        }
        else
        {
            printf("Done Parent %d Me %d \n", getppid(), wait(NULL));
            return 0;
        }
    }
    int *shm_ptr = mmap(0,shm_size,PROT_READ | PROT_WRITE,MAP_SHARED,shm_fd,0);
    for(int i = 1 ; i <= argc ;i++)
    {
        munmap(shm_ptr,getpagesize());
    }
    munmap(shm_ptr,shm_size);
    shm_unlink(shm_name);
    return 0;
}
