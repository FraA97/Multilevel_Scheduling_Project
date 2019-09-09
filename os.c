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