all:
	gcc -c *c
	gcc -o lab04_ex1 lab04_ex1.c -lpthread
	gcc -o lab04_ex2 lab04_ex2.c -lpthread
	gcc -o lab04_ex3 lab04_ex3.c -lpthread
clean:
	rm lab04_ex1 lab04_ex2 lab04_ex3 *o
