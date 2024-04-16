#ifndef _QUEUE_H_
#define _QUEUE_H_

#include "common.h"

/** define queue to remove node with minimum value */
#define MINVALUE   0
/** define queue to remove node with maximum value */
#define MAXVALUE   1
/** define queue to solve ambiguity by FIFO */
#define FIFOBREAK  0
/** define queue to solve ambiguity by LIFO */
#define LIFOBREAK  1
/** define maximum size of the queue*/
#define QSIZE      65536

#define SetTieBreak(a,b) a->C.tiebreak=b
#define SetRemovalPolicy(a,b) a->C.removal_policy=b

//typedef struct fifo {
//    /** Array of elements */
//    int* FIFO;
//    /** Maximum number of elements of the queue (array size) */
//    int n;
//    /** Index of the first element of the queue */
//    int first;
//    /** Index of the last element of the queue */
//    int last;
//    /** Array of flags, one for each element from the queue */
//    char* color;
//} FIFO;

typedef struct fifo {
    int* FIFO;     // Array of elements
    int* weights;  // Array of weights
    int n;         // Maximum number of elements of the queue (array size)
    int first;     // Index of the first element of the queue
    int last;      // Index of the last element of the queue
    char* color;   // Array of flags, one for each element from the queue
} FIFO;

typedef struct gqnode {
    int  next;  /* next node */
    int  prev;  /* prev node */
    char color; /* IFT_WHITE=0, IFT_GRAY=1, IFT_BLACK=2 (IFT_WHITE=never inserted, IFT_GRAY=inserted,
  IFT_BLACK=removed)*/
} GQNode;

typedef struct gdoublylinkedlists {
    GQNode* elem;  /* all possible doubly-linked lists of the circular queue */
    int nelems;  /* total number of elements */
    int* value;   /* DEPRECATED: the value of the nodes in the graph.*/

    void* value_data;
    int (*value_function)(struct ift_gqueue* Q, int p);
} GDoublyLinkedLists;

typedef struct gcircularqueue {
    int* first;   /* list of the first elements of each doubly-linked list */
    int* last;    /* list of the last  elements of each doubly-linked list  */
    int  nbuckets; /* number of buckets in the circular queue */
    int  minvalue;  /* minimum value of a node in queue */
    int  maxvalue;  /* maximum value of a node in queue */
    char tiebreak; /* 1 is LIFO, 0 is FIFO (default) */
    char removal_policy; /* 0 is MINVALUE and 1 is MAXVALUE */
} GCircularQueue;

typedef struct gqueue { 

    GCircularQueue C;
    GDoublyLinkedLists L;
} GQueue;


// GQueue
int default_value_function(GQueue* Q, int p);
GQueue* CreateGQueue(int nbuckets, int nelems, int* value);
GQueue* CreateGQueueWithCostFunction(int nbuckets, int nelems, void* value, int (*cost_function)(GQueue* Q, int p));
void ResetGQueue(GQueue* Q);
int EmptyGQueue(GQueue* Q);
int RemoveGQueue(GQueue* Q);
void DestroyGQueue(GQueue** Q);
GQueue* GrowGQueue(GQueue** Q, int nbuckets);
void InsertGQueue(GQueue** Q, int elem);
void RemoveGQueueElem(GQueue* Q, int elem);

// FIFO
FIFO* CreateFIFO(int n);
void DestroyFIFO(FIFO** F);
char InsertFIFO(FIFO* F, int elem, int weight);
int RemoveFIFO(FIFO* F);
bool EmptyFIFO(FIFO* F);
bool FullFIFO(FIFO* F);
void ResetFIFO(FIFO* F);
int ColorFIFO(FIFO* F, int pos);
bool IsInFIFO(FIFO* F, int elem);
void UpdateWeightFIFO(FIFO* F, int elem, int new_weight);
#endif