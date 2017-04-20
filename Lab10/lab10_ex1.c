#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int cylinder = 0; /* Record 1st number read from input.txt */
int requests = 0; /* Record 2nd number read from input.txt */
int start = 0; /* Initial starting position */
int counter = 0; /* Counter for num array index */

int FCFS(int *num, int FCFS);
int SSTF(int *numsorted, int SSTF);
int SCAN(int *numsorted, int SCAN);
int LOOK(int *numsorted, int LOOK);
int CSCAN(int *numsorted, int CSCAN);
int CLOOK(int *numsorted, int CLOOK);

int main(void)
{
	
	int num[10000]; /* Record numbers read from input.txt */
		
	/* Read input.txt */
	FILE* stream = fopen("input.txt", "r");
	char line[1024];
	while(fgets(line, 1024, stream))
	{
		char* tmp = strdup(line);
		char* tok;

		for(tok = strtok(tmp, " "); tok && &tok; tok = strtok(NULL, " \n"))
		{
//			printf("%s\n", tok);
			/* Read first number as range of cylinder */
			if(cylinder == 0)
				cylinder = atoi(tok);
			
			/* Fill num array with contents of file */
			else
			{
				/* Read second number as # of lines */
				/* Do not store in array */
				if(requests == 0)
					requests = atoi(tok);

				/* Store all other numbers in array */
				else
				{
					num[counter] = atoi(tok);
					counter++;
				}
			}
				
		}
	}
	start = num[counter - 1]; /* Read last number as start position */
	
/* General Note:
   num array should list up to counter-1
   because the last index contains the 
   the start position, not a data point */

//	printf("Cylinder: %d\n", cylinder);
//	printf("Requests: %d\n", requests);
//	printf("Start: %d\n", start);
//	for(int i = 0; i < counter-1; i++)
//		printf("Content: %d\n", num[i]);

	/* Copy array into new array */
	int numsorted[counter];
	for(int i=0; i<counter; i++)
		numsorted[i] = num[i];

	/* Sort new array */
	for(int i=0; i<counter-1; i++)
	{
		for(int j=i+1; j<counter-1; j++)
		{
			int x;
			if(numsorted[i] > numsorted[j])
			{
				x = numsorted[i];
				numsorted[i] = numsorted[j];
				numsorted[j] = x;
			}
		}
	}
//	for(int i=0; i<counter-1; i++)
//		printf("Sorted: %d\n", numsorted[i]);

	/* Algorithm head movement */
	int FCFS_cylinders = 0;
	int SSTF_cylinders = 0;
	int SCAN_cylinders = 0;
	int CSCAN_cylinders = 0;
	int LOOK_cylinders = 0;
	int CLOOK_cylinders = 0;

	FCFS_cylinders = FCFS(num, FCFS_cylinders);
	SSTF_cylinders = SSTF(numsorted, SSTF_cylinders);
	SCAN_cylinders = SCAN(numsorted, SCAN_cylinders);
	LOOK_cylinders = LOOK(numsorted, LOOK_cylinders);
	CSCAN_cylinders = CSCAN(numsorted, CSCAN_cylinders);
	CLOOK_cylinders = CLOOK(numsorted, CLOOK_cylinders);

	printf("FCFS: %d\n", FCFS_cylinders);
	printf("SSTF: %d\n", SSTF_cylinders);
	printf("SCAN: %d\n", SCAN_cylinders);
	printf("LOOK: %d\n", LOOK_cylinders);
	printf("CSCAN: %d\n", CSCAN_cylinders);
	printf("CLOOK: %d\n", CLOOK_cylinders);
}

int FCFS(int *num, int FCFS)
{
	/* FCFS Algorithm */
	FCFS = abs(start-num[0]);
//	printf("Head: %d\n", FCFS);

	for(int i=0; i<counter-2; i++)
	{
		FCFS = FCFS + abs(num[i]-num[i+1]);
//		printf("Head: %d\n", abs(num[i]-num[i+1]));
//		printf("abs(%d - %d)\n", num[i], num[i+1]);
	}

	return FCFS;
}

int SSTF(int *numsorted, int SSTF)
{
	/* SSTF variables */
	int head = start;
	int min = 0;
	int max = 0;
	int min_index = 0;
	int max_index = 0;
	int min_found = 0;
	int max_found = 0;
	
	int alreadychecked[counter];
	for(int i=0; i<counter; i++)
		alreadychecked[i] = 0;

	/* SSTF Algorithm */
	for(int i=0; i<counter-1; i++)
	{
//		printf("--- %d ---\n", i);

		for(int j=0; j<counter-1; j++)
		{
//			printf("Num: %d, Checked: %d\n", numsorted[j], alreadychecked[j]);
			/* Find largest point less than head */
			if(numsorted[j] < head && alreadychecked[j] != 1)
			{
				min = numsorted[j];
				min_index = j;
			}

			/* Find smallest point greater than head */
			else if(numsorted[j] > head && alreadychecked[j] != 1)
			{

				max = numsorted[j];
				max_index = j;

				break;
			}
		}

//		printf("Head: %d, Compare %d and %d\n", head, min, max);
//		printf("Min: %d vs Max: %d\n", abs(head-min), abs(head-max));
		
		/* If min was not updated */
		/* (occurs if all data points less than head used already */
		if(min == head || min_found == 1)
		{
			min_found = 1;
			SSTF = SSTF + abs(head-max);
			head = max;
			alreadychecked[max_index] = 1;
//			printf("New Head: %d\n", max);
//			printf("A: %d has been checked\n", max);
		}

		/* If max was not updated */
		/* (occurs if all data points greater than head used already */
		else if(max == head || max_found == 1)
		{
			max_found = 1;
			SSTF = SSTF + abs(head-min);
			head = min;
			alreadychecked[min_index] = 1;
//			printf("New Head: %d\n", min);
//			printf("B: %d has been checked\n", min);
		}
				
		/* If seek time to min is faster */
		else if(abs(head-min) < abs(head-max))
		{
			SSTF = SSTF + abs(head-min);
			head = min;
			alreadychecked[min_index] = 1;
//			printf("New Head: %d\n", min);
//			printf("C: %d has been checked\n", min);
		}

		/* If seek time to max is faster */
		else
		{
			SSTF = SSTF + abs(head-max);
			head = max;
			alreadychecked[max_index] = 1;
//			printf("New Head: %d\n", max);
//			printf("D: %d has been checked\n", max);
		}
	}

	return SSTF;
}

int SCAN(int *numsorted, int SCAN)
{
	/* SCAN variables */
	int min = 0;
	int max = 0;
	int min_index = 0;
	int max_index = 0;
	
	for(int i=0; i<counter-1; i++)
	{
		/* Find largest point less than start */
		if(numsorted[i] < start)
		{
			min = numsorted[i];
			min_index = i;
		}

		/* Find smallest point greater than start */
		else if(numsorted[i] > start)
		{
			max = numsorted[i];
			max_index = i;
			break;
		}
	}

	/* SCAN down then up */
	if(start < cylinder/2)
	{
		/* Cylinders from start to nearest point */
		SCAN = abs(start-numsorted[min_index]);

		/* SCAN one way */
		for(int i=min_index; i>0; i--)
		{
			SCAN = SCAN + abs(numsorted[i]-numsorted[i-1]);
		}

		SCAN = SCAN + numsorted[0]-0; /* Cylinders from first point to 0 */
		SCAN = SCAN + numsorted[max_index]; /* Cylinders in opposite direction */

		/* SCAN opposite direction */
		for(int i=max_index; i<counter-2; i++)
		{
			SCAN = SCAN + abs(numsorted[i]-numsorted[i+1]);
		}
	}

	/* SCAN up then down */
	else
	{
		/* Cylinders from start to nearest point */
		SCAN = abs(start-numsorted[max_index]);

		/* SCAN one way */
		for(int i=max_index; i<counter-2; i++)
		{
			SCAN = SCAN + abs(numsorted[i]-numsorted[i+1]);
		}

		SCAN = SCAN + cylinder-1-numsorted[counter-2]; /* Cylinders from last point to top */
		SCAN = SCAN + numsorted[min_index]; /* Cylinders in opposite direction */

		/* SCAN opposite direction */
		for(int i=min_index; i>0; i--)
		{
			SCAN = SCAN + abs(numsorted[i]-numsorted[i-1]);
		}
	}
	
	return SCAN;
}

int LOOK(int *numsorted, int LOOK)
{
	/* LOOK variables */
	int min = 0;
	int max = 0;
	int min_index = 0;
	int max_index = 0;

	for(int i=0; i<counter-1; i++)
	{
		/* Find largest point less than start */
		if(numsorted[i] < start)
		{
			min = numsorted[i];
			min_index = i;
		}

		/* Find smallest point greater than start */
		else if(numsorted[i] > start)
		{
			max = numsorted[i];
			max_index = i;
			break;
		}
	}

	/* LOOK down then up */
	if(start < cylinder/2)
	{
		/* Cylinders from start to nearest point */
		LOOK = abs(start-numsorted[min_index]);

		/* LOOK one way */
		for(int i=min_index; i>0; i--)
		{
			LOOK = LOOK + abs(numsorted[i]-numsorted[i-1]);
		}

		LOOK = LOOK + abs(numsorted[0]-numsorted[max_index]);

		/* LOOK opposite direction */
		for(int i=max_index; i<counter-2; i++)
		{
			LOOK = LOOK + abs(numsorted[i]-numsorted[i+1]);
		}
	}

	/* LOOK up then down */
	else
	{
		/* Cylinders from start to nearest point */
		LOOK = abs(start-numsorted[max_index]);

		/* LOOK one way */
		for(int i=max_index; i<counter-2; i++)
		{
			LOOK = LOOK + abs(numsorted[i]-numsorted[i+1]);
		}

		LOOK = LOOK + abs(numsorted[counter-2]-numsorted[min_index]);

		/* LOOK opposite direction */
		for(int i=min_index; i>0; i--)
		{
			LOOK = LOOK + abs(numsorted[i]-numsorted[i-1]);
		}
	}

	return LOOK;
}

int CSCAN(int *numsorted, int CSCAN)
{
	/* CSCAN variables */
	int min = 0;
	int max = 0;
	int min_index = 0;
	int max_index = 0;

	for(int i=0; i<counter-1; i++)
	{
		/* Find largest point less than start */
		if(numsorted[i] < start)
		{
			min = numsorted[i];
			min_index = i;
		}

		/* Find smallest point greater than start */
		else if(numsorted[i] > start)
		{
			max = numsorted[i];
			max_index = i;
			break;
		}
	}

	/* CSCAN down */
	if(start < cylinder/2)
	{
		/* Cylinders from start to nearest point */
		CSCAN = abs(start-numsorted[min_index]);

		/* CSCAN one way */
		for(int i=min_index; i>0; i--)
		{
			CSCAN = CSCAN + abs(numsorted[i]-numsorted[i-1]);
		}

		CSCAN = CSCAN + numsorted[0]-0; /* Cylinders from first point to 0 */
		CSCAN = CSCAN + (cylinder-1-numsorted[counter-2]); /* Cylinders from top to last point */

		/* CSCAN from opposite end */
		for(int i=counter-2; i>max_index; i--)
		{
			CSCAN = CSCAN + abs(numsorted[i]-numsorted[i-1]);
		}
	}

	/* CSCAN up */
	else
	{
		/* Cylinders from start to nearest point */
		CSCAN = abs(start-numsorted[max_index]);

		/* CSCAN one way */
		for(int i=max_index; i<counter-2; i++)
		{
			CSCAN = CSCAN + abs(numsorted[i]-numsorted[i+1]);
		}

		CSCAN = CSCAN + abs(numsorted[counter-2]-cylinder-1); /* Cylinders from last point to top */
		CSCAN = CSCAN + abs(0-numsorted[0]); /* Cylinders from 0 to first point */

		/* CSCAN from opposite end */
		for(int i=0; i<min_index; i++)
		{
			CSCAN = CSCAN + abs(numsorted[i]-numsorted[i+1]);
		}
	}	

	return CSCAN;
}
int CLOOK(int *numsorted, int CLOOK)
{
	/* CLOOK variables */
	int min = 0;
	int max = 0;
	int min_index = 0;
	int max_index = 0;

	for(int i=0; i<counter-1; i++)
	{
		/* Find largest point less than start */
		if(numsorted[i] < start)
		{
			min = numsorted[i];
			min_index = i;
		}

		/* Find smallest point greater than start */
		else if(numsorted[i] > start)
		{
			max = numsorted[i];
			max_index = i;
			break;
		}
	}

	/* CLOOK down */
	if(start < cylinder/2)
	{
		/* Cylinders from start to nearest point */
		CLOOK = CLOOK + abs(start-numsorted[min_index]);

		/* CLOOK one way */
		for(int i=min_index; i>0; i--)
		{
			CLOOK = CLOOK + abs(numsorted[i]-numsorted[i-1]);
		}

		/* CLOOK opposite direction */
		for(int i=counter-2; i>min_index+1; i--)
		{
			CLOOK = CLOOK + abs(numsorted[i]-numsorted[i-1]);
		}
	}

	/* CLOOK up */
	else
	{
		/* Cylinders from start to nearest point */
		CLOOK = CLOOK + abs(start-numsorted[max_index]);

		/* CLOOK one way */
		for(int i=max_index; i<counter-2; i++)
		{
			CLOOK = CLOOK + abs(numsorted[i]-numsorted[i+1]);
		}

		/* CLOOK from opposite end */
		for(int i=0; i<min_index; i++)
		{
			CLOOK = CLOOK + abs(numsorted[i]-numsorted[i+1]);
		}
	}	

	return CLOOK;
}
