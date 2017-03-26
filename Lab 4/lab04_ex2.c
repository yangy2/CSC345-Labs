/*
Yilin Yang
CSC345-01
Lab 4 Exercise 2
*/

#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <semaphore.h>

int main(int argc, char** argv)
{
	sem_t sem;

	if(sem_init(&sem, 0, 1) == -1)
		printf("%s\n", strerror(errno));

	if(sem_wait(&sem) != 0)
		printf("%s\n", strerror(errno));

	printf("*** Critical Section ***\n");

	if(sem_post(&sem) != 0)
		printf("%s\n", strerror(errno));

	printf("*** Non-Critical Section ***\n");

	if(sem_destroy(&sem) != 0)
		printf("%s\n", strerror(errno));

	return 0;

}
