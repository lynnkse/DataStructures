#ifndef __GEN_QUEUE_H__
#define __GEN_QUEUE_H__

/** 
 * @brief Generic heap implementation
 * 
 * @author Anton Gulyaev 
 */ 

#include <stddef.h>
#include "../inc/ADTDefs.h"

typedef struct Queue Queue;

/*Creates queue. Revieves initial size and returns pointer to the created queue. Size must be positive number. Returns NULL on error.*/
Queue* QueueCreate(size_t _size);

/*Destroys queue. Recieves pointer to the queue to be destroyed*/
void QueueDestroy(Queue* _queue, void(*_destroyElement)(void*));

/*Inserts value in queue. Recieves queue pointer and value to be inserted. Returns error codes*/
ADTErr QueueInsert(Queue* _queue, void* _item);

/*Removes value from the queue and stores it in variable. Recieves queue pointer and pointer to the variable in which the retrieved value to be stored. Returns error codes*/
ADTErr QueueRemove(Queue* _queue, void** _item);

/*Returns 1 if the queue is empty, 0 otherwise. Revieves pointer to the queue*/
int QueueIsEmpty(Queue* _queue);

#endif /*#ifndef __QUEUE_H__*/
