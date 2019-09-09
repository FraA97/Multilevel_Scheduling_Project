#include "pcb.h"

PCB * PCB_init(){
	PCB* pcb = (PCB*) malloc(sizeof(PCB));
	pcb->pid = pcb->arrival_time = pcb->priority_level = pcb->in_status = -1;
	pcb->events = List_init();
	return pcb;
}

Event * Event_create(Resource r, int d){
	Event* e = (Event*) malloc(sizeof(Event));
	e->res = r;
	e->duration = d;
	return e;
}


void PCB_print(PCB* pcb, FILE* f){
	fprintf(f,"pid: %d\n",pcb->pid);
	fprintf(f,"arrival_time: %d\n",pcb->arrival_time);
	fprintf(f,"priority_level: %d\n",pcb->priority_level);
	if(!isEmpty(pcb->events)){
		fprintf(f, "EVENTI:\n" );
		Event_list_print(pcb->events,f);
	}

	fprintf(f, "in status: %d\n",pcb->in_status );
}


void PCB_list_print(ListHead* list, FILE* f){
	PCB* p = (PCB*)list->head;
	for(int i=0;i < list->lenght; i++){
		PCB_print_info(p,f);
		fprintf(f, "\n");
		p = (PCB*)p->list.next;
		if(p==NULL)
			return;
		
	}
}


void Event_print(Event* e,FILE* f){
	char* type;
	if(e->res == CPU)
		type = "CPU";
	else
		type = "IO";
	fprintf(f,"%s for %d\n",type,e->duration);
}

void Event_list_print(ListHead* list,FILE* f){
	Event * e = (Event*) list->head;
	for(int i=0; i < list->lenght; ++i) {
		Event_print(e,f);
		fprintf(f, "\n");
		e = (Event*)e->list.next;
		if(e == NULL)
			return;
	}
}