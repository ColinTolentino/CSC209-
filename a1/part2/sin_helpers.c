#include <stdlib.h>
#include <stdio.h>
// TODO: Implement populate_array
/*
 * Convert a 9 digit int to a 9 element int array.
 */
int populate_array(int sin_num, int *sin_array) {
	int counter = 8; //Since we parse the integer backwards
	int digit_counter = 0;
	//loops through the integer starting at the back and adds it the to array backwards.
	while(counter >= 0 && sin_num > 0){ 
		sin_array[counter] = (sin_num%10);
		sin_num = sin_num/10; 
		counter--;
		digit_counter++; //Keeps track of the number of loops incase the digit is too small.
	}
	if(digit_counter != 9 || sin_num > 0){ //This means the digit is too short or the digit is too big
		return 1;
	}
	return 0;
}
// TODO: Implement check_sin
/* 
 * Return 0 (true) iff the given sin_array is a valid SIN.
 */
int check_sin(int *sin_array) {
	int sum = 0;
	for(int i = 0; i < 9; i++){
		//if index odd 1 3 5 7
		if(i%2 != 0){ //Every other number.
			//if the value is between 5 and 9
			if((sin_array[i]*2) >= 10){
				//divide the value at the index by 10 after multiplying be 2
				sum = sum + 1 + ((sin_array[i]*2)%10);
			}
			else{
				//if value at the index is between 0 and 4
				sum = sum + (sin_array[i]*2);
			}
		}
		//for odd indices 0 2 4 6 8
		else{ //Not every other number.
			sum = sum + sin_array[i];
		}
	}
	//if its divisble by 10 then its valid
	if(sum%10 == 0){
		return 0;
	}
	//not valid
	else{
    	return 1;
	}	
}

