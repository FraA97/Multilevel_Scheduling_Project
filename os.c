#include "os.h"
const int quantus[4]= {4,8,16,255}; //array of quantum for each waiting line
const int max_waiting[4] = {255,128,64,32};


OS* OS_init(int argc, char ** argv,FILE* f){
	OS* os = (OS*)malloc(sizeof(OS));
	os->step = -1;
	os->running = NULL;
	os->q = -1;
	os->IO = List_init();
	os->toArrive = List_init();
	for(int j = 1; j<argc; j++){
		PCB * p = Process_create(argv[j]);
		pushBack(os->toArrive,(ListElem*)p);
		fprintf(f,"creato processo da %s\n",argv[j]);
		PCB_print_info(p,f);
		fprintf(f,"\n");
	}
	for(int i = 0; i<4; i++)
		os->waiting[i] = List_init();
	return os;
	os->running_event = NULL;

}


int OS_step(OS* os,FILE * f){
	fprintf(f,"************************************************************************************\n");
	fprintf(f,"STEP: %d \n",os->step);
	OS_print_info( os,f); 

	os->step ++;
	//check end
	if(check_end(os) == 1){ 
		fprintf(f,"END");
		return 0;
	}

	//check for new process
	new_arrivals(os); //TODO
	

	//check running process
	if(os->running == NULL ){
		printf("getting next running\n");
		if(get_next_running(os) == 0) //TODO
			printf("no process running\n");
		else {
			os->running->in_status = 0;
			printf("got next running: %d\n",os->running->pid);
		}
	}

	else{
		os->running->in_status ++;

		printf("running %d", os->running->pid);	
		printf("for %d\n", os->running->in_status);

		Event* e=(Event*) os->running->events->head;
		printf("running has a burst %d long\n",e->duration);

	 	if(os->running->in_status >= os->q){
	 		if(quantum_end(os)==0) //TODO
				printf("no process running\n");
			else printf("got next running\n");
		}

	 	else if( os->running->in_status >= e->duration){
			if(burst_end(os)== 0) //TODO
				printf("no process running\n");
			else {
				os->running->in_status = 0;
				printf("got next running\nos->running->in status = %d\n",os->running->in_status );
			}	
		}
	 	
	 	else
	 		printf("not changing running\n");
		
	}
	//manage the waiting lists
	waiting(os); //TODO

	// manage the IO
	io(os); //TODO

	return 1;
}


void OS_print_info(OS* os,FILE*f){
	//print running
	if(os->running == NULL)
		fprintf(f,"NO PROCESS RUNNING\n");
	else{
		fprintf(f,"RUNNING:\n");
		PCB_print_info(os->running, f);
		fprintf(f,"q: %d\n",os->q);
	}

	//print waiting
	for(int i=0; i<4; i++){
		if(!isEmpty(os->waiting[i])){
			fprintf(f, "WAITING %d\n",i );
			PCB_list_print_info(os->waiting[i],f);
		}
		else
			fprintf(f,"NO PROCESS IN WAITING LIST %d\n",i);
	}
	//print IO
	if(!isEmpty(os->IO)){
		fprintf(f, "IO:\n" );
		PCB_list_print_info(os->IO,f);
	}
	else
		fprintf(f,"NO PROCESS IN IO LIST \n");

	//print toArrive
	if(!isEmpty(os->toArrive)){
		fprintf(f,"YET TO ARRIVE:\n");
		PCB_list_print_info(os->toArrive,f);
	}
	else
		fprintf(f,"ALL PROCESSES HAVE ARRIVED \n");
}


int check_end(OS* os){
	if(os->step <= 1)
		return 0;
	for(int i= 0; i<4; i++){
		if(os->waiting[i]->lenght >0)
			return 0;
	}
	if(os->running != NULL) return 0;
	if(os->IO->lenght > 0) return 0;
	if(os->toArrive->lenght > 0) return 0;
	return 1;
}

void insert_waiting(OS* os, PCB* p){
	printf("insert_waiting\n");
	p->in_status = 0;
	if(p->priority_level<0 || p->priority_level>3){
		printf("errore in insert_waiting\n");
		return;
	}
	pushBack(os->waiting[p->priority_level],(ListElem*) p);
	printf("process %d in waiting %d\n",p->pid,p->priority_level);
}

void new_arrivals(OS* os){
	if(!isEmpty(os->toArrive)){
		PCB* aux = (PCB*)os->toArrive->head;
		PCB* next = (PCB*)aux->list.next;
		while(aux){
			
			if(aux->arrival_time == os->step){
				printf("process %d has arrived at the %d step\n",aux->pid, os->step);
				detach(os->toArrive,(ListElem*) aux);
				insert_waiting(os,aux);
			}
			aux = next;
			if(aux == NULL)
				return;
			next = (PCB*)next->list.next;
		}
	}
}


void io(OS* os){
	if(!isEmpty(os->IO)){
		printf("io\n");
		PCB* aux = (PCB*)os->IO->head;
		PCB* next = (PCB*) aux->list.next;
		while(aux){
			printf("%d in IO for %d\n",aux->pid,aux->in_status);
			Event * e = (Event*)aux->events->head;
			Event_print(e,stdout);
			if(aux->in_status == e->duration){ //if its burst has ended
				printf("proces %d has ended its io burst\n",aux->pid);
				e = (Event*)pop(aux->events);
				Event_free(e);

				if(!isEmpty(aux->events)){ //the process still has some bursts to be done
					PCB* toWaiting = (PCB*) detach(os->IO,(ListElem*) aux);
					insert_waiting(os, toWaiting);

				}

				else{  //the process is ended
					PCB* ended = (PCB*) detach(os->IO, (ListElem*)aux);
					printf("process %d has ended\n",ended->pid);
					PCB_free(ended);				
				}

			}

			aux->in_status++;
			aux = next;
			if(aux == NULL)
				return;
			next =(PCB*) next->list.next;
		}

	}
}


void waiting(OS* os){

	for(int i=1; i<4; i++){ //for each waiting line

		if(!isEmpty(os->waiting[i])){
			printf("checking waiting %d\n",i );
			PCB* aux = (PCB*)os->waiting[i]->head; 
			PCB* next = (PCB*)aux->list.next;
			while(aux){ //for each process in the specific waiting line

				if(aux->in_status == max_waiting[i]) { //if the process has been in the waiting queue for to long
					printf("to much waiting for process %d in waiting %d",aux->pid,i);
					PCB* toAdvance = aux;
					toAdvance = (PCB*)detach(os->waiting[i],(ListElem*)toAdvance);
					toAdvance->in_status = 0;
					toAdvance->priority_level --; //we change the process priority to take it to a higher priority queue
					insert_waiting(os, toAdvance);
				}
				else{//process is ok in its waiting line
					aux->in_status ++;
					printf("process %d in waiting %d for %d\n",aux->pid,i,aux->in_status);
				}

				aux = next;
				if(aux == NULL)
					return;
				next = (PCB*) next->list.next;
			}
		}
	}
}


int get_next_running(OS*os){
	for(int i = 0; i<4; i++){
		if(!isEmpty(os->waiting[i])){
			PCB* next = (PCB*)pop(os->waiting[i]);
			next->in_status == 0;
			os->q = max_waiting[i];
			os->running = next;
			return 1;
		}
	}
	return 0;
}

int burst_end(OS*os){
	printf("burst has ended\n");
	pop(os->running->events);
		if(!isEmpty(os->running->events)){ //the process has still some bursts to be done => we put it in IO queue
			PCB * toIO = os->running;
			toIO->in_status = 0;
			pushBack(os->IO, (ListElem*)toIO);
			os->running = NULL;
			printf("proces %d in IO\n",toIO->pid);
		}
		else{ //the process has ended
			PCB* ended = os->running;
			os->running = NULL;
			printf("process %d has ended\n",ended->pid );
			PCB_free(ended);
		}
		return get_next_running(os);
}

int quantum_end(OS * os){ //we have to put the running process in a lower priority waiting list
	printf("quantum has ended\n");
	PCB * toWaiting = (PCB*)os->running;
	os->running = NULL;
	if(toWaiting->priority_level<3)
		toWaiting->priority_level ++;
	//we have to "take out" the part of the burst that has been consumed
	Event* ev = (Event*)toWaiting->events->head;
	ev->duration -= os->q;
	toWaiting->in_status = 0;
	insert_waiting(os, toWaiting);
	//we get a new running process
	return get_next_running(os);
}