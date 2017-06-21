/**********************************************************************
cs1713p0.h
Purpose:
   Defines constants:
       Maximum sizes
       Errors
   Defines Student structure.
   Defines function prototypes used only for this program.
Notes:
   
**********************************************************************/

/* constants */
#define MAX_LINE_SIZE 100
#define ERR_BAD_INPUT 2

/* typedefs and function prototypes */

/* Student typedef contains exam scores, student IDs, and full name */
typedef struct
{
    double dExam1;                  // exam #1 score
    double dExam2;                  // exam #2 score
    double dFinalExam;              // final exam score
    char szStudentIdNr[7];          // Student Identification Nr. In C, all strings are terminated by a null (0)
    char szStudentFullNm[21];       // Student full Nm
} Student;

