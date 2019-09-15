
#include <stdio.h>
#include<stdlib.h>

typedef struct list_elem{
	struct list_elem* next;
	struct list_elem* prev;
}ListElem;

typedef struct list_head{
	ListElem* head;
	ListElem* tail;
	int lenght;
}ListHead;

ListHead* List_init();//initialize the list
ListElem* List_find(ListHead* head, ListElem* item);
ListElem* detach(ListHead* list, ListElem* elem);
ListElem* pop(ListHead* list);//take the first elem in the list

int pushFront(ListHead* list, ListElem* elem);//insert a new elem at the head of the list
int pushBack(ListHead* list, ListElem* elem); //insert a new elem at the end of the list
int isEmpty(ListHead* list);
void List_free(ListHead* head);