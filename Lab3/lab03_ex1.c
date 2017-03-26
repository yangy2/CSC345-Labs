/*
Yilin Yang
CSC345-1
Lab 3 Exercise 1
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int len = 7;
int nums[7] = {90, 81, 78, 95, 79, 72, 85};
int r_min, r_avg, r_max, r_med, r_std = 0;
void *myMin (void *param)
{

	r_min = nums[0];
	for (int i=0; i < len; ++i)
	{
		if (r_min > nums[i])
			r_min = nums[i];
	}
}

void *myAvgStd (void *param)
{
	int var = 0;

	for (int i=0; i < len; ++i)
		r_avg = r_avg + nums[i];
	r_avg = r_avg / len;

	for (int j=0; j < len; ++j)
		var = var + pow((nums[j] - r_avg), 2);
	var = var / len;
	r_std = sqrt(var);
}

void *myMax (void *param)
{
	r_max = nums[0];
	for (int i=0; i < len; ++i)
	{
		if (r_max < nums[i])
			r_max = nums[i];
	}
}

void *myMed (void *param)
{
	int swap;
	for(int i=0; i < len-1; ++i)
	{
		for(int j=i+1; j < len; ++j)
		{
			if(nums[j] < nums[i])
			{
				swap = nums[i];
				nums[i] = nums[j];
				nums[j] = swap;
			}
		}
	}

	if(len%2 == 0)
		r_med = (nums[len/2] + nums[len/2 + 1])/2;
	else
		r_med = nums[len/2];
}

int main (int argc, char** argv)
{
	pthread_t tid;

	pthread_create (&tid, 0, myMin, NULL);
	pthread_join (tid, NULL);
	printf("Min = %d\n", r_min);

	pthread_create (&tid, 0, myAvgStd, NULL);
	pthread_join (tid, NULL);
	printf("Avg = %d\n", r_avg);
	printf("Std = %d\n", r_std);
	
	pthread_create(&tid, 0, myMax, NULL);
	pthread_join (tid, NULL);
	printf("Max = %d\n", r_max);

	pthread_create(&tid, 0, myMed, NULL);
	pthread_join (tid, NULL);
	printf("Med = %d\n", r_med);


	return 0;
}
