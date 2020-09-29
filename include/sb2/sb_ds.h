#ifndef SBDS_MODULE
#define SBDS_MODULE

#include <stdint.h>

// Makes new pointer to any object in the heap.
#define newptr(type)        calloc(1,sizeof(type))
// Makes new array of an object in the heap.
#define newarr(type,num)    calloc(num,sizeof(type))
// New string of length x. Heap allocated.
#define newstr(len)         calloc(len,1)
// New wide string of length x. Heap allocated.
#define newwstr(len)		calloc(len,2)
// More idiomatic way to get the list functions from the Datastructures function-only struct.
#define Lists   Datastructures.List
// Prints a pointer to the console.
#define printptr(ptr)       printf("%p\n",(void *)ptr)

typedef struct _listnode {
	void *data;
	struct _listnode *prev, *next;
} ListNode;

typedef struct _sblist {
	ListNode *head, *current, *tail;
	uint16_t size;
} sbList;

// The Datastructures struct. This struct has all the functions for all datastructures availible for saucebot to use. They are generic data structures, meaning that any type can be used in them, as long as the user enforces that typing.
typedef struct {

	struct {
		// Creates a new list.
		sbList *( *New )(void);
		// Adds the specified item to the end of the list.
		void ( *Add )(sbList *, void *);
		// Removes the item from the specified index and returns it. If the index is out of bounds, this will return NULL.
		void *( *Remove )(sbList *, uint16_t);
		// Adds an item at the specified index. If the index is out of bounds(index > size), this will do nothing.
		void ( *AddAt )(sbList *, uint16_t, void *);
		// Gets an item from the list at the specified index.
		void *( *Get )(sbList *, uint16_t);
		// Sets an item in the list at the specified index.
		void ( *Set )( sbList *, uint16_t, void * );
		// Deletes a list from memory. The second parameter specifies if you want to also delete the items in the list (0 for no deletion, nonzero for deletion).
		void ( *Delete )( sbList * );
		// Gets the size of a list.
		uint16_t ( *Size )(sbList *);
	} List;

} DatastructuresStruct;

extern DatastructuresStruct Datastructures;

// Initializes data structure functions so that `Datastructures` can be used to make and manage data structures. Call this before calling initGUI().
void initDS(void);

// Creates a new list.
sbList *newlist(void);

// Adds an item to the end of the list.
void addtoendoflist(sbList *, void *);

// Removes an item from the list at the specified index. If the index is out of bounds, this function will return NULL.
void *removefromlist(sbList *, uint16_t);

// Adds an item to the list at the specified index. If the index is out of bounds, this function will do nothing.
void addtolistatindex(sbList *, uint16_t, void *);

// Gets an item from the list at the specified index. If the index is out of bounds, this function returns NULL.
void *getfromlist(sbList *, uint16_t);

void setiteminlist( sbList *, uint16_t, void * );

void deletelist( sbList * );

// Size of the list.
uint16_t sizeoflist(sbList *);

#endif