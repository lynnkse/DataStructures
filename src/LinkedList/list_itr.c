#include "../../inc/intern_list.h"
#include "../../inc/list_itr.h"
#include <stdlib.h>

ListItr ListItr_Begin(const List* _list)
{
	if(NULL == _list)
	{
		return NULL;
	}
	
	return (ListItr) (_list->m_head).m_next;/*changed to next here*/
}

ListItr ListItr_End(const List* _list)
{
	if(NULL == _list)
	{
		return NULL;
	}

	return (ListItr) &(_list->m_tail);
}

int ListItr_Equals(const ListItr _a, const ListItr _b)
{	
	return (_a == _b ? 1 : 0);
}

ListItr ListItr_Next(ListItr _itr)
{
	if(NULL == _itr)
	{
		return NULL;
	}
	
	return ((Node*)_itr)->m_next == NULL ? _itr : (ListItr) (((Node*)_itr)->m_next);
}

ListItr ListItr_Prev(ListItr _itr)
{
	if(NULL == _itr)
	{
		return NULL;
	}
	
	return ((Node*)_itr)->m_prev == NULL ? _itr : (ListItr) (((Node*)_itr)->m_prev);
}

void* ListItr_Get(ListItr _itr)
{
	if(NULL == _itr)
	{
		return NULL;
	}
	
	return (ListItr) (((Node*)_itr)->m_item);
}

void* ListItr_Set(ListItr _itr, void* _element)
{
	void* item;
	
	if(NULL == _itr || NULL == _element) /*there shouldn't be any NULL items according to the list API*/
	{
		return NULL;
	}

	if(NULL == ((Node*)_itr)->m_next || NULL == ((Node*)_itr)->m_prev)
	{
		return NULL;
	}	

	item = ((Node*)_itr)->m_item;
	((Node*)_itr)->m_item = _element;
	
	return item;
}

ListItr ListItr_InsertBefore(ListItr _itr, void* _element)
{
	Node* newNode;
	
	if(NULL == _itr || NULL == _element || NULL == ((Node*)_itr)->m_prev) 
	{
		return NULL;
	}
	
	newNode = (Node*) malloc(sizeof(Node));
	if(NULL == newNode)
	{
		return NULL;
	}
	newNode->m_item = _element;
	newNode->m_next = (Node*)_itr;
	newNode->m_prev = ((Node*)_itr)->m_prev;
	newNode->m_prev->m_next = newNode;
	newNode->m_next->m_prev = newNode;
	
	return (ListItr) newNode;
}	

void* ListItr_Remove(ListItr _itr)
{
	void* item;
	if(NULL == _itr)
	{
		return NULL;
	}
	
	item = ((Node*)_itr)->m_item;
	
	((Node*)_itr)->m_next->m_prev = ((Node*)_itr)->m_prev;
	((Node*)_itr)->m_prev->m_next = ((Node*)_itr)->m_next;

	free(_itr);
	
	return item;
}









































































