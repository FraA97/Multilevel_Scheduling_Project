#include "pcb.h"

PCB * PCB_init(){
	PCB* pcb = (PCB*) malloc(sizeof(PCB));
	pcb->pid = pcb->arrival_time = pcb->priority_level = pcb->in_status = -1;
	pcb->events = List_init();

	pcb->list.next = pcb->list.prev = NULL;
	return pcb;
}

Event * Event_create(Resource r, int d){
	Event* e = (Event*) malloc(sizeof(Event));
	e->list.next = e->list.prev = NULL;
	e->res = r;
	e->duration = d;
	return e;
}



PCB* Process_create(const char* filename){
	FILE* f = fopen(filename,"r");
	if(!f){
		printf("ERRORE: file %s non esistente",filename);
		return NULL;
	}
	PCB * p = PCB_init();
	size_t buff_size = 10;
	char* buff = (char*)malloc(sizeof(char)*buff_size);
	size_t line_lenght = 0;
	while(getline(&buff,&line_lenght,f) >0){
		int pid = -1;
		int arrival_time = -1;
		int priority_level = -1;
		int duration = -1;
		int num_tokens = 0;

		num_tokens = sscanf(buff,"PROCESS %d %d %d",&pid,&arrival_time,&priority_level);
		if(num_tokens == 3 && p->pid < 0){
			p->pid = pid;
			p->arrival_time = arrival_time;
			p->priority_level = priority_level;
			goto Next;
		}


		num_tokens = sscanf(buff,"CPU %d",&duration);
		if(num_tokens == 1){
			Event * e = Event_create(0,duration);
			pushBack(p->events,(ListElem*)e);
			goto Next;
		}



		num_tokens = sscanf(buff,"IO %d",&duration);
		if(num_tokens == 1){
			Event * e = Event_create(1,duration);
			pushBack(p->events,(ListElem*)e);
			goto Next;
		}
		Next:
		;
	}
	if(buff)
		free(buff);
	fclose(f);
	return p;
}



void PCB_print_info(PCB* pcb, FILE* f){
	fprintf(f,"pid: %d\n",pcb->pid);
	fprintf(f,"arrival_time: %d\n",pcb->arrival_time);
	fprintf(f,"priority_level: %d\n",pcb->priority_level);
	if(!isEmpty(pcb->events)){
		fprintf(f, "EVENTI:\n" );
		Event_list_print(pcb->events,f);
	}

	fprintf(f, "in status: %d\n",pcb->in_status );
}

void PCB_list_print_info(ListHead* list, FILE* f){
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
	while(e!= NULL) {
		Event_print(e,f);
		fprintf(f, "\n");
		e = (Event*)e->list.next;
		}
}


int Event_free(Event* e){
	if(e->list.next != NULL || e->list.prev != NULL){
		printf("ERRORE: impossibile fare free su un evento che si trova in una lista\n");
		return 0;
	}
	free(e);
	return 1;
}


int PCB_free(PCB* p){
	if(p->list.next != NULL || p->list.prev != NULL){
		printf("ERRORE: impossibile fare free su un processo che si trova in una lista\n");
		return 0;
	}
	if(! isEmpty(p->events)){
		printf("ERRORE: impossibile fare free su un processo che non è terminato\n");
		return 0;	
	}
	List_free(p->events);
	free(p);
	return 1;

}