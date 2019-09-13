#include <stdlib.h>
#include <stdio.h>
#include "list.h"

typedef enum Resource {CPU = 0, IO = 1} Resource;

typedef struct event{
	ListElem list;
	Resource res;
	int duration;
} Event;

typedef struct pcb{
	ListElem list;
	int pid;  //process' pid
	int arrival_time; //process arrival time
	int priority_level; //process current priority level
	int in_status;	//process time in its current status(for running & IO)
	ListHead*  events; //list of process events
}PCB;


PCB* PCB_init();
Event* Event_create(Resource r, int d);
PCB* Process_create(const char* filename); //creates a pcb

void PCB_print_info(PCB* pcb, FILE* f);
void PCB_list_print(ListHead* list, FILE* f);
void Event_print(Event* e,FILE* f);
void Event_list_print(ListHead* list,FILE* f);
