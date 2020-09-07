#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "sb2/sb_ds.h"

#define TABLE_PRIME     (11)

DatastructuresStruct Datastructures;

void initDS(void) {
	Datastructures.Map.New = newmap;
	Datastructures.Map.Insert = insmap;
	Datastructures.Map.Search = searchmap;
	Datastructures.Map.Delete = delmap;
	Datastructures.Map.Destroy = destroymap;
	Datastructures.List.New = newlist;
	Datastructures.List.Add = addtoendoflist;
	Datastructures.List.Remove = removefromlist;
	Datastructures.List.AddAt = addtolistatindex;
	Datastructures.List.Get = getfromlist;
	Datastructures.List.Set = setiteminlist;
	Datastructures.List.Size = sizeoflist;
	Datastructures.List.Delete = deletelist;
}

sbMap *newmap(void) {
	sbMap *map = newptr(sbMap);
	if (map == NULL) return NULL;
	map->map = newarr(Bucket, 64);
	if (map->map == NULL) return NULL;
	map->count = 0;
	map->size = 64;
	return map;
}

void *insmap(sbMap *hashmap, void *key, void *data) {
	uint8_t idx = (TABLE_PRIME * (uint64_t)key) % hashmap->size;
	hashmap->count++;
	Bucket *toFill = hashmap->map[idx];
	if (toFill != NULL) {
		toFill->current = toFill->head;
		while (toFill->current->next != NULL)
			toFill->current = toFill->current->next;
		toFill->current->next = newptr(BucketNode);
		toFill->current->next->Items.data = data;
		toFill->current->next->Items.key = key;
		toFill->current->next->prev = toFill->current;
		toFill->current = toFill->current->next;
		toFill->tail = toFill->current;
		return data;
	} else {
		toFill = newptr(Bucket);
		hashmap->map[idx] = toFill;
		toFill->head = toFill->current = toFill->tail = newptr(BucketNode);
		toFill->current->Items.data = data;
		toFill->current->Items.key = key;
		toFill->current->next = NULL;
		toFill->current->prev = NULL;
		return data;
	}
	return NULL;
}

void *searchmap(sbMap *hashmap, void *key) {
	uint8_t idx = (TABLE_PRIME * (uint64_t)key) % hashmap->size;
	Bucket *toSearch = hashmap->map[idx];
	if (toSearch == NULL) return NULL;
	toSearch->current = toSearch->head;
	while (toSearch->current != NULL) {
		if (key == toSearch->current->Items.key)
			return toSearch->current->Items.data;
		toSearch->current = toSearch->current->next;
	}
	return NULL;
}

void *delmap(sbMap *hashmap, void *key) {
	uint8_t idx = (TABLE_PRIME * (uint64_t)key) % hashmap->size;
	Bucket *toSearch = hashmap->map[idx];
	if (toSearch == NULL) return NULL;
	toSearch->current = toSearch->head;
	while (toSearch->current != NULL) {
		if (key == toSearch->current->Items.key) {
			void *ret = toSearch->current->Items.data;
			if ( delbucketnode( toSearch, toSearch->current ) )
				hashmap->map[idx] = NULL;
			return ret;
		}
		toSearch->current = toSearch->current->next;
	}
	return NULL;
}

void destroymap( sbMap *map ) {
	uint16_t i;
	Bucket **bucketArr = map->map;
	for ( i = 0; i < map->size; ++i )
		delbucket( bucketArr[i] );
	free( bucketArr );
	free( map );
}

void delbucket( Bucket *bucket ) {
	if ( bucket == NULL )
		return;
	BucketNode *current = bucket->head;
	while ( current->next != NULL ) {
		free( current->prev );
		current = current->next;
	}
	free( current );
	return;
}

int delbucketnode( Bucket *bucket, BucketNode *node ) {
	if ( node->prev != NULL )
		node->prev->next = node->next;
	else
		bucket->head = node->next;
	
	if ( node->next != NULL )
		node->next->prev = node->prev;
	else
		bucket->tail = node->prev;
	
	free( node );
	if ( bucket->head == NULL ) {
		free( bucket );
		return 1;
	}
	
	return 0;
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
