#include <stdio.h>
#include <string.h>
#include <stdlib.h>


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

	int page[pageframe]; /* Array to simulate page replacement */
	int FIFO_fault = 0;
	int LRU_fault = 0;
	int Optimal_fault = 0;

	/* Fill page array with initial values */
	for(int i = 0; i < pageframe; i++)
	{
		page[i] = num[i+1];
		FIFO_fault++;
		LRU_fault++;
		Optimal_fault++;
//		printf("Page: %d\n", page[i]);
	}

//	for(int i = pageframe + 1; i < counter; i++)
//		printf("Remain: %d\n", num[i]);
	
	int replace_index = 0;
	
	/* FIFO Replacement */
	for(int j = pageframe + 1; j < counter; j++)
	{
		int match = 0; /* 0 if no match found, 1 if match found*/

		/* compare number with contents of page array */
		for(int i = 0; i < pageframe; i++)
		{
			if(num[j] == page[i])
				match = 1;
		}
		
		/* replace contents if no match found */
		if(match == 0)
		{
			page[replace_index] = num[j];
			if(replace_index < pageframe)
				replace_index++;
			else
				replace_index = 0;
			FIFO_fault++;
		}
	}

//	for(int i = 0; i < pageframe; i++)
//		printf("Final: %d\n", page[i]);
	printf("FIFO faults: %d\n", FIFO_fault);
}
