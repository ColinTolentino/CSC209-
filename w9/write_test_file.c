#include <stdio.h>
#include <stdlib.h>


int main(int argc, char *argv[]) {
	FILE *fp;
	int random_num;

	if(argc != 2){
		perror("Must enter a filename");
		return 1;
	}

	fp = fopen(argv[1], "wb");
	for(int i = 0; i <= 99; i++){
		random_num = rand() % 100;
		fwrite((const void*) & random_num,sizeof(int),1,fp);
	}
	fclose(fp);
	return 0;

}
