#include "threads.h"
#include <unistd.h>

int m = 0;
int n = 0;

void add1() {
	int i;
	for (i = 0; i < 10; i++) {
	m ++;
	printf("m++\n");
	sleep(1);
	}
}	

void add2() {
	int i;
	for (i = 0; i < 10; i++) { 
	n ++;
	printf("n++\n");
	}
}

void print() {
	printf("m: %d; n: %d\n", m, n);
}

int main() {
	create(add1);
	create(add2);
	create(add1);
	create(add2);
	join(print);
	return 0;
}
