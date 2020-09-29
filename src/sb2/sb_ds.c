#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "sb2/sb_ds.h"

#define TABLE_PRIME     (11)

DatastructuresStruct Datastructures;

void initDS(void) {
	Datastructures.List.New = newlist;
	Datastructures.List.Add = addtoendoflist;
	Datastructures.List.Remove = removefromlist;
	Datastructures.List.AddAt = addtolistatindex;
	Datastructures.List.Get = getfromlist;
	Datastructures.List.Set = setiteminlist;
	Datastructures.List.Size = sizeoflist;
	Datastructures.List.Delete = deletelist;
}

sbList *newlist(void) {
	sbList *lst = newptr(sbList);
	lst->head = newptr(ListNode);
	lst->tail = lst->head;
	lst->current = lst->head;
	return lst;
}

void addtoendoflist(sbList *lst, void *data) {
	lst->tail->data = data;
	lst->tail->next = newptr(ListNode);
	lst->tail->next->prev = lst->tail;
	lst->tail = lst->tail->next;
	lst->size++;
}

void *removefromlist(sbList *lst, uint16_t idx) {
	uint16_t lIdx = lst->size - 1;
	if ( idx > lIdx )
		return NULL;

	ListNode *cur = lst->head;
	for ( uint16_t i = 0; i < idx; ++i )
		cur = cur->next;
	
	if ( idx > 0 && idx < lIdx ) {
		cur->prev->next = cur->next;
		cur->next->prev = cur->prev;
	} else if ( idx == 0 ) {
		cur->next->prev = NULL;
		lst->head = cur->next;
	} else {
		cur->prev->next = NULL;
		lst->tail = cur->prev;
	}

	lst->size--;
	lst->current = NULL;
	
	void *data = cur->data;
	free( cur );
	return data;
}

void addtolistatindex(sbList *lst, uint16_t idx, void *data) {
	if (idx > lst->size) return;
	lst->current = lst->head;
	for (uint16_t i = 0; i < idx; ++i) {
		lst->current = lst->current->next;
	}
	ListNode *toAdd = newptr(ListNode);
	toAdd->data = data;
	ListNode *behind = lst->current->prev;
	lst->current->prev = toAdd;
	if (behind != NULL) behind->next = toAdd;
	if (idx == 0) lst->head = toAdd;
	if (idx == lst->size) lst->tail = toAdd;
	lst->current = lst->head;
	lst->size++;
}

void *getfromlist(sbList *lst, uint16_t idx) {
	lst->current = lst->head;
	if (idx > (lst->size - 1)) return NULL;
	for (uint16_t i = 0; i < idx; ++i) {
		lst->current = lst->current->next;
	}
	return lst->current->data;
}

void setiteminlist( sbList *lst, uint16_t idx, void *data ) {
	lst->current = lst->head;
	if (idx > (lst->size - 1)) return;
	for (uint16_t i = 0; i < idx; ++i) {
		lst->current = lst->current->next;
	}
	lst->current->data = data;
	return;
}

uint16_t sizeoflist(sbList *lst) {
	return lst->size;
}

void deletelist( sbList *lst ) {
	lst->current = lst->head;
	for ( int i = 0; i < lst->size; ++i ) {
		lst->current = lst->current->next;
		free( lst->current->prev );
	}
	free( lst->current );
	free( lst );
}
