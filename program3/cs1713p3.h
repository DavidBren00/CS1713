/**********************************************************************
cs1713p3.h
GitHub Test Push
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
#define MAX_FLIGHTS 30
#define MAX_FLIGHT_ID_SIZE 11
#define MAX_LINE_SIZE 100

// boolean
#define FALSE           0
#define TRUE            1

/*  Error Messages */
#define ERR_MISSING_SWITCH          "missing switch"
#define ERR_EXPECTED_SWITCH         "expected switch, found "
#define ERR_MISSING_ARGUMENT        "missing argument for "
#define ERR_COMMAND_FILENAME        "invalid command file name "
#define ERR_CUSTOMER_ID_DATA        "invalid customer id data "
#define ERR_CUSTOMER_ADDRESS_DATA   "invalid customer address data "
#define ERR_RESERVATION_DATA        "invalid customer reservation data "
#define ERR_FLIGHT_FILENAME         "invalid flight definition file name "
#define ERR_FLIGHT_DEF_DATA         "invalid flight definition data "
#define ERR_TOO_MANY_FLIGHTS        "attempted to put more than MAX_FLIGHTS in the flight array, found "
#define ERR_FLIGHT_NOT_FOUND        "flight ID not found "
#define ERR_TOO_FEW_SEATS           "not enough available seats to satisfy request "
#define ERR_CUSTOMER_SUB_COMMAND    "invalid CUSTOMER subcommand "
#define ERR_FLIGHT_SUB_COMMAND      "invalid FLIGHTsubcommand "
#define ERR_INVALID_COMMAND         "invalid command "
#define ERR_INCREASE_SUB_COMMAND    "invalid INCREASE subcommand "
#define ERR_SHOW_SUB_COMMAND        "invalid SHOW subcommand "

/* program return codes */
#define ERR_COMMAND_LINE_SYNTAX     -1      // invalid command line syntax
#define USAGE_ONLY                  -2      // show usage only
#define ERROR_PROCESSING            -3

/* prototypes */

/* functions you must code (in addition to what you did for program #1) */
int getFlights(Flight flightM[], char * pszFlightFileName);
void printFlights(char *pszHeading, Flight flightM[], int iFlightCnt);
void sortFlights(Flight flightM[], int iFlightCount);
void processCustomerCommand(Flight flightM[], int iFlightCnt
    , char *pszSubCommand, char *pszRemainingInput
    , Customer *pCustomer, double *pdCustomerRequestTotalCost);
void processFlightCommand(Flight flightM[], int iFlightCnt
    , char *pszSubCommand, char *pszRemainingInput);
int search(Flight flightM[], int iFlightCount, char szMatchFlightId[]);

// Error handling functions
void exitError(char *pszMessage, char *pszDiagnosticInfo);
void exitUsage(int iArg, char *pszMessage, char *pszDiagnosticInfo);
