#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define NUMBER_SONGS 4

// 1 x 2 x 3 (3 is the first dimension, 2 is the second, and 1 is the third
// char str[8] // One dimensional array with 8 cells
// char str[2][8] // Two dimensional array with 2 rows and 8 cells
// char [4][2][8] // Three dimensional array with 4 tables of 2 rows and 8 cells

// Rec 3: Three dimensional array (4 x 2 x 100)
// Store the name of the song in 100 cells
// Store the artist name in the 2nd row
// Store songs in the 4 tables

void main() {
	char array [NUMBER_SONGS][2][100] =
		{
			{
				{"Californication"},
				{"Red Hot Chili Peppers"}
			},
			{
				{"Dani California"},
				{"Red Hot Chili Peppers"}
			},
			{
				{"By The Way"},
				{"Red Hot Chili Peppers"}
			},
			{
				{"Aeroplane"},
				{"Red Hot Chili Peppers"}
			},
		};
	
	int i, n;
	time_t t;
	int randomNumber;
	
	// Initialize random number generator
	srand((unsigned) time(&t));
	
	// Generate a random number from 0 to 3
	randomNumber = rand() % 3;
	
	// Print the random number
	printf("The random number is %d\n", randomNumber);
	
	switch(randomNumber) {
		case 0:
			printf("'Californication' by Red Hot Chili Peppers");
			break;
		case 1:
			printf("'Dani California' by Red Hot Chili Peppers");
			break;
		case 2:
			printf("'By The Way' by Red Hot Chili Peppers");
			break;
		case 3:
			printf("'Aeroplane' by Red Hot Chili Peppers");
			break;
	}
}
