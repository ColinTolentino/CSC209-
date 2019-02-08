#include <stdio.h>

int main(){
	char first[11];
	int second;

	while(scanf("%s %d", first, &second) != EOF){


		if(second == 0){
			printf("%s\n",first);
		}
		else if(second >= 1 && second <= 9){
			printf("%c\n", first[second]);
		}
}
	return 0;
}