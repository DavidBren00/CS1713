/**********************************************************************
cs1713p2.h
Purpose:
    Defines typedefs for
        Customer - includes email address, full name, birthdate, gender,
                   and postal address
        FlightRequest - flights the customer wants.
        Flight - flight information
    Defines constants for
        boolean values
        error messages
        program return codes
    Prototypes
Notes:
    
**********************************************************************/

/**** typedefs ****/

// Customer information 
typedef struct
{
    char        cGender;                    // M - Male or F - Female
    char        szBirthDt[11];              // Birthdate - YYYY/MM/DD
    char        szEmailAddr[51];            // customer's email address is used
                                            // to uniquely define a customer
    char        szFullName[31];             // customer full name
    char        szStreetAddress[31];        // street number, street name, and 
                                            // (optional) apartment number
    char        szCity[21];                 // address city
    char        szStateCd[3];               // address state code
    char        szZipCd[6];                 // address zip code
} Customer;

// Flight Request information 
typedef struct
{
    char szFlightId[11];                   // Flight Identifier 
    int iRequestSeats;                     // Requested number of seats
} FlightRequest;

// Flight Definition
typedef struct
{
    char szFlightId[11];                    // Flight Identifier
    char szFrom[4];                         // Origin of the flight
    char szDest[4];                         // Destination of the flight
    char szDepartTm[6];                     // Departure Time (24 hour clock) HH:MM
    int  iAvailSeatCnt;                     // Number of available seats
    double dSeatPrice;                      // Price per seat (i.e., unit price)
} Flight;

/**** constants ****/
// Maximum sizes 
#define MAX_LINE_SIZE   100
#define MAX_FLIGHTS     30
#define ID_INFO         1
#define ADDRESS_INFO    2
#define FLIGHT_INFO     3

// boolean
#define FALSE           0
#define TRUE            1

/*  Error Messages */
#define ERR_MISSING_SWITCH          "missing switch"
#define ERR_EXPECTED_SWITCH         "expected switch, found"
#define ERR_MISSING_ARGUMENT        "missing argument for"
#define ERR_CUSTOMER_RES_FILENAME   "invalid customer reservation file name"
#define ERR_CUSTOMER_ID_DATA        "invalid customer id data"
#define ERR_CUSTOMER_ADDRESS_DATA   "invalid customer address data"
#define ERR_RESERVATION_DATA        "invalid customer reservation data"
#define ERR_FLIGHT_FILENAME         "invalid flight definition file name"
#define ERR_FLIGHT_DEF_DATA         "invalid flight definition data"
#define ERR_TOO_MANY_FLIGHTS        "attempted to put more than MAX_FLIGHTS in the flight array, found"

/* program return codes */
#define ERR_COMMAND_LINE_SYNTAX     -1      // invalid command line syntax
#define USAGE_ONLY                  -2      // show usage only
#define ERROR_PROCESSING            -3

/* prototypes */

/* new functions you must code (in addition to what you did for program #1) */
int getFlights(Flight flightM[]);
void printFlights(Flight flightM[], int iFlightCount);

//TODO: UNCOMMENT AND REPLACE older processReservations() declaration
void processReservations(Flight flightM[], int iFlightCount);
//void processReservations();

//TODO: UNCOMMENT AND REPLACE older processCommandSwitches(....) declaration 
void processCommandSwitches(int argc, char *argv[], char **ppszCustomerFileName, char **ppszFlightFileName);
//void processCommandSwitches(int argc, char *argv[], char **ppszCustomerFileName);

void exitError(char *pszMessage, char *pszDiagnosticInfo);
void exitUsage(int iArg, char *pszMessage, char *pszDiagnosticInfo);
