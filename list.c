#include "list.h"
#include <stdlib.h>

ListHead* List_init(){
	ListHead * list = (ListHead*)malloc(sizeof(ListHead));
	list-> head = NULL;
	list->tail = NULL;
	list->lenght = 0;
	return list;
}

ListElem* pop(ListHead* list){
	ListElem* res = list->head;
	list->head = res->next;
	list->head->prev = NULL;
	list->lenght--;
	res->next = NULL;
	return res; 
}

int pushFront(ListHead* list, ListElem* elem){
	if (elem->next != NULL){
		printf("impossibile inserire un elemento che già si trova in un'altra lista");
		return -1;
	}
	elem->next = list->head;
	list->head->prev = elem;
	list->head = elem;
	list->lenght ++;
	return 1;
}

int pushBack(ListHead* list, ListElem* elem){
	if (elem->next != NULL){
		printf("impossibile inserire un elemento che già si trova in un'altra lista");
		return -1;
	}
	list->tail->next = elem;
	elem->prev = list->tail;
	list->tail = elem;
	list->lenght ++;
	return 1;
}

int isEmpty(ListHead* list){
	return list-lenght!=0;
}

ListElem* detach(ListHead* list, ListElem* elem){
	ListElem* current = list->head;
	for(int i=0; i< list->lenght; i++){
		if(current == elem){
			elem->prev->next = elem->next;
			elem->next->prev = elem->prev;
			elem->next = NULL;
			elem->prev = NULL;
			return elem;
		}
	}
	return NULL;
}