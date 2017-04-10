/*
 Yilin Yang
 CSC345-01
 Lab 9 Exercise 1
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int replace_index = 0;

/* Fill page array with placeholder values */
/* Called prior to execution of each replacement algorithm */
int refresh(int *page, int pageframe)
{
	for(int i = 0; i < pageframe; i++)
		page[i] = -1;

	replace_index = 0;
}

int main(void)
{
	int pageframe = 0; /* Record 1st number read from input.txt */
	int counter = 0; /* Counter for num array index */
	int num[100]; /* Record numbers read from input.txt */

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
			/* Read first number as # of page frames */
			if(pageframe == 0)
			{
				pageframe = atoi(tok);
			}
			/* Fill num array with contents of file */
			if(pageframe != 0)
			{
				num[counter] = atoi(tok);
				counter++;
			}
				
		}
	}
//	for(int i = 0; i < counter; i++)
//		printf("Array: %d\n", num[i]);

	/* General Variables */
	int page[pageframe]; /* Array to simulate page replacement */
	int FIFO_fault = 0;
	int LRU_fault = 0;
	int Optimal_fault = 0;

	refresh(page, pageframe); /* Clear page array */
	
	/* FIFO Replacement */
	for(int j = 1; j < counter; j++)
	{
		int match = 0; /* 0 if no match found, 1 if match found*/
//		printf("%d, %d, %d\n", page[0], page[1], page[2]);

		/* compare number with contents of page array */
		for(int i = 0; i < pageframe; i++)
		{
//			printf("Comparing %d with %d\n", num[j], page[i]);
			if(num[j] == page[i])
				match = 1;
		}

		/* replace contents if no match found */
		if(match == 0)
		{
			page[replace_index] = num[j];
			
//			printf("Inserting %d into index %d\n", num[j], replace_index);

			if(replace_index < pageframe-1)
				replace_index++;
			else
				replace_index = 0;

//			printf("Index: %d\n", replace_index);
			FIFO_fault++;
		}
	}

	refresh(page, pageframe);

	/* LRU Variables */
	int LRU[pageframe]; /* Array tracking index hits */
	/* set 1st index to be least recently used */
	for(int i = 0; i < pageframe; i++)
		LRU[i] = i;

	/* LRU Replacement */
	for(int j = 1; j < counter; j++)
	{
		int match = 0; /* 0 if no match found, 1 if match found*/
//		printf("%d, %d, %d\n", page[0], page[1], page[2]);

		/* compare number with contents of page array */
		for(int i = 0; i < pageframe; i++)
		{
//			printf("Comparing %d with %d\n", num[j], page[i]);
			if(num[j] == page[i])
			{
				match = 1;

				/* decrement all index access time */
				for(int k = 0; k < pageframe; k++)
					LRU[k]--;
				LRU[i] = pageframe-1; /* set current index to most recently used */
			}
		}

		/* replace contents if no match found */
		if(match == 0)
		{
			/* find least recently used index */
			replace_index = 0;
			int min = 0;
			for(int i = 0; i < pageframe; i++)
			{
//				printf("LRU: %d\n", LRU[i]);

//				printf("Comparing %d with %d\n", replace_index, LRU[i]);
				if(replace_index >= LRU[i])
					min = i;
			}
			replace_index = min;
//			printf("The least recently used index is %d\n", replace_index);
			
			page[replace_index] = num[j];

			/* decrement all index access time */
			for(int k = 0; k < pageframe; k++)
				LRU[k]--;
			LRU[replace_index] = pageframe-1; /* set current index to most recently used*/

//			printf("Inserting %d into index %d\n", num[j], replace_index);

//			printf("Index: %d\n", replace_index);
			LRU_fault++;
		}
	}

	refresh(page, pageframe); /* Clear page array */
	
	/* Optimal Replacement Variables */
	int Optimization[pageframe]; /* Array tracking future element use */

	/* Optimal Replacement */
	for(int j = 1; j < counter; j++)
	{
		/* Reset Optimization array */
		for(int i = 0; i < pageframe; i++)
			Optimization[i] = 0;

		int match = 0; /* 0 if no match found, 1 if match found*/
//		printf("%d, %d, %d\n", page[0], page[1], page[2]);

		/* compare number with contents of page array */
		for(int i = 0; i < pageframe; i++)
		{
//			printf("Comparing %d with %d\n", num[j], page[i]);
			if(num[j] == page[i])
				match = 1;
		}

		/* replace contents if no match found */
		if(match == 0)
		{
			/* Find when each page will be reused (if at all) */
			for(int i = 0; i < pageframe; i++)
			{
//				printf("--- Page Index %d ---\n", i);

				/* Look for future instances of page */
				for(int k = j; k < counter; k++)
				{
					Optimization[i]++; /* Greater value means it is used farther in the future */

					/* Found future instance of page */
					if(page[i] == num[k])
					{
//						printf("Element %d will be used again in (%d) indices\n", page[i], Optimization[i]);
						break;
					}
					/* Did not find future instance of page */
					else if (k == counter-1)
					{
						Optimization[i]++;
//						printf("Element %d will not be used again (%d)\n", page[i], Optimization[i]);
					}
				}
			}

			/* find replacement index */
			replace_index = 0;
			int max = 0;
			for(int i = 0; i < pageframe; i++)
			{
				/* Find page to be used farthest in the future */
				/* FIFO if there are ties */
				if(max < Optimization[i])
				{
//					printf("%d is less than %d\n", max, Optimization[i]);
					max = Optimization[i];
					replace_index = i;
				}

				/* Special case in the beginning when the page array is not fully occupied */
				/* If an initial page is never used again, then the next page entry will */
				/* overwrite it despite there still being vacancies due to FIFO logic */

				/* If two or more pages are equally farthest in the future */
				else if(max == Optimization[i])
				{
//					printf("%d equals %d\n", max, Optimization[i]);

					/* Check if the previous index is occupied */
					/* If it is, it is safe to overwrite the current index */
					/* Otherwise we cannot overwrite the current index */
					/* because the prior index is still vacant */
					if(page[i-1] != -1)
					{
//						printf("Previous index [%d] contains %d\n", i-1, page[i-1]);
						replace_index = i;
					}
				}
			}

//			printf("The replacement index is %d\n", replace_index);

			page[replace_index] = num[j];
			
//			printf("Inserting %d into index %d\n", num[j], replace_index);

//			printf("Index: %d\n", replace_index);
			Optimal_fault++;
		}
	}

	/* Print results to terminal */
	printf("FIFO faults: %d\n", FIFO_fault);
	printf("LRU faults: %d\n", LRU_fault);
	printf("Optimal faults: %d\n", Optimal_fault);

	/* Print results to output.txt file */
	FILE *output = fopen("output.txt", "w");
	fprintf(output, "FIFO faults: %d\n", FIFO_fault);
	fprintf(output, "LRU faults: %d\n", LRU_fault);
	fprintf(output, "Optimal faults: %d\n", Optimal_fault);
	fclose(output);
}
