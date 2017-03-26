/*
Yilin Yang
CSC345-01
Lab 3 Exercise 2
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define iterations 10000

long incircle = 0;

void *points(void *param)
{
	double x, y;
	
	for (long i = 0; i < iterations; i++)
	{
		x = (double)rand()/RAND_MAX;
		y = (double)rand()/RAND_MAX;

		if (x*x + y*y <= 1)
			incircle++;
	}
}

int main(int argc, char **argv)
{
	pthread_t tid;
	pthread_create(&tid, 0, points, NULL);
	pthread_join(tid, NULL);

	printf("Monte Carlo algorithm using %d iterations.\n", iterations);
	printf("Estimate pi value: %f\n", (double)incircle/iterations*4);
}
