/*Jillian Coveney
 * Coen 11
 * Lab 1 
 * Tuesday 2:15pm
 */

#include <time.h>
#include <stdlib.h>
#include <stdio.h>

int divide(int a, int b);

int main()
{
	int dividend;
	int denominator;
	int quotient;
	int guess;
	int i;
	int counter = 0;
	int score = 0; 
	

srand((int)time(NULL));	

	for  (i = 0; i < 10; i++)
		{
		denominator  = rand()%12+1; 
		quotient = rand()%13;
		dividend = (denominator * quotient);
		printf("The dividend is: %d\n", dividend);
		printf("The denominator is: %d\n", denominator);
		counter = division (dividend, denominator, quotient);
		score += counter;
		}
	printf("Total number of correct answers: %d\n", score);
}

int division (dividend, denominator, quotient)
{
	int guess;
		printf("Enter guess for quotient: \n");
		scanf("%d", &guess);
		if (guess == quotient){
			printf("Correct Answer! \n");
				return 1;
		}else{
			printf("Wrong Answer. The correct answer was %d\n", quotient);
				return 0;
		}
}
