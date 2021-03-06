/*
Yilin Yang
CSC345-01
Lab 6 Exercise 1
*/
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define MAX_WAIT 5

typedef struct
{
	char* bound;
	int index;
} FARMER;

int globaltime = 1; //Track time
int north, south; //Number of northern/southern farmers
sem_t sem;

void enter_bridge(char* bound, int index);
void exit_bridge(char* bound, int index);

void* n_pass_bridge(void* param);
void* s_pass_bridge(void* param);


int main(int argc, char** argv)
{
	north = atoi(argv[1]);
	south = atoi(argv[2]);

	//Create array of northern farmers
	FARMER northerner[north];
	for(int i=0; i<north; i++)
	{
		northerner[i].bound = "North";
		northerner[i].index = i+1;
//		printf("%s farmer #%d\n", northerner[i].bound, northerner[i].index);
	}

	//Create array of southern farmers
	FARMER southerner[south];
	for(int i=0; i<south; i++)
	{
		southerner[i].bound = "South";
		southerner[i].index = i+1;
//		printf("%s farmer #%d\n", southerner[i].bound, southerner[i].index);
	}

	printf("%d northern farmer(s) and %d southern farmer(s).\n", north, south);

	sem_init(&sem, 0, 1);

	//Set up threads
	pthread_attr_t attr;
	pthread_attr_init(&attr);

	pthread_t *Nfarmer = malloc(north*sizeof(pthread_t));
	pthread_t *Sfarmer = malloc(south*sizeof(pthread_t));

	//Call pass_bridge based on farmer
	for(int i=0; i<north; i++)
		pthread_create(&Nfarmer[i], 0, n_pass_bridge, (void*)(intptr_t)(northerner[i].index));
	for(int i=0; i<south; i++)
		pthread_create(&Sfarmer[i], 0, s_pass_bridge, (void*)(intptr_t)(southerner[i].index));


	for(int i=0; i<north; i++)
		pthread_join(Nfarmer[i], NULL);
	for(int i=0; i<south; i++)
		pthread_join(Sfarmer[i], NULL);

	sem_destroy(&sem);

	printf("All finished!\n");

	return 0;
}

void enter_bridge(char* bound, int index)
{
	printf("M%d: %s farmer #%d will cross the bridge.\n", globaltime, bound, index);
	
	sem_wait(&sem); //lock
	globaltime++;

	printf("M%d: %s farmer #%d is crossing the bridge.\n", globaltime, bound, index);

	int timer = rand() % MAX_WAIT; //generate random delay
	globaltime++;

	printf("M%d: %d seconds until completion...\n", globaltime, timer);
	sleep(timer); //delay
}

void exit_bridge(char* bound, int index)
{
	globaltime++;
	printf("M%d: %s farmer #%d has crossed the bridge.\n", globaltime, bound, index);
	sem_post(&sem); //unlock
}

void* n_pass_bridge(void* param)
{
	int x = (intptr_t)param;
	enter_bridge("North", x);
	exit_bridge("North", x);
}

void* s_pass_bridge(void* param)
{
	int x = (intptr_t)param;
	enter_bridge("South", x);
	exit_bridge("South", x);
}
