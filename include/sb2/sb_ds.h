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
// More idiomatic way to get the map functions from the Datastructures function-only struct.
#define Maps    Datastructures.Map
// More idiomatic way to get the list functions from the Datastructures function-only struct.
#define Lists   Datastructures.List
// Prints a pointer to the console.
#define printptr(ptr)       printf("%p\n",(void *)ptr)

// Struct for a node of a bucket, used in a map. It has a substruct called Items with data and a key, and next and prev list nodes to travel to.
typedef struct _bucketnode {
	struct {
		void *key, *data;
	} Items;
	struct _bucketnode *next, *prev;
} BucketNode;

// The bucket is one unit in the hash table array. It is a doubly linked list in order to handle collisions. It has a pointer to a current node, a head node, and a tail node.
typedef struct _bucket {
	BucketNode *current, *head, *tail;
} Bucket;

// This is a hash table. It has a bucket array for the map, and 2 integers--one for the size and another for how many items are currently in the map. Use the functions defined in Datastructures.Map to control these.
typedef struct _sbmap {
	Bucket **map;
	uint16_t size, count;
} sbMap;

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

	// Hash map functions. Create a new hash map, insert an item, search for an item, or delete an item.
	struct {
		// Creates a new map.
		sbMap *( *New )( void );
		// Inserts an item into a map. Send this an sbMap *, then the key for the item, then the actual item.
		void *( *Insert )( sbMap *, void *, void * );
		// Searches a map for an item. Send this an sbMap * and a key, and it will return the item associated with that key.
		void *( *Search )( sbMap *, void * );

		void *( *Delete )( sbMap *, void * );

		void ( *Destroy )( sbMap * );
	} Map;

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

// Function to initialize a new sbMap. This function is put into `Datastructures.Map.new` when initDS is called. Returns NULL on failiure.
sbMap *newmap(void);

// Internal insert function for `Datastructures.Map.insert`. This inserts an item into a hash map using a key. The key is the first void *. Should never return NULL, but it will return NULL on failiure. The only case for returning NULL is if calloc fails.
void *insmap(sbMap *, void *, void *);

// Internal search function for `Datastructures.Map.search`. This searches a hash map for an item (using its key) and returns that item. Returns NULL on failiure.
void *searchmap(sbMap *, void *);

// Internal delete function for `Datastructures.Map.delete`. This deletes an item from a map and returns that item. Returns NULL on failiure.
void *delmap(sbMap *, void *);

// Internal destroy function for `Datastructures.Map.Destroy`. This destroys a map and removes it from memory.
void destroymap( sbMap * );

// deletes a bucket. Does not remove its items from memory, however.
void delbucket( Bucket * );

int delbucketnode( Bucket *, BucketNode * );

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