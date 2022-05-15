#include <stdio.h>
#include <stdlib.h>

typedef data_t int

typedef struct ELE{
	data_t data;
	struct ELE *next;
} *listptr;

int main(){
	listptr start = (struct ELE*)malloc(sizeof(struct ELE));
	/*
		insert code, use linked list of primes to more quicly check for primes
		space efficiency, roughly 2 hundred thousand 8byte data structs = 1.6 mil bytes =~= 2MB of linked list elements (DAMNNNNN)
	*/
	return 0;
}
