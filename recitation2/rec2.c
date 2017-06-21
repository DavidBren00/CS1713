#include <stdio.h>

int main()
{
	int a; //integer
	float b; //floating point
	char c; //character
	char d[11]; //string (array of 10 characters)
	
	printf("Enter an integer: "); 
	scanf("%d", &a); 
	printf("Your integer is: %d\n", a); 
	
	printf("Enter a floating point: "); 
	scanf("%f", &b);
	printf("Your floating point is: %f\n", b);
	
	printf("Enter a character: ");
	scanf(" %c", &c);
	printf("Your character is: %c\n", c);
	
	printf("Enter a string (length 10 chars): ");
	scanf("%10s", &d);
	printf("Your string is: %s\n", d);
	
	printf("Your character as an integer format is: ");
	printf("%d\n", c);
	
	printf("Your string as an integer format is: ");
	printf("%d\n", d);
		
	return 0;
}
