#include <stdio.h>
#include "os.h"

void main(int argc, char ** argv){
	if(argc < 2){
		printf("ERRORE: passare almeno un nome di un file");
		return;
	}

	FILE * f = fopen("results.txt","w");
	OS* os = OS_init(argc,argv,f);	
	printf("os initialized\n");
	while(OS_step(os,f)!= 0){
		printf("next step\n");
	}
	OS_free(os);
	fclose(f);
	printf("completato, i risultati si trovano nel file results.txt\n");
}
