#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

void *hello(void *v)
{
	char* s = (char *)v;
	size_t lungime = strlen(s);
	char* r = (char*)malloc((lungime+1) * sizeof(char));
	r[lungime] = '\0';
	for(int i = 0; i < lungime; i++)
		r[i] = s[lungime-1-i];
	printf("%s\n", r);
	free(r);
	return NULL;
}

int main(int argc, char* argv[])
{	
	if (argc != 2)
	{
		printf("Nr invalid de arg!\n");
		return EINVAL;
	}
	
	pthread_t thr;
		if (pthread_create(&thr, NULL, hello, argv[1]))		//creeaza thread
	{
		perror("Eroare creare thread!\n");
		return errno;
	}
		
	void *result;
	
	if (pthread_join(thr, &result))				//asteptare dupa thread
	{
		perror("Eroare finalizare thread!\n");
		return errno;
	}
	
	
	return 0;
}
