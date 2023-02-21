#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>
#include <string.h>

#define MAX_RESOURCES 5
#define nr_threads 5

int available_resources = MAX_RESOURCES;
int resources_used[nr_threads] = {4, 2, 1, 3, 2};
int thread_positions[nr_threads];
pthread_mutex_t mtx;

int decrease_count(int count)
{
	pthread_mutex_lock(&mtx);
	
	if (available_resources < count)
	{
		pthread_mutex_unlock(&mtx);
		return -1;
	}
	else available_resources -= count;
	printf("Got %d resources, %d remaining\n", count, available_resources);
	
	pthread_mutex_unlock(&mtx);
	return 0;
}


int increase_count(int count)
{
	pthread_mutex_lock(&mtx);
	
	available_resources += count;
	printf("Released %d resources, %d remaining\n", count, available_resources);

	pthread_mutex_unlock(&mtx);
	return 0;
}


void *functie(void *argv)
{
	int thread_curent = *(int *) argv;
	if (decrease_count(resources_used[thread_curent]) == -1)
	{
		printf("Not enough resources for this thread!\n");
		//pthread_mutex_unlock(&mtx);
		return NULL;
	}
	increase_count(resources_used[thread_curent]);
	return NULL;

}


int main(int argc, char *argv[])
{
	if (pthread_mutex_init(&mtx, NULL))
	{
		perror("Eroare creare mutex!\n");
		return errno;
	}
	
	printf("MAX RESOURCES = %d\n", MAX_RESOURCES);
	pthread_t threads[nr_threads];
	
	for (int i = 0; i < nr_threads; i++)
	{
		thread_positions[i] = i;
		if (pthread_create(&threads[i], NULL, functie, &thread_positions[i]))
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
	return 0;

}
