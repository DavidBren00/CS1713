/**********************************************************************
cs1713p0.c by your name
Purpose:	
    This program computes the average grade for each student.
Command Parameters:
    n/a
Input:
    The standard input file stream contains multiple lines (terminated by EOF).  
    Each line contains:
    dExam1 dExam2 dFinalExam szSudentNr szName
    10.2lf 10.2lf 10.2lf     6s         20s (but the name may have blanks)
Results:
    Prints the student Id, name, exam scores, and average for each student.
Returns:
    0 - normal
    2 - bad input data, not enough good data values on a line
Notes:
    When bad data is encountered, the program terminates.
**********************************************************************/

// If compiling using visual studio, tell the compiler not to give its warnings
// about the safety of scanf and printf
#define _CRT_SECURE_NO_WARNINGS 1

/*  Include Files */

#include <stdio.h>
#include "cs1713p0.h"

int main(int argc, char *argv[]) // char *argv[] is the pointer part of this
{
    Student student;                        // student structure for holding student data
    FILE *pfileStudent;                     // FILE variable for student file, this points to a file called fileStudent
    char szInputBuffer[MAX_LINE_SIZE + 1];  // input buffer for fgets
    int iScanfCnt;                          // scanf returns the number of successful inputs
    double dAverage;                        // grade average for a student

    pfileStudent = stdin; // Could be defined in line 33 as well, stdin is tied to the console

    // Print a heading
    printf("%-10s %-20s %10s %10s %10s %10s\n" //%... is a formatting pattern: "-" means left justified, "10s" is a string of up to 10 characters, "\n" is new line
        , "ID", "Name", "Exam 1", "Exam 2", "Final", "Average"); // These are the literals, they are all strings

    // read data input lines of text until EOF.  fgets returns NULL at EOF
    while (fgets(szInputBuffer, MAX_LINE_SIZE, pfileStudent) != NULL)
    {
        // if the line is just a line feed, skip it.
        if (szInputBuffer[0] == '\n')
            continue;

        iScanfCnt = sscanf(szInputBuffer, "%6s %20s %lf %lf %lf\n"
        	, student.szStudentIdNr
        	, student.szStudentFullNm
            , &student.dExam1 //& is the address
            , &student.dExam2
            , &student.dFinalExam);
            

        // Check for bad input.  scanf returns the number of valid conversions
        if (iScanfCnt < 5)
        {
            printf("invalid input when reading student data, only %d valid values. \n"
                , iScanfCnt);
            printf("\tdata is %s\n", szInputBuffer);
            return ERR_BAD_INPUT;
        }
        dAverage = (student.dExam1 + student.dExam2 + student.dFinalExam) / 3;
        printf("%-10s %-20s %10.2f %10.2f %10.2f %10.2f\n"
            , student.szStudentIdNr
            , student.szStudentFullNm
            , student.dExam1
            , student.dExam2
            , student.dFinalExam
            , dAverage);
    }
    fclose(pfileStudent);
    return 0;
}


