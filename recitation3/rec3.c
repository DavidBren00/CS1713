#include<stdio.h>

int main(int argc, char *argv[]) { // Required when taking command line args from user
	
	int a;
	int i;
//	printf("%d\n", argc);
//	printf("%s\n", argv[0]);   // Print out 1 command line argument per line
//	printf("%s\n", argv[1]);   // Print out 1 command line argument per line	
//	printf("%s\n", argv[2]);   // Print out 1 command line argument per line
	
//	for(i = 0; i < argc; i++) {
//		printf("printing %s\n", argv[i]);
//	}
	
	if (argc == 1) {
		printf("No additional parameters passed");
    } else if ((strcmp(argv[1], "movie") != 0) || ((strcmp(argv[2], "scifi") != 0))){
    	printf("Missing second argument");
	} else if ((strcmp(argv[1], "movie") == 0) && ((strcmp(argv[2], "scifi") == 0))) {
		printf("You chose scifi here. We recommend that you watch Star Wars");
	} else if ((strcmp(argv[1], "movie") == 0) && ((strcmp(argv[2], "children") == 0))) {
		printf("You have chosen children. We recommend that you watch The Lego Movie");
	}
}
