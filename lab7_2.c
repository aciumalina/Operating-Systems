#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>
#include <string.h>
#include <semaphore.h>

#define nr_threads 5

int thread_positions[nr_threads];
int barrier_value = 0;

pthread_mutex_t mtx;
sem_t sem;

void barrier_point()
{
	pthread_mutex_lock(&mtx);
	barrier_value++;
	
	if (barrier_value == nr_threads)
	{
		pthread_mutex_unlock(&mtx);
		if (sem_post(&sem))
		{
			printf("Error: %d\n", errno);
			return;
		}
		return;
	}
	
	pthread_mutex_unlock(&mtx);
	
	if (sem_wait(&sem))
	{
		printf("Error: %d\n", errno);
		return;
	}
	
	if (sem_post(&sem))
	{
		printf("Error: %d\n", errno);
		return;
	}
	return;
}


void* tfun(void *arg)
{
	int *tid = (int*)arg;
	printf("%d reached the barrier\n", *tid);
	barrier_point();
	printf("%d passed the barrier\n", *tid);
	return NULL;
}

int main(int argc, char* argv[])
{
	if (pthread_mutex_init(&mtx, NULL))
	{
		perror("Eroare creare mutex!\n");
		return errno;
	}
	
	printf("Nr threads: %d\n", nr_threads);
	
	if (sem_init(&sem, 0, 0))
	{
		printf("Eroare initializare semafor!\n");
		return errno;
	}
	
	pthread_t threads[nr_threads];
	
	for (int i = 0; i < nr_threads; i++)
	{
		thread_positions[i] = i;
		if (pthread_create(&threads[i], NULL, tfun, &thread_positions[i]))
		{
			perror("Eroare creare thread!\n");
			return errno;
		}
	}
	
	for (int i = 0; i < nr_threads; i++)
	{
		if (pthread_join(threads[i], NULL))
		{
			perror("Eroare join!\n");
			return errno;
		}
	}
	
	pthread_mutex_destroy(&mtx);
	sem_destroy(&sem);
	return 0;
}
