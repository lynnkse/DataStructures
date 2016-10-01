#include "../../inc/ADTDefs.h"
#include "../../inc/list.h"
#include "../../inc/intern_list.h"
#include "../../inc/gen_queue.h"
#include <stdlib.h>

#define MAGIC_NUMBER 0xdeadbeef

#define CHECK_AND_RETURN(check, checkAgainst,returnVal) if(checkAgainst == check) \
															{ \
															return returnVal; \
															} 

struct Queue
{
	List* m_list;
	size_t m_magicNumber;
};

static ADTErr ListErrToQueueErr(List_Result _err)
{
	ADTErr queueRes = ERR_GENERAL;
	switch(_err)
	{
		case LIST_SUCCESS:
			queueRes = ERR_OK;
			break;
		case LIST_UNINITIALIZED_ERROR:
			queueRes = ERR_GENERAL;
			break;
		case LIST_ALLOCATION_ERROR:
			queueRes = ERR_ALLOCATION_FAILED;
			break;
	    case LIST_NULL_ELEMENT_ERROR:
			queueRes = ERR_INVALID_DATA;
			break;
		case LIST_UNDERFLOW:
			queueRes = ERR_UNDERFLOW;
			break;
	}
	return queueRes;
}

Queue* QueueCreate(size_t _size)
{
	Queue* queue;
	
	queue = (Queue*) malloc(sizeof(Queue));
	
	CHECK_AND_RETURN(queue, NULL ,NULL);
	
	queue->m_list = List_Create();
	if(NULL == queue->m_list)
	{
		free(queue);
		return NULL;
	}
	
	queue->m_magicNumber = MAGIC_NUMBER;
	
	return queue;
}

void QueueDestroy(Queue* _queue, void(*_destroyElement)(void*))
{
	if(NULL != _queue && MAGIC_NUMBER == _queue->m_magicNumber)
	{
		List_Destroy(&(_queue->m_list), _destroyElement);
		_queue->m_magicNumber = 0;
		free(_queue);
	}
}

ADTErr QueueInsert(Queue* _queue, void* _item)
{
	List_Result lRes = List_PushHead(_queue->m_list, _item);
	return ListErrToQueueErr(lRes);
}

ADTErr QueueRemove(Queue* _queue, void** _item)
{
	List_Result lRes = List_PopTail(_queue->m_list, _item);
	return ListErrToQueueErr(lRes);
}

int QueueIsEmpty(Queue* _queue)
{
	CHECK_AND_RETURN(_queue, NULL , 1)
	return (List_Size(_queue->m_list) == 0 ? 1 : 0);
}










































