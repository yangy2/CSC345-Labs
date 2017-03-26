/*
Yilin Yang
CSC345-01
Lab 4 Exercise 3
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define iterations 10000
#define threadtotal 10

long incircle = 0;

pthread_mutex_t lock;

void *points(void *param)
{
	double x, y;
	
	for (long i = 0; i < iterations; i++)
	{
		x = (double)rand()/RAND_MAX;
		y = (double)rand()/RAND_MAX;

		pthread_mutex_lock(&lock);
		if (x*x + y*y <= 1)
			incircle++;
		pthread_mutex_unlock(&lock);
	}
}

int main(int argc, char **argv)
{
/*	pthread_t tid, tid2;
	pthread_create(&tid, 0, points, NULL);
	pthread_create(&tid2, 0, points, NULL);
	pthread_join(tid, NULL);
	pthread_join(tid2, NULL);
*/
	pthread_attr_t attr;
	pthread_attr_init(&attr);

	pthread_t *threads = malloc(threadtotal*sizeof(pthread_t));
	for(int i=0; i<threadtotal; i++)
		pthread_create(&threads[i], &attr, points, NULL);

	for(int i=0; i<threadtotal; i++)
		pthread_join(threads[i], NULL);


	pthread_mutex_destroy(&lock);

	printf("Monte Carlo algorithm using %d iterations.\n", iterations*threadtotal);
	printf("Estimate pi value: %f\n", (double)incircle/iterations*4/threadtotal);
}
