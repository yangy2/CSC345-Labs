/*
Yilin Yang
CSC345-01
Lab 8 Exercise 1
*/

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
	unsigned long address = atoi(argv[1]);
	unsigned long page =  address >> 12;
	unsigned long offset = address & 0xfff;

	printf("Address: %lu\n", address);
	printf("Page Number: %lu\n", page);
	printf("Offset: %lu\n", offset);

}
