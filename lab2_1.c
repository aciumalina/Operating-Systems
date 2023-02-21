#include<stdio.h>
#include<unistd.h>
#include<sys/stat.h>
#include <fcntl.h>

int main()
{
	int fd;
	char msg[50] = "Hello World";
	char buffer[80];
	fd = open("text.txt", 700);
	//printf("fd = %d", fd);

if (fd != -1)
{
	//printf("\n text.txt openend with read/write\n");
	write(1, msg, sizeof(msg));  //Scriere in consola
	//lseek(fd, 0, SEEK_SET);
	//read(fd, buffer, sizeof(msg));
	//printf("\n %s was written to my file\n", buffer);
	close(fd);
}
	return 0;
}
