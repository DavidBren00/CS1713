/**********************************************************************
p2mat574.c by Rafael Rodriguez
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
#include "cs1713p2.h"
FILE *pFileCust;               // stream Input for Customer Reservation data
FILE *pFileFlight;             // stream innput for flight definitions


int main(int argc, char *argv[])
{
	Flight flightM[MAX_FLIGHTS];
	int iFlightCount; // the count of the number of entries in the flightM array
	char *pszFlightFileName = NULL;
    char *pszCustomerFileName = NULL;
 
    // Process the command switches
    processCommandSwitches(argc, argv,  &pszCustomerFileName, &pszFlightFileName);
    
    // open the Customer Reservation stream data file
    if (pszCustomerFileName == NULL)
        exitError(ERR_MISSING_SWITCH, "-c");

    // open the Flight Definitions stream data file
    if (pszFlightFileName == NULL)
        exitError(ERR_MISSING_SWITCH, "-f");
    
    pFileCust = fopen(pszCustomerFileName, "r");
    if (pFileCust == NULL)
        exitError(ERR_CUSTOMER_RES_FILENAME, pszCustomerFileName);
   
    pFileFlight = fopen(pszFlightFileName, "r");
    if (pFileFlight == NULL)
        exitError(ERR_FLIGHT_FILENAME , pszFlightFileName);
        
    // populate the flightM array and funtionally return the count of iFlightCount
	iFlightCount = getFlights(flightM);
	
	// print the original flight information before processing the reservation requests
	printf("Original Flight Information\n");
	printf("   Flight Id    From Dest Depart Avail   Unit Price\n");
	printFlights(flightM, iFlightCount);
   
    // process the Reservations with flight information
    processReservations(flightM, iFlightCount);
    
    // print the flight information after processing the reservation requests
    printf("Flight Information after Processing Reservations\n");
	printf("   Flight Id    From Dest Depart Avail   Unit Price\n");
	printFlights(flightM, iFlightCount);

    fclose(pFileCust);
    fclose(pFileFlight);
    printf("\n");    // included so that you can put a breakpoint on this line
    return 0;
}

// Used to compare the flightID with the reservation request and see if flight exists
int checkFlightID(Flight flightM[], char *szFlightId, int iFlightCount) {
	int i;
	 
	for(i = 0; i < iFlightCount; i++) {
		// Use string compare to check flight IDs
		if(strcmp(flightM[i].szFlightId, szFlightId) == 0) {
			return i;
		}
	}
	return -1;
}
/******************** processReservations *****************************
   void processReservations()
Purpose:
    Reads and prints three types of data records:  identification, 
    address, and flight requests.  
Parameters:
    n/a
Notes:
    pFileCust must be open
**************************************************************************/
void processReservations(Flight flightM[], int iFlightCount)
{ 
    char szInputBuffer[100];         // input buffer for a line of text
    int iscanfCnt;                   // for returning sscanf success
    Customer customer;               // customer identification and address
    FlightRequest flightRequest;     // flight request
    int count = 0;
    int i;

    // read customer info until EOF
    while (fgets(szInputBuffer, 100, pFileCust) != NULL)
    {
        printf("******************** Flight Reservation Request ********************\n");
        iscanfCnt = sscanf(szInputBuffer, "%c %10s %40s %30[^\n]\n" 
             , &customer.cGender
             , customer.szBirthDt
             , customer.szEmailAddr
             , customer.szFullName);
        
        // Check for bad customer identification data
        if (iscanfCnt < 4)
            exitError(ERR_CUSTOMER_ID_DATA, szInputBuffer);
        printf("%s %s (%c %s)\n" 
            , customer.szEmailAddr
            , customer.szFullName
            , customer.cGender
            , customer.szBirthDt);

        //read an address record

        if (fgets(szInputBuffer, 100, pFileCust) == NULL)
            exitError(ERR_CUSTOMER_ADDRESS_DATA, szInputBuffer);

        iscanfCnt = sscanf(szInputBuffer, "%30[^,], %20[^,], %2[^,], %5[^\n]\n" 
            , customer.szStreetAddress
            , customer.szCity
            , customer.szStateCd
            , customer.szZipCd);

        // Check for bad customer address data
        if (iscanfCnt < 4)
            exitError(ERR_CUSTOMER_ADDRESS_DATA, szInputBuffer);

        printf("%s\n%s %s %s\n"
            , customer.szStreetAddress
            , customer.szCity
            , customer.szStateCd
            , customer.szZipCd);

        // Print a heading for the Flight Requests

        printf("   %-10s\t%-5s\t%4s\t%4s\n", "Flight", "Seats", "Unit Price", "Cost");

        // There are many flight requests for a customer.
        // We expect to receive a szFlightId equal to "END"
        // marking the end of the flight requests.
        // Read flight requests until EOF. 
		double totalCost = 0.0;
        while (fgets(szInputBuffer, 100, pFileCust) != NULL)
        {
            iscanfCnt = sscanf(szInputBuffer, "%10s %4d"
                , flightRequest.szFlightId
                , &flightRequest.iRequestSeats);

            // Check for bad reservation data
            if (iscanfCnt < 2) {
                exitError(ERR_RESERVATION_DATA, szInputBuffer);
            }
                
        	// Read until the end of the line
			if (strcmp(flightRequest.szFlightId, "END") == 0)
                break;
            
			// Print the flight request information
        	printf("   %-10s\t%5d\t", flightRequest.szFlightId
            	, flightRequest.iRequestSeats);
				     		
        	// Compare the flight ids
            i = checkFlightID(flightM, flightRequest.szFlightId, iFlightCount);

            if (i == -1) {
            	printf("*** flight ID not found\n");
            } else if (flightRequest.iRequestSeats > flightM[i].iAvailSeatCnt) {
            	printf("*** insufficient seats\n");
            } else {
            	count = flightM[i].iAvailSeatCnt-flightRequest.iRequestSeats;
            	flightM[i].iAvailSeatCnt = count;
            	
            	printf("%.2f\t\t", flightM[i].dSeatPrice);
            	float cost = (flightM[i].dSeatPrice*flightRequest.iRequestSeats);
            	printf("%.2f\n", cost);
            	totalCost = totalCost + cost;
            }

        }
        printf("             Total Cost                 %.2f\n", totalCost);
	} // end of while loop for reading customer id records
}

// This retrieves the flight data and also returns the count of flights as its functional value.	
int getFlights(Flight flightM[]) {
	                 
	char szInputBuffer[MAX_LINE_SIZE + 1];
    int iScanfCnt; //for returning sscanf success
    // Flight flight; // don't need this since we are scanning items into flightM[i]
    int i = 0; // the index for the array
    
    // Need to stuff what you scan in into the array flightM[]
	
	while (fgets(szInputBuffer, MAX_LINE_SIZE, pFileFlight) != NULL) {
		
		//printf("Original Flight Information\n");
	    //printf("   Flight Id    From Dest Depart Avail   Unit Price\n");
		
		iScanfCnt = sscanf(szInputBuffer, "%10s %3s %3s %5s %4d %10lf\n"
        		, flightM[i].szFlightId
        		, flightM[i].szFrom
            	, flightM[i].szDest
            	, flightM[i].szDepartTm
            	, &flightM[i].iAvailSeatCnt
            	, &flightM[i].dSeatPrice
		);
		i++;
	} // end while loop
	return i;
}

/* printFlights */
void printFlights(Flight flightM[], int iFlightCount) {
	
	int i = 0;
		
	for (i = 0; i < iFlightCount; i++) {        	
		printf("   %10s   %3s  %3s  %5s   %4d     %8.2lf\n"
	        , flightM[i].szFlightId
	        , flightM[i].szFrom
	        , flightM[i].szDest
	        , flightM[i].szDepartTm
			, flightM[i].iAvailSeatCnt
            , flightM[i].dSeatPrice
		);
	};
}

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
void processCommandSwitches(int argc, char *argv[], char **ppszCustomerFileName, char **ppszFlightFileName)
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
            case 'f':                   // Flight File Name
                if (++i >= argc)
                    exitUsage(i, ERR_MISSING_ARGUMENT, argv[i - 1]);
                else
                    *ppszFlightFileName = argv[i];
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
