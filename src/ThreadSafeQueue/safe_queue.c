#include "../../inc/safe_queue.h"
#include <stdlib.h>
#include <stddef.h>
#include "../../inc/gen_queue.h"
#include<pthread.h>
#include "../../inc/ADTDefs.h"

#define MAGIC_NUMBER 0xDABAD00

struct SafeQueue
{
	Queue* m_queue;
	pthread_mutex_t m_mutex;
	pthread_cond_t m_cond_empty;
	pthread_cond_t m_cond_full;
	int m_full;
	int m_empty;
	int m_cap;
	size_t m_magicNumber;
};

SafeQueue* SafeQueueCreate(size_t _size)
{
	SafeQueue* newQueue;
	
	newQueue = (SafeQueue*) malloc(sizeof(SafeQueue));
	
	if(NULL == newQueue)
	{
		return NULL;
	}
	
	pthread_cond_init(&newQueue->m_cond_empty, NULL);
	pthread_cond_init(&newQueue->m_cond_full, NULL);
	pthread_mutex_init(&newQueue->m_mutex, NULL);
	
	newQueue->m_queue = QueueCreate(_size);
	
	if(NULL == newQueue->m_queue)
	{
		free(newQueue);
		return NULL;
	}
	
	newQueue->m_full = 0;
	newQueue->m_cap = _size;
	newQueue->m_empty = _size;
	newQueue->m_magicNumber = MAGIC_NUMBER;
	
	return newQueue;
}

void SafeQueueDestroy(SafeQueue* _queue, void(*_destroyElement)(void*))
{
	if(NULL != _queue && MAGIC_NUMBER == _queue->m_magicNumber)
	{
		QueueDestroy(_queue->m_queue, _destroyElement);
		pthread_mutex_destroy(&_queue->m_mutex);
		pthread_cond_destroy(&_queue->m_cond_empty);
		pthread_cond_destroy(&_queue->m_cond_full);
		_queue->m_magicNumber = 0;
		free(_queue);
	}
}

ADTErr SafeQueueInsert(SafeQueue* _queue, void* _item)
{
	ADTErr result = ERR_OK;
	
	if(NULL == _queue)
	{
		return ERR_NOT_INITIALIZED;
	}
	
	pthread_mutex_lock(&_queue->m_mutex);
	
	while(_queue->m_empty == 0)
	{
		pthread_cond_wait(&_queue->m_cond_empty, &_queue->m_mutex);
	}
	
	--_queue->m_empty;
	++_queue->m_full;
	
	result = QueueInsert(_queue->m_queue, _item);
	
	pthread_cond_broadcast(&_queue->m_cond_full);
	
	pthread_mutex_unlock(&_queue->m_mutex);
	
	return result;
}

ADTErr SafeQueueRemove(SafeQueue* _queue, void** _item)
{
	ADTErr result;
	
	if(NULL == _queue || NULL == _item)
	{
		return ERR_NOT_INITIALIZED;
	}
	
	pthread_mutex_lock(&_queue->m_mutex);
	
	while(_queue->m_full == 0)
	{
		pthread_cond_wait(&_queue->m_cond_full, &_queue->m_mutex);
	}
	
	++_queue->m_empty;
	--_queue->m_full;
	
	result = QueueRemove(_queue->m_queue, _item);
	
	pthread_cond_broadcast(&_queue->m_cond_empty);
	
	pthread_mutex_unlock(&_queue->m_mutex);
	
	return result;
}

int SafeQueueIsEmpty(SafeQueue* _queue)
{
	int result = 0;
	if(NULL == _queue)
	{
		return 1;
	}
	
	pthread_mutex_lock(&_queue->m_mutex);
	result = QueueIsEmpty(_queue->m_queue);
	pthread_mutex_unlock(&_queue->m_mutex);
	
	return result;
}































