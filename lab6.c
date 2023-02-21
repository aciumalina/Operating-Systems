#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <stdlib.h>

int x = 0;

void *hello(void *v)
{

	int *thr_id = (int *)v;
	printf("%d: Hello!\n", *thr_id);
	for(int i =0; i < 100000; i++)
		x = x + 1;
	return NULL;
}

int main(int argc, char* argv[])
{	
	if (argc != 2)
	{
		printf("Nr invalid de arg!\n");
		return EINVAL;
	}
	
	
	
	int nr_threads = strtol(argv[1], NULL, 10);
	printf("Voi crea %d fire de executie\n", nr_threads);
	
	pthread_t *thr = (pthread_t*)malloc(nr_threads*sizeof(pthread_t));
	
	int *thr_ids = (int*)malloc(nr_threads*sizeof(int));
		
	for (int i = 0; i < nr_threads; i++)
	{
		thr_ids[i] = i;
		if (pthread_create(thr+i, NULL, hello, thr_ids+i))		//creeaza thread
	{
		perror("Eroare creare thread!\n");
		return errno;
	}
	}
	
	
	
	void *result;
	
	for (int i = 0; i < nr_threads; i++)
	{
	if (pthread_join(thr[i], &result))				//asteptare dupa thread
	{
		perror("Eroare finalizare thread!\n");
		return errno;
	}
	}
	
	printf("x = %d\n", x);
	
	free(thr);
	free(thr_ids);
	
	return 0;
}
