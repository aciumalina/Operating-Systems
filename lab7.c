#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>
#include <string.h>

pthread_mutex_t mtx;
int x;	//available_resources la tema
void * thread_func(void *arg)
{

	for(int i = 0; i < 100000; i++){
		pthread_mutex_lock(&mtx);
		x = x + 1;
		pthread_mutex_unlock(&mtx);
		
	}

}

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		printf("Nr invalid de argumente!\n");
		return EINVAL;
	}
	if (pthread_mutex_init(&mtx, NULL)){
		perror("Eroar creare mutex!\n");
		return errno;
	}
	
	int nr_threads = strtol(argv[1], NULL, 10);
	pthread_t *thr = (pthread_t *)malloc(nr_threads * sizeof(pthread_t));
	
	for (int i = 0; i < nr_threads; i ++)
	{
		if (pthread_create(thr+i, NULL, thread_func, NULL))
		{
			perror("Eroare creare thread!\n");
			return errno;
		}
	}
	
	void *result;
	for (int i = 0; i < nr_threads; i++)
	{
		if(pthread_join(thr[i], &result))
		{
			perror("Eroare join!\n");
			return errno;
		}
	}
	
	free(thr);
	pthread_mutex_destroy(&mtx);
	
	printf("x = %d\n", x);
	return 0;
}
