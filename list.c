#include "list.h"
#include <stdlib.h>

ListHead* List_init(){
	ListHead * list = (ListHead*)malloc(sizeof(ListHead));
	list-> head = NULL;
	list->tail = NULL;
	list->lenght = 0;
	return list;
}

ListElem* List_find(ListHead* list, ListElem* elem){
	ListElem* aux = list->head;
	while(aux){
    	if (aux==elem)
      		return elem;
    	aux=aux->next;
  	}
  	return NULL;
}

ListElem* detach(ListHead* list, ListElem* elem){
	if(List_find(list,elem) == NULL)
		return NULL;
	ListElem* prev = elem->prev;
	ListElem* next = elem->next;
	if(prev){
		prev->next = next;
	}
	if(next){
		next->prev = prev;
	}
	if(list->head == elem)
		list->head == next;
	else if(list->tail == elem)
		list->tail == prev;
	return elem;
}

ListElem* pop(ListHead* list){
	detach(list,list->head);
}

int pushFront(ListHead* list, ListElem* elem){
	ListElem* head = list->head;
	if(head == NULL){
		list->head = elem;
		list->tail = elem;
		return 1;
	}
	elem->next = list->head;
	head->prev = elem;
	list->head = elem;
	list->lenght ++;
	return 1;
}

int pushBack(ListHead* list, ListElem* elem){
	ListElem* tail = list->tail;
	if(tail == NULL){
		list->head = elem;
		list->tail = elem;
		return 1;
	}
	tail->next = elem;
	elem->prev = tail;
	list->tail = elem;
	list->lenght ++;
	return 1;
}

int isEmpty(ListHead* list){
	return list->lenght!=0;
}
