#include <stdlib.h>
#include <stdio.h>
#include "pcb.h"


typedef struct os{
	int step; // which step is the os currently in
	PCB* running; //which process is running at the moment
	int q; // max time for the process what is actualy running
	ListHead* IO; //list of processes currently in an IO status
	ListHead* toArrive; //list of processes that shoud still arrive in the os
	ListHead* waiting[4]; //array of lists of processes in witing status
}OS;


OS* OS_init(int argc, char ** argv,FILE* f); //initialize the os; 
int OS_step(OS* os,FILE* f); //execute a single step in the os
void OS_print_info(OS* os,FILE*f);

void OS_free(OS* os);
int check_end(OS* os); //check if every process have ended
void new_arrivals(OS* os); // inserts for each step the newly arrived processes in thir right queue

void io(OS* os);//manages the IO
void insert_waiting(OS* os, PCB* p); //inserts a process in the waiting lists
void waiting(OS* os); //manage the waiting queues
int get_next_running(OS* os); //scheduling
int burst_end(OS*os);
int quantum_end(OS* os);