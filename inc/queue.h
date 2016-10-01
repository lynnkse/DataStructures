/**********************************************************
*	           											  *
*	 Queue.h  											  *
*    10.7.16    								          *
*	 Anton Gulyaev                                        *
*                                                         *
*	 Creation and manipulation of IPCQueue.                  *
*														  *
***********************************************************/
#ifndef __QUEUE_H__
#define __QUEUE_H__

#include "ADTDefs.h"

typedef struct IPCQueue IPCQueue;

/*Creates IPCQueue. Revieves initial size and returns pointer to the created IPCQueue. Size must be positive number. Returns NULL on error.*/
void IPCQueueCreate(IPCQueue* _IPCQueue);

void IPCQueueDestroy(IPCQueue* _IPCQueue);

/*Inserts value in IPCQueue. Recieves IPCQueue pointer and value to be inserted. Returns error codes*/
ADTErr IPCQueueInsert(IPCQueue* _IPCQueue, int _item);

/*Removes value from the IPCQueue and stores it in variable. Recieves IPCQueue pointer and pointer to the variable in which the retrieved value to be stored. Returns error codes*/
ADTErr IPCQueueRemove(IPCQueue* _IPCQueue, int* _item);

/*Returns 1 if the IPCQueue is empty, 0 otherwise. Revieves pointer to the IPCQueue*/
int IPCQueueIsEmpty(IPCQueue* _IPCQueue);

/*Prints out the IPCQueue value and internal parameters*/
void IPCQueuePrint(IPCQueue* _IPCQueue);

size_t GetIPCQueueSize();

#endif /*#ifndef __IPCQueue_H__*/
