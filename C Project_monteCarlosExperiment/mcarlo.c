#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include "rutines.h"

void handle_signal(int signum);         
void initialize();                     
void process_iteration();               
void calculate_and_send_points(int iterations);  
int count_points_in_circle(int iterations);      
double generate_random_double();        

int main(int argc, char* argv[]) {
    initialize();   //this will initiate the process 

    while (1) {     //i created a infinite loop to process the iterations
        process_iteration();  //this will execute the iterations of the process
    }
}

void handle_signal(int signum) {
    if (signum == SIGTERM) {   // this will handle the sigterm to exit the process
        write_message("SIGTERM received. Exiting process.");  // and will show the message of ending
        exit(EXIT_SUCCESS);    // and finally end the process
    }
}

void initialize() {
    if (signal(SIGTERM, handle_signal) == SIG_ERR) {  // this is to customize the sigterm message
        error("signal");  // and will show a error message if it fails
    }

    srand(time(NULL) ^ (getpid() << 16));  // it will initiate the seed with random numbers via using the currently time and the pid of the process 

    write_message("Montecarlo process initialized.");  
}

void process_iteration() {
    char buffer[MAXSTR];   // this buffer will read the chanins 
    int iterations;

    //this will read the number of iterations from stdin
    read_string("", buffer);   
    iterations = atoi(buffer); //this con convert the chain to natural number

    //this will calculate the points and send the results
    calculate_and_send_points(iterations);   
}

void calculate_and_send_points(int iterations) {
    int points_in_circle = count_points_in_circle(iterations); //this will count the points inside the circle
    char buffer[MAXSTR];

    sprintf(buffer, "%d\n", points_in_circle); //to convert the results into a chain 
    write_string("", buffer);  
}

int count_points_in_circle(int iterations) {
    int points_in_circle = 0;

    for (int i = 0; i < iterations; i++) {    //to do the number of iterations that has been specify from main
        double x = generate_random_double();  //to generate the coordenate
        double y = generate_random_double();  
        if (x * x + y * y <= 1) {             //this is to check if the point is inside the circle 
            points_in_circle++;               //and increment the count of points 
        }
    }

    return points_in_circle;  
}

double generate_random_double() {
    return (double)rand() / (RAND_MAX + 1.0); //this will generate a random number inside the range
}
