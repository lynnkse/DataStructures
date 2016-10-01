#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "../../inc/queue.h"
#include <semaphore.h>
#include "../../inc/ADTDefs.h"

#define SIZE 5
#define MAGIC_NUMBER 0xDEADBEEF

struct IPCQueue
{
	size_t m_magicNum;	
	size_t m_size;
	int m_head;
	int m_tail;
	size_t m_nItems;	
	pthread_mutex_t m_mutex;
	pthread_cond_t m_cond_full;
	pthread_cond_t m_cond_empty;
	size_t m_full;
	size_t m_empty;
	int m_vec[SIZE];
};


void IPCQueueCreate(IPCQueue* _IPCQueue)
{
	pthread_mutexattr_t mattr;
    pthread_mutexattr_init(&mattr);
    pthread_mutexattr_settype(&mattr, PTHREAD_PROCESS_SHARED);
	pthread_condattr_t attrcond;
	pthread_condattr_init(&attrcond);
	pthread_condattr_setpshared(&attrcond, PTHREAD_PROCESS_SHARED);
    
	if(_IPCQueue->m_magicNum != MAGIC_NUMBER)
	{
		_IPCQueue->m_head = 0;
		_IPCQueue->m_tail = -1;
		_IPCQueue->m_nItems = 0;
		_IPCQueue->m_magicNum = MAGIC_NUMBER;
		_IPCQueue->m_size = SIZE;
		sem_init(&_IPCQueue->m_full, 1, 0);
		sem_init(&_IPCQueue->m_empty, 1, SIZE);
		pthread_mutex_init(&_IPCQueue->m_mutex, &mattr);
		pthread_cond_init(&_IPCQueue->m_cond_full, &attrcond);
		pthread_cond_init(&_IPCQueue->m_cond_empty, &attrcond);
		_IPCQueue->m_full = 0;
		_IPCQueue->m_empty = SIZE;
	}
}

void IPCQueueDestroy(IPCQueue* _IPCQueue)
{
	_IPCQueue->m_magicNum = 0;
}

ADTErr IPCQueueInsert(IPCQueue* _IPCQueue, int _item)
{
	/*if(_IPCQueue->m_size == _IPCQueue->m_nItems)
	{
		return ERR_OVERFLOW;	
	}*/
	
	pthread_mutex_lock(&_IPCQueue->m_mutex);

	while(_IPCQueue->m_empty == 0)
	{
		pthread_cond_wait(&_IPCQueue->m_cond_empty, &_IPCQueue->m_mutex);
	}

	_IPCQueue->m_full++;
	_IPCQueue->m_empty--;

	_IPCQueue->m_vec[(_IPCQueue->m_tail + 1) % _IPCQueue->m_size] = _item;
	_IPCQueue->m_nItems++;
	printf("Inserting. NumOfItems: %zu\n", _IPCQueue->m_nItems);
	_IPCQueue->m_tail = (_IPCQueue->m_tail + 1) % _IPCQueue->m_size;

	pthread_cond_broadcast(&_IPCQueue->m_cond_full);

	pthread_mutex_unlock(&_IPCQueue->m_mutex);
	
	
	
	return ERR_OK;
}

ADTErr IPCQueueRemove(IPCQueue* _IPCQueue, int* _item)
{
	/*if(_IPCQueue->m_nItems == 0)
	{
		return ERR_UNDERFLOW;
	}*/
	
	
	pthread_mutex_lock(&_IPCQueue->m_mutex);

	while(_IPCQueue->m_full == 0)
	{
		pthread_cond_wait(&_IPCQueue->m_cond_full, &_IPCQueue->m_mutex);
	}

	_IPCQueue->m_full--;
	_IPCQueue->m_empty++;

	*_item = _IPCQueue->m_vec[_IPCQueue->m_head];
	_IPCQueue->m_head = (_IPCQueue->m_head + 1) % _IPCQueue->m_size;
	_IPCQueue->m_nItems--;
	
	printf("Removing. NumOfItems: %zu\n", _IPCQueue->m_nItems);
	
	pthread_cond_broadcast(&_IPCQueue->m_cond_empty);

	pthread_mutex_unlock(&_IPCQueue->m_mutex);
	
	
	
	return ERR_OK;
}

int IPCQueueIsEmpty(IPCQueue* _IPCQueue)
{
	if(_IPCQueue->m_size == 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void IPCQueuePrint(IPCQueue* _IPCQueue)
{
	int i;
	printf("\nItems: %zu\n", _IPCQueue->m_nItems);	
	for(i = 0; i < _IPCQueue->m_nItems; i++)
	{
		printf("%d ", _IPCQueue->m_vec[i]);
	}
}

size_t GetIPCQueueSize()
{
	return sizeof(IPCQueue);
}





















