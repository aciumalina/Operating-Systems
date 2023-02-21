#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

#define m 2
#define p 4
#define n 3


int m1[m][p] = {
	{10,11,12,13},
	{14,15,16,17}
};

int m2[p][n] = {
	{1,2,3},
	{4,5,6},
	{7,8,9},
	{10,11,12}
};

int produs[m][n];
	
void *inmultire(void *arg)
{
	int rez = 0;
	int i = ((int*)arg)[0];
	int j =((int*)arg)[1];
	for(int q = 0; q < p; q++)
		rez += m1[i][q] * m2[q][j];
	produs[i][j] = rez;
	free(arg);
	return NULL;
}

int main(int argc, char* argv[])
{		
	pthread_t *thr = (pthread_t*)malloc(m*n*sizeof(pthread_t));
	int k = 0;

	for (int i = 0; i < m; i++)
	{
		for(int j = 0; j < n; j++)
		{
		void *arg = malloc(2);
		((int*)arg)[0] = i;
		((int*)arg)[1] = j;
		if (pthread_create(&thr[k++], NULL, inmultire, arg))		
	{
		perror("Eroare creare thread!\n");
		return errno;
	}
	
		}	
	}
			
	void *result;
	
	for (int i = 0; i < k; i++)
	{
	if (pthread_join(thr[i], &result))				
	{
		perror("Eroare finalizare thread!\n");
		return errno;
	}
	}
	for(int i = 0; i < m; i++)
	{
		for(int j = 0; j < n; j++)
		{
			printf("%d\n", produs[i][j]);
		}
	}
	
	free(thr);
	return 0;
}
