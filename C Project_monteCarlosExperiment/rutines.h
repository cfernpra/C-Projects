#include <errno.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>

#define MAXSTR		256
#define MAXPROCS	5

#define TRUE		1
#define FALSE		0


/* Prototypes of the developed functions */
void error(char *miss);
void write_message(char *miss);
void read_string(char *miss, char *addr);
void write_string(char *miss, char *buffer);

/******************************************************************************/
/* Name of funtion: error                                                    */
/* Description: Write an error message and finish the execution of the program  */
/* Parameters:       The error message (miss)                               */
/******************************************************************************/

void error(char *miss)
{
	write(2, miss, strlen(miss));
	write(2, "\n", 1);
	write(2, strerror(errno), strlen(strerror(errno)));
	exit(1);
}

/******************************************************************************/
/* Name of funtion: write_message                                             */
/* Description:       Write message by the standard error output 	      */
/* Parameters:       The message	                                      		*/
/******************************************************************************/

void write_message(char *miss)
{
	write(2, miss, strlen(miss));
	write(2, "\n", 1);
}



/*****************************************************************************/
/* Name of funtion: read_string                                           */
/* Description:       Write a message that is passed as a parameter and save  */
/*                   to memory what it reads from the standard input          */ 
/* Parameters:       The explanatory message (miss) and the memory address where  */
/*                   save reading                                      */
/*****************************************************************************/

void read_string(char *miss, char *addr)
{
	int n;

	if (write(1, miss, strlen(miss)) < 0)
		error("error writing");
	if ((n = read(0, addr, MAXSTR)) < 0)
		error("error reading");
	if (addr[n - 1] != '\n')
		error("It should be a line jump");
	addr[n - 1] = 0;	/* We put the end of string mark */
}


/******************************************************************************/
/* Name of funtion: write_string                                          */
/* Description:       Write a explanatory message and write a string         */
/*                   of memory for standard output                          */
/* Parametres:       The explanatory message (miss) and the memory address where   */
/*                   is the string                                              */
/******************************************************************************/

void write_string(char *miss, char *buffer)
{
	if (write(1, miss, strlen(miss)) < 0)
		error("write miss");
	if (write(1, " ", 1) < 0)
		error("write");
	if (write(1, buffer, strlen(buffer)) < 0)
		error("write buffer");
	if (write(1, "\n", 1) < 0)
		error("write");
}
