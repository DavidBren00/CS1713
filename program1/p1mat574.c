/**********************************************************************
cs1713p1.c
Purpose:
    This program reads customer flight reservations to produce output 
    suitable as a Reservation Request. 
Command Line Arguments:
    reserve  -c customerReservationFileName  
Input:  
    Stream input file which contains many customer reservations, each 
    containing possibly many flight requests.  There are three different 
    kinds of lines of data for each reservation:
        - Customer Identification Information:
            o One line per reservation request (separated by spaces)
              cGender   szBirthDt   szEmailAddr    szFullName 
              1c        10s         40s            30s (may contain spaces)
        - Customer Address Information 
            o One line per reservation request (separated by commas)
              szStreetAddress             szCity  szStateCd  szZipCd
              30s (may contain spaces)   20s     2s         5s
        - Flight Request:
            o 0 to many of them per reservation (terminated by END in the Flight ID)
              szFlightId    iNumSeats 
              10s           4d
Results:
    Prints each Reservation in a readable format.
    Examples:
        ******************* Flight Reservation Request  **********************
        petem@xyz.net Pete Moss (M 1986/01/01)
        123 Boggy Lane
        New Orleans, LA 70112
           Flight       Seats      
           H100.15005      2      
           H222.15005      2
           H200.15010      2
           H333.15010      2       
        ******************* Flight Reservation Request  **********************
        pcorn@abc.net Pop Corn (M 1957/02/02)
        456 Kernel
        San Antonio, TX 78210
           Flight       Seats      
           H222.15005      1
           HXXX.XXXXX      1
           H333.15010      1       
Returns:
    0  normal
    -1 invalid command line syntax
    -2 show usage only
    -3 error during processing, see stderr for more information
Notes:
    reserve -?  will provide the usage information.  In some shells,
                you will have to type reserve -\?
  
**********************************************************************/
// If compiling using visual studio, tell the compiler not to give its warnings
// about the safety of scanf and printf
#define _CRT_SECURE_NO_WARNINGS 1

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cs1713p1.h"
FILE *pFileCust;               

void processCommandSwitches(int argc, char *argv[], char **ppszCustomerFileName); 
void processReservations();


int main(int argc, char *argv[])
{
    char *pszCustomerFileName = NULL;
 
    // Process the command switches
    processCommandSwitches(argc, argv,  &pszCustomerFileName); 
    
    // open the Customer Reservation stream data file
    if (pszCustomerFileName == NULL)
        exitError(ERR_MISSING_SWITCH, "-c");
    
    pFileCust = fopen(pszCustomerFileName, "r");
    if (pFileCust == NULL)
        exitError(ERR_CUSTOMER_RES_FILENAME, pszCustomerFileName);
   
    // process the Reservations
    processReservations();

    fclose(pFileCust);
    printf("\n");    // included so that you can put a breakpoint on this line
    return 0;
}

/****** you need to document and code this function *****/

void processReservations()
{
	// Read the data in from the file into a buffer
	// Use the buffer to chop it up into the fields that represent the data
	Customer customer;                      // structure for holding customer data
	FlightRequest request;                  // structure for holding flight data
    char szInputBuffer[MAX_LINE_SIZE + 1];  // input buffer for fgets
    int iScanfCnt;                          // scanf returns the number of successful inputs

    // Print a heading
    //printf("%100s","******************* Flight Reservation Request  **********************");
    printf("******************* Flight Reservation Request  **********************\n");
    char state = ID_INFO;

    // read data input lines of text until EOF.  fgets returns NULL at EOF
    while (fgets(szInputBuffer, MAX_LINE_SIZE, pFileCust) != NULL)
    {
        // if the line is just a line feed, skip it.
        if (strncmp(szInputBuffer, "END 0", 5) == 0) {
        	printf("******************* Flight Reservation Request  **********************\n");
        	state = ID_INFO;
        	//clears the Customer struct for error checking
			memset(&customer, 0, sizeof(customer));
			
        	continue;
		}
		
		if (state == ID_INFO) {
			
			iScanfCnt = sscanf(szInputBuffer, "%c %10s %40s %30[^\n]"
        		, &customer.cGender
        		, customer.szBirthDt
            	, customer.szEmailAddr
            	, customer.szFullName
			);
			
			//Check for bad input.  scanf returns the number of valid conversions
        	if (iScanfCnt < 4) {
            	fprintf(stderr,"Error reading Customer Identification info! See stderr for more info. Exiting...\n", szInputBuffer);
            	exit(-3);
            } else {
				printf("%s %s (%c %10s)\n"
	        		, customer.szEmailAddr
	        		, customer.szFullName
	            	, customer.cGender
	        		, customer.szBirthDt
					);
			}
			state = ADDRESS_INFO;
		}
		else if(state == ADDRESS_INFO) {
			iScanfCnt = sscanf(szInputBuffer, "%30[^,],%20[^,],%2[^,],%5[^,]"
        		, customer.szStreetAddress
        		, customer.szCity
            	, customer.szStateCd
            	, customer.szZipCd
			);
			// Check for bad input.  scanf returns the number of valid conversions
        	if (iScanfCnt < 4) {
            	fprintf(stderr, "Error reading Customer Address info! See stderr for more info. Exiting...\n", szInputBuffer);
            	exit(-3);
            } else{
				printf("%s\n"
	        		, customer.szStreetAddress
					);
				printf("%s, %s %s\n"
	        		, customer.szCity
	            	, customer.szStateCd
	        		, customer.szZipCd
					);
			}
			state = FLIGHT_INFO;
		}
		else if(state == FLIGHT_INFO) {
			memset(&request, 0, sizeof(request));
			iScanfCnt = sscanf(szInputBuffer, "%10s %4d"
        		, request.szFlightId
        		, &request.iRequestSeats
			);
			// Check for bad input.  scanf returns the number of valid conversions
        	if (iScanfCnt < 2) {
            	fprintf(stderr,"Error reading Flight Request info! See stderr for more info. Exiting...\n", szInputBuffer);
            	exit(-3);
            } else {
				printf("   %10s %4d\n"
	        		, request.szFlightId
	        		, request.iRequestSeats
					);
			}
		}
    } // End of the while loop
} // End of the method


/******************** processCommandSwitches *****************************
void processCommandSwitches(int argc, char *argv[], char **ppszCustomerFileName)
Purpose:
    Checks the syntax of command line arguments and returns the filenames.
    If any switches are unknown, it exits with an error.
Parameters:
    I   int argc                        count of command line arguments
    I   char *argv[]                    array of command line arguments
    O   char **ppszCustomerFileName     customer reservation file name
Notes:
    If a -? switch is passed, the usage is printed and the program exits
    with USAGE_ONLY.
    If a syntax error is encountered (e.g., unknown switch), the program
    prints a message to stderr and exits with ERR_COMMAND_LINE_SYNTAX.
**************************************************************************/
void processCommandSwitches(int argc, char *argv[], char **ppszCustomerFileName)
{
    int i;
    
    for (i = 1; i < argc; i++)
    {
        // check for a switch
        if (argv[i][0] != '-')
            exitUsage(i, ERR_EXPECTED_SWITCH, argv[i]);
        // determine which switch it is
        switch (argv[i][1])
        {
            case 'c':                   // Customer Reservation File Name
                if (++i >= argc)
                    exitUsage(i, ERR_MISSING_ARGUMENT, argv[i - 1]);
                else
                    *ppszCustomerFileName = argv[i];
                break;
            case '?':
                exitUsage(USAGE_ONLY, "", "");
                break;
            default:
                exitUsage(i, ERR_EXPECTED_SWITCH, argv[i]);
        }
    }
}

/******************** exitError *****************************
    void exitError(char *pszMessage, char *pszDiagnosticInfo)
Purpose:
    Prints an error message and diagnostic to stderr.  Exits with
    ERROR_PROCESSING.
Parameters:
    I char *pszMessage              error message to print
    I char *pszDiagnosticInfo       supplemental diagnostic information
Notes:
    This routine causes the program to exit.
**************************************************************************/
void exitError(char *pszMessage, char *pszDiagnosticInfo)
{
    fprintf(stderr, "Error: %s %s\n"
        , pszMessage
        , pszDiagnosticInfo);
    exit(ERROR_PROCESSING);
}
/******************** exitUsage *****************************
    void exitUsage(int iArg, char *pszMessage, char *pszDiagnosticInfo)
Purpose:
    If this is an argument error (iArg >= 0), it prints a formatted message
    showing which argument was in error, the specified message, and
    supplemental diagnostic information.  It also shows the usage. It exits
    with ERR_COMMAND_LINE_SYNTAX.

    If this is just asking for usage (iArg will be -1), the usage is shown.
    It exits with USAGE_ONLY.
Parameters:
    I int iArg                      command argument subscript
    I char *pszMessage              error message to print
    I char *pszDiagnosticInfo       supplemental diagnostic information
Notes:
    This routine causes the program to exit.
**************************************************************************/
void exitUsage(int iArg, char *pszMessage, char *pszDiagnosticInfo)
{
    if (iArg >= 0)
        fprintf(stderr, "Error: bad argument #%d.  %s %s\n"
            , iArg
            , pszMessage
            , pszDiagnosticInfo);
    fprintf(stderr, "reserve -c customerReservationFile\n");
    if (iArg >= 0)
        exit(-1);
    else
        exit(-2);
}
