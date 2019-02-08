#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <math.h>

int main(int argc, char *argv[]) {
	int age = 2;
	int *a= &age;
	int b = *a;
	age++;
	printf("%d\n",age);
	
}

