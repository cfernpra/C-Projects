#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include "rutines.h"
#include <math.h>
 

#define DMAX_PROCESSES 5
#define DPOINTS_RANDOM 10000

/* Parameter variables according to menu options */
int    T=DPOINTS_RANDOM;	/* Number of random points to calculate for iteration */
int    N=0;									/* Number Montecarlo processes */
double MaximError=0.1;			/* Maximum error allowed */

int 	processes=0;
int 	pids[DMAX_PROCESSES];
int 	pipesOut[DMAX_PROCESSES][2];
int 	pipesIn[DMAX_PROCESSES][2];

/* Prototypes of the developed functions */
int menu(void);
void initialize(void);
int create_processes_montecarlo(void);
int remove_proces_montecarlo(void);
void calculate_pi(void);
void finish(void);



/*********************************************************************************/
/* Funcion name: menu                                                            */
/* Description: Generate the menu, read the option from the standard input       */
/* Parameters: Message read (returned by reference)                              */
/* Return: chosen option                                                         */
/* Global variable MaximError is modified as a result                            */	
/* from option 3; and the N variable as a result of option 1                     */
/*********************************************************************************/

int menu(void)
{
	int option;
	char buffer[MAXSTR];
	
	/* write menu */
 	write_string("\nMENU:","");
	write_string(" 1) Create processes Montecarlo ","");
	write_string(" 2) Remove proces Montecarlo ","");
	write_string(" 3) Calculate ","");
	write_string(" 4) Finish all the aplications ","");
	
	read_string("OPTION  ", buffer);
	option = atoi(buffer);
	while (option < 1 || option > 4)
	{
		read_string("\n   Indicate the option again ", buffer);
		option=atoi(buffer); 
	}

	/* reads the number of processes Montecarlo */
	if (option == 1)
	{
		do
		{
			read_string ("Enter the number of processes: ", buffer);
			N = atoi(buffer);
			
			if ( (N > 5) || (N < 1) )
				write_string("Number of the processess incorrect. ", "");
		}
		while ((N > 5) || (N < 1));
	} 

	/* read maximum number of errors allowed */
	if (option == 3)
	{
		do
		{
			read_string("write maximum number of errors allowed: ", buffer); //fixed error caused by lack of ("
			MaximError = atof(buffer);
		}
		while	( MaximError<=0 );
	} 

	return option;
}


/*********************************************************************************/
/* Function name: initialize                                                     */
/* Description: Initialize application                                           */
/* Parameters: None                                                              */
/*********************************************************************************/

void initialize(void) {
    // Since pipesOut / pipesIn are an array of natural numbers, Ive decided to use memset in order to optimize the initiation of the functions via fullfit it with -1
    memset(pipesOut, -1, sizeof(pipesOut));
    memset(pipesIn, -1, sizeof(pipesIn));
    memset(pids, 0, sizeof(pids));

    // Also i used a mixed of time and PID to get a random number in the seed 
    srand((unsigned int)(time(NULL) ^ (getpid() << 16)));
}


/*********************************************************************************/
/* Function name: create_processes_montecarlo                                    */
/* Description: Create Montecarlo processes                                      */
/* Parameters: None                                                              */
/* Return: TRUE if successful, FALSE on errors                                   */
/*********************************************************************************/

int create_processes_montecarlo(void) {
    // Ive used this loop to check if any process is been executing
    for (int i = 0; i < DMAX_PROCESSES; i++) {
        if (pids[i] != 0) {
            return FALSE; // In the case there is any PID in the array, this means there is a executing process ongoing 
        }
    }

    // Ive created this temporary array to save the pids 
    int temp_pids[DMAX_PROCESSES] = {0};

    for (int i = 0; i < N; i++) {
        // Here i created the pipes with error handeling 
        if (pipe(pipesOut[i]) == -1) {
            perror("pipeOut"); // In the case there is a error, this will show if the out pipe creation fails Mostrar mensaje 
            return FALSE;
        }
        if (pipe(pipesIn[i]) == -1) {
            perror("pipeIn"); //This will show a error message when the in pipe fails
            close(pipesOut[i][0]); // This is to close the in and out connectios
            close(pipesOut[i][1]);
            return FALSE;
        }

        int pid = fork();
        if (pid < 0) {
            perror("fork"); //This will show a error message if the child process fails
            return FALSE;
        } else if (pid == 0) { //This is the child procress
            //and here the configuration of pipes of the child processs and also the execution of the montecarlo
            close(pipesOut[i][1]); //This will close the out child pipe
            if (dup2(pipesOut[i][0], STDIN_FILENO) == -1) {
                perror("dup2 pipesOut");
                exit(EXIT_FAILURE);
            }
            close(pipesOut[i][0]); //and this will close the original out pipe

            close(pipesIn[i][0]); //and this close the reading in pipe
            if (dup2(pipesIn[i][1], STDOUT_FILENO) == -1) {
                perror("dup2 pipesIn");
                exit(EXIT_FAILURE);
            }
            close(pipesIn[i][1]); //here the same than before but in the original

            execlp("./mcarlo", "./mcarlo", NULL); //this will execute the montecalro process
            perror("execlp"); //this will show a error if the execlp fails
            exit(EXIT_FAILURE); //It will out if there is any error in execlp
        } else { //this is the father process
            //The code to close the pipes and save the PID
            close(pipesOut[i][0]); //this close the read (in) 
            close(pipesIn[i][1]); // this close the (out)

            temp_pids[i] = pid; //here it save the PID in temp way
        }
    }

    //This will copy the PIDs to the global array, but only if everything was fine
    for (int i = 0; i < N; i++) {
        pids[i] = temp_pids[i];
        processes++; //this to increment the process count
    }

    return TRUE;
}


/*********************************************************************************/
/* Function name: remove_proces_montecarlo                                       */
/* Description: Remove the last Montecarlo process                               */
/* Parameters: None                                                              */
/* Return: TRUE if successful, FALSE on errors                                   */
/*********************************************************************************/


int remove_proces_montecarlo(void) {
    //This is to verify if there is any process been executed
    int active_processes = 0;
    for (int i = 0; i < DMAX_PROCESSES; i++) {
        if (pids[i] != 0) {
            active_processes = 1;
            break;
        }
    }
    if (!active_processes) {
        return FALSE; //just if there is not process to delete
    }

    //this a temp array to save the PIDs that has been deleted
    int temp_pids[DMAX_PROCESSES] = {0};
    int removed_count = 0;

    for (int i = 0; i < DMAX_PROCESSES; i++) {
        if (pids[i] != 0) {
            if (kill(pids[i], SIGTERM) == -1) {
                perror("kill"); //this is to show an error if kill fails
                return FALSE;
            }

            //This wait till the process ends
            if (waitpid(pids[i], NULL, 0) == -1) {
                perror("waitpid"); //this will show an error if waitpid fails
                return FALSE;
            }

            //This will save the PID in the temporal array
            temp_pids[removed_count++] = pids[i];

            //this cleans the asociated pipes
            close(pipesOut[i][1]);
            close(pipesIn[i][0]);

            //this resets the PID in the global array
            pids[i] = 0;
        }
    }

    //Thi will generate a message of process deleting
    for (int i = 0; i < removed_count; i++) {
        char buffer[MAXSTR];
        snprintf(buffer, sizeof(buffer), "Destroyed process: %d", temp_pids[i]);
        write_message(buffer);
    }

    //finally it reset the process count
    processes = 0;

    return TRUE;
}

/*********************************************************************************/
/* Function name: calculate_pi													                           */
/* Description:	Calcul 						          							                       */
/* Parametres:	None																				                       */
/*		      	  The global variables MaximError i  N error maxim allowed */
/*              the number process montecarlo                              */		
/*********************************************************************************/

void calculate_pi(void) {
    if (processes == 0) {
        return;
    }

    int totalPoints = 0, pointsInside = 0;
    double currentError = 0.0;
    char writeBuffer[MAXSTR], readBuffer[MAXSTR], message[MAXSTR];

    while (currentError > MaximError || totalPoints == 0) {
        //this will send the number of points inside the circle inside the square of each process
        snprintf(writeBuffer, sizeof(writeBuffer), "%d\n", T); //i used snprintf for security reasons and better performance
        for (int i = 0; i < processes; i++) {
            if (write(pipesOut[i][1], writeBuffer, strlen(writeBuffer)) == -1) {
                perror("Error writing to pipeOut");
                return;
            }
        }

        //it reads the results of each process 
        for (int i = 0; i < processes; i++) {
            int bytesRead = 0;
            message[0] = '\0';
            while ((bytesRead = read(pipesIn[i][0], readBuffer, MAXSTR - 1)) > 0) {
                readBuffer[bytesRead] = '\0';
                strncat(message, readBuffer, bytesRead);
                if (readBuffer[bytesRead - 1] == '\n') break;
            }
            if (bytesRead < 0) {
                perror("Error reading from pipeIn");
                return;
            }
            pointsInside += atoi(message);
            totalPoints += T;
        }

        //finally the calcs of pi based on the calcs contained in the document of the uoc for this exercise
        double piEstimate = 4.0 * pointsInside / totalPoints;
        currentError = 4.0 * sqrt((double)pointsInside / totalPoints * (1.0 - (double)pointsInside / totalPoints) / totalPoints);
        snprintf(writeBuffer, sizeof(writeBuffer), "Total points: %d, Points inside: %d, Estimated Pi: %f, Current Error: %f\n", totalPoints, pointsInside, piEstimate, currentError);
        write_message(writeBuffer);
    }
}


/*********************************************************************************/
/* Function name: finish                                                        */
/* Description:       Kill all the process and finish                            */
/* Parametres:       None                                                         */
/*********************************************************************************/

void finish(void)
{
    //this will call the function remove montecarlo in order to delete all the process ongoing
    for (int i = 0; i < DMAX_PROCESSES; i++) {
        if (pids[i] != 0) {
            //and this sends the signal using sigterm of end to the process
            if (kill(pids[i], SIGTERM) == -1) {
                perror("Error killing process");
            }

            //this will waits till the end of the process 
            if (waitpid(pids[i], NULL, 0) == -1) {
                perror("Error waiting for process termination");
            }

            //this will close the pipes 
            close(pipesOut[i][1]);
            close(pipesIn[i][0]);

            //will reset the pid in the original array 
            pids[i] = 0;
        }
    }

    //to reset the process count  
    processes = 0;

    //to clean all the pipes using memset to fullfit the array of natural numbers with -1, more "clean" than a loop under my point of view
    memset(pipesOut, -1, sizeof(pipesOut));
    memset(pipesIn, -1, sizeof(pipesIn));
}


int main(int argc, char* argv[])
{
  char miss[80];
	float x1,x2;
  	
	initialize();

	for (;;) {
		switch (menu()) {
			case 1:
				/* Create Process montecarlo (call function) */ 
				if (!create_processes_montecarlo())
					if (write (2, "Error creating montecarlo\n", strlen("Error creating montecarlo\n")) < 0)
							error("Creating crear montecarlo process");
				break;
				
			case 2:
				/* Remove montecarlo process(call function) */ 
				if (!remove_proces_montecarlo())
					if (write (2, "Error removing montecarlo\n", strlen("Error removing montecarlo\n")) < 0)
						error("removing montecarlo process");
				break;
				
			case 3:
				/*  Calculate(call a funtion) */ 
				calculate_pi();
				break;
				
			case 4:
				/* finish, kill all the processes montecarlo (call a funtion)*/
				finish();
				exit (0);
		}
	}
}

