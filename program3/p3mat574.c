/***********************************************************************************
p3mat574.c by Rafael Rodriguez   (skeletal version)
Purpose:
    This program reads flight information and a command file.   It 
    processes the commands against the flight information.
    This file contains the functions that students write.
Command Parameters:
    p3 -f flightFileName -c commandFileName
Input:
    Flight   Stream input file which contains many records defining flights:
                 szFlightId szFrom  szDest  szDepartTm  iAvailSeats dSeatPrice 
                 10s        3s      3s      5s          4d          10lf 

    Command  This is different from the previous assignment.  The file contains 
             text in the form of commands (one command per text line):  
                 CUSTOMER BEGIN cGender   szBirthDt   szEmailAddr    szFullName
                     specifies the beginning of customer request and includes 
                     all the identification information from program 2.
                 CUSTOMER ADDRESS szStreetAddress,szCity,szStateCd,szZipCd
                     specifies the address for a customer (separated by commas)
                 CUSTOMER REQUEST szFlightId iNumSeats
                     specifies a single flight request.  Steps:
                     >	Print the flight ID and requested number of seats
                     >	Lookup the flight ID using a binary search.  If not found,
                        print a warning (but do not terminate your program) and return.
                     >	If found, try to satisfy the entire requested number of seats.
                        If not enough seats,  print a warning and return.
                     >	Print the unit price and cost.
                     >	Accumulate the total cost for this customer
                 CUSTOMER COMPLETE
                     specifies the completion of the list of flight requests 
                     for a customer.
                 FLIGHT INCREASE szFlightId iQuantity
                     increase the available seats for a flight by the specified quantity.
                 FLIGHT SHOW szFlightId    
                     requests a display of a particular flight.  Show all of its information.

Results:
    Prints the Flights prior to sorting
    Prints the Flight after sorting.
    Processes the commands (see above) and shows any errors.
    Prints the resulting Flights
Returns:
    0  normal
    -1 invalid command line syntax
    -2 show usage only
    -3 error during processing, see stderr for more information

Notes:
    p3 -?       will provide the usage information.  In some shells,
                you will have to type p3 -\?

**********************************************************************/
// If compiling using visual studio, tell the compiler not to give its warnings
// about the safety of scanf and printf
#define _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/******************** getFlights **************************************
    int getFlights(Flight flightM[])
Purpose:

Parameters:
  
Returns:
    
Notes:
    
**************************************************************************/
int getFlights(Flight flightM[], char * pszFlightFileName)
{
    char szInputBuffer[100];		// input buffer for reading data
    int i = 0;                      // subscript in flightM
    int iScanfCnt;                  // returned by sscanf
    FILE *pFileFlight;              // Stream Input for Flights data. 

    /* open the Flights stream data file */
    if (pszFlightFileName == NULL)
        exitError(ERR_MISSING_SWITCH, "-f");

    pFileFlight = fopen(pszFlightFileName, "r");
    if (pFileFlight == NULL)
        exitError(ERR_FLIGHT_FILENAME, pszFlightFileName);

    while (fgets(szInputBuffer, MAX_LINE_SIZE, pFileFlight) != NULL) {
		
		iScanfCnt = sscanf(szInputBuffer, "%10s %3s %3s %5s %4d %10lf\n"
        		, flightM[i].szFlightId
        		, flightM[i].szFrom
            	, flightM[i].szDest
            	, flightM[i].szDepartTm
            	, &flightM[i].iAvailSeatCnt
            	, &flightM[i].dSeatPrice
		);
		
		if(iScanfCnt < 6) {
			exitError(ERR_FLIGHT_DEF_DATA, szInputBuffer);
		}
		i++;
	} // end while loop
    fclose(pFileFlight);
    return i;
}
/******************** sortFlights **************************************
    void sortFlights(Flight flightM[], int iFlightCnt)
Purpose:

Parameters:
  
Returns:
    
Notes:
    
**************************************************************************/
//void sortFlights(Flight flightM[], int iFlightCnt) // from skeleton code
void sortFlights(Flight flightM[], int iFlightCnt) // from the bubble sort code
{
    int i, j;
    Flight temp;

	for (i = 0; i < (iFlightCnt - 1); i++)
	{
		for (j = 0; j < (iFlightCnt - i - 1); j++)
		{
			//int strcmp(szStr1, szStr2) // want to see if this is -1
			if (strcmp(flightM[j+1].szFlightId, flightM[j].szFlightId) < 0)
			{
				//exchange [j+1] with [j]
				temp = flightM[j+1];
				flightM[j+1] = flightM[j];
				flightM[j] = temp;
			}

		}  //end for j

	} //end for i

} // end sort

/******************** printFlights **************************************
    void printFlights(char *pszHeading, Flight flightM[], int iFlightCnt)
Purpose:

Parameters:
  
Returns:
    
Notes:
    
**************************************************************************/
void printFlights(char *pszHeading, Flight flightM[], int iFlightCnt)
{
    int i;
    printf("%s\n", pszHeading);
    printf("    %-10s   %-4s %-4s %-6s %-6s  %-10s\n"
        , "Flight Id", "From", "Dest", "Depart", "Avail", "Unit Price");

    for (i = 0; i < iFlightCnt; i++)
    {
        printf("    %-10s   %-4s %-4s %-6s %5d   %10.2lf\n"
            , flightM[i].szFlightId
            , flightM[i].szFrom
            , flightM[i].szDest
            , flightM[i].szDepartTm
            , flightM[i].iAvailSeatCnt
            , flightM[i].dSeatPrice);
    }
}

/********************processCustomerCommand *****************************
    void processCustomerCommand(Flight flightM[], int iFlightCnt
     , char *pszSubCommand, char *pszRemainingInput
     , Customer *pCustomer, double *pdCustomerRequestTotalCost )
Purpose:
    Processes the subcommands associated with the CUSTOMER command:
                 CUSTOMER BEGIN cGender   szBirthDt   szEmailAddr    szFullName
                     specifies the beginning of customer request and includes 
                     all the identification information from program 2.
                 CUSTOMER ADDRESS szStreetAddress,szCity,szStateCd,szZipCd
                     specifies the address for a customer (separated by commas)
                 CUSTOMER REQUEST szFlightId iNumSeats
                     specifies a single flight request.  Steps:
                     >	Print the flight ID and requested number of seats
                     >	Lookup the flight ID using a binary search.  If 
                        not found, print a warning (but do not terminate your 
                        program) and return.
                     >	If found, try to satisfy the entire requested number 
                        of seats.  If not enough seats, print a warning and 
                        return.
                     >	Print the unit price and cost.
                     >	Accumulate the total cost for this customer

                 CUSTOMER COMPLETE
                     specifies the completion of the list of flight requests 
                     for a customer.
Parameters:
    I/O Flight flightM[]              Array of flights
    I   int   iFlightCnt              Number of elments in flightM[]
    I   char  *pszSubCommand          Should be BEGIN, ADDRESS, REQUEST or COMPLETE
    I   char  *pzRemainingInput       Points to the remaining characters in the input
                                      line (i.e., the characters that following the
                                      subcommand).
    I/O Customer *pCustomer           The BEGIN subcommand begins a new customer.  The 
                                      customer's Request Total Cost must be set to 0.
    I/O double   *pdCustomerRequestTotalCost     The customer total cost.  This changes depending
                                      on the subcommand:
                                          BEGIN - set to 0
                                          REQUEST - add the cost  (unless there is a warning) 
Notes:

**************************************************************************/
void processCustomerCommand(Flight flightM[], int iFlightCnt
    , char *pszSubCommand, char *pszRemainingInput
    , Customer *pCustomer, double *pdCustomerRequestTotalCost)
{
    int iScanfCnt;
    FlightRequest flightRequest;

    // Determine what to do based on the subCommand
    if (strcmp(pszSubCommand, "BEGIN") == 0)
    {
        // get the Customer Identification Information
        // your code

        if (iScanfCnt < 4)
            exitError(ERR_CUSTOMER_ID_DATA, pszRemainingInput);

    }
    else if (strcmp(pszSubCommand, "COMPLETE") == 0)
    {
        // print the customer's total cost
        // your code 

    }
    else if (strcmp(pszSubCommand, "ADDRESS") == 0)
    {
        // get the postal address 
        // your code 

        printf("\t\t\t\t%-10s %8s %10s %8s\n"
            , "Flight Id"
            , "Quantity"
            , "Unit Price"
            , "Cost");
    }
    else if (strcmp(pszSubCommand, "REQUEST") == 0)
    {
        int i;
        // get a flight request
        // your code 


        // find the flight in the array
        i = search(flightM, iFlightCnt, flightRequest.szFlightId);

        // your code

  
    }
    else printf("   *** %s %s\n", ERR_CUSTOMER_SUB_COMMAND, pszSubCommand);
}
/********************processFlightCommand *****************************
    void processFlightCommand(Flight flightM[], int iFlightCnt
         , char *pszSubCommand, char *pszRemainingInput)
Purpose:
    Processes the subcommands associated with the FLIGHT command:
        FLIGHT INCREASE szFlightId iQuantity
               increase the available seats for a flight by the specified quantity.
        FLIGHT SHOW szFlightId    
               requests a display of a particular flight.  Show all of its information.
Parameters:
    I/O Flight flightM[]              Array of flights
    I   int   iFlightCnt              Number of elments in flightM[]
    I   char  *pszSubCommand          Should be RECEIVE or SHOW
    I   char  *pzRemainingInput       Points to the remaining characters in the input
                                      line (i.e., the characters that following the
                                      subcommand).
Notes:

**************************************************************************/
void processFlightCommand(Flight flightM[], int iFlightCnt
                             , char *pszSubCommand, char *pszRemainingInput)
{
    Flight flight;
    int iQuantity;      // quantity of seats 
    int iScanfCnt;
    int i;

    // Determine what to do based on the subCommand
    // your code

}
/******************** search *****************************
    int search(Flight flightM[], int iFlightCnt, char *pszMatchFlightId)
Purpose:
    
Parameters:
    I   Flight flightM[]               Array of flights
    I   int   iFlightCnt               Number of elments in flightM[]
    I   char *pszMatchFlightId         Flight Id to find in the array
Returns:
    >= 0   subscript of where the match value was found
    -1     not found
Notes:

**************************************************************************/
int search(Flight flightM[], int iFlightCnt, char *pszMatchFlightId)
{
    // your code


}

//int binarySearch (int numbers[], int length, int iMatch)
//{
//	int LB, UB, MID;
//	LB = 0;
//	UB = length – 1; //UB,  LB initially set to represent entire array
//
//	while ( LB <= UB)
//	{
//                //find the new midpoint
//    		MID = (LB+UB) / 2;
//
//    		if (iMatch == numbers[MID])
//         		return MID;     //success
//    		else if (iMatch < numbers[MID])
//         		UB = MID – 1;    //set new UB one less than midpoint
//    		else
//         		LB = MID + 1;    //set new LB on greater than midpoint
//	}
//
//       return -1;
//}
