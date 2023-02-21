#include<stdio.h>
#include<unistd.h>
#include<sys/stat.h>
#include <fcntl.h>
#include <errno.h>

int main(int argc, char *argv[])
{

struct stat sb;
if (stat(argv[1], &sb )) {
	perror ("foo");
	return errno;
}

char buffer[sb.st_size];
int fd1 = open(argv[1], O_RDWR);
int fd2 = open(argv[2], O_RDWR);

if (fd1 != -1 && fd2 != -1)
{
	read(fd1, buffer, sizeof(buffer));
	write(fd2, buffer, sizeof(buffer));
	//printf("%s", buffer);
	
	close(fd1);
	close(fd2);
}
	return 0;

}
