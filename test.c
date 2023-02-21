# include <sys/stat.h>
#include <errno.h>
#include <stdio.h>
//int stat ( const char* path , struct stat* sb );
int main(){
struct stat sb;
if (stat("text", &sb )) {
	perror ("text");
	return errno;
}
printf ("Text takes % jd bytes on disk ", sb.st_size);
}
