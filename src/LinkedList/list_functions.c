#include "../../inc/list_itr.h"
#include "../../inc/intern_list.h"
#include "../../inc/list.h"
#include "../../inc/list_functions.h"
#include <stdlib.h>

ListItr ListItr_FindFirst(ListItr _begin, ListItr _end, PredicateFunction _predicate, void* _context)
{
	if(NULL == _begin || NULL == _end || NULL == _predicate)
	{
		return NULL;
	}

	/*_begin = ((Node*)_begin)->m_next;*/

	while(_begin != _end)
	{
		if(_predicate(ListItr_Get(_begin), _context) != 0)
		{
			return _begin;
		}
		_begin = ((Node*)_begin)->m_next;
	}

	return NULL;
}

size_t ListItr_CountIf(ListItr _begin, ListItr _end, PredicateFunction _predicate, void* _context)
{
	size_t nElements = 0;	
	if(NULL == _begin || NULL == _end || NULL == _predicate)
	{
		return 0;
	}

	while(_begin != _end)
	{
		if(_predicate(((Node*)_begin)->m_item, _context) != 0)
		{
			++nElements;
		}
		_begin = ((Node*)_begin)->m_next;
	}

	return nElements;
}

ListItr ListItr_ForEach(ListItr _begin, ListItr _end, ListActionFunction _action, void* _context)
{
	if(NULL == _begin || NULL == _end || NULL == _action)
	{
		return NULL;
	}

	if(((Node*)_begin)->m_prev == NULL)
	{
		_begin = ((Node*)_begin)->m_next; 
	}

	while(_begin != _end)
	{
		if(_action(((Node*)_begin)->m_item, _context) == 0)
		{
			return _begin;
		}
		_begin = ((Node*)_begin)->m_next;
	}
	return ((Node*)_begin)->m_prev;
}

void ListItr_Sort(ListItr _begin, ListItr _end, LessFunction _less)
{
	Node* iterBegin = _begin;
	Node* iterEnd = _end;
	int swaps = 1;
	void* item;
	if(NULL == _begin || NULL == _end || NULL == _less)
	{
		return;
	}

	while(swaps > 0)
	{
		swaps = 0;		
		while(iterBegin != iterEnd)
		{
			if(_less(iterBegin->m_next->m_item, iterBegin->m_item))
			{
				item = iterBegin->m_item;
				iterBegin->m_item = iterBegin->m_next->m_item;
				iterBegin->m_next->m_item = item;
				swaps++;
			}
			iterBegin = iterBegin->m_next;
		}
		iterEnd = iterEnd->m_prev;
		iterBegin = _begin;
	}
}

ListItr ListItr_Splice(ListItr _dest, ListItr _begin, ListItr _end)
{
	if(NULL == _dest || NULL == _begin || NULL == _end)
	{
		return NULL;
	}

	((Node*)_begin)->m_prev->m_next = _end;
	((Node*)_end)->m_prev = ((Node*)_begin)->m_prev;

	return (ListItr) _dest;
}
/*TODO check iterator*/
static void InsertBefore(Node* _inserted, Node* _dest)
{
	_inserted->m_next = _dest;
	_inserted->m_prev = _dest->m_prev;
	_inserted->m_next->m_prev = _inserted;
	_inserted->m_prev->m_next = _inserted;
}

ListItr ListItr_Merge(ListItr _destBegin, ListItr _firstBegin, ListItr _firstEnd, ListItr _secondBegin, ListItr _secondEnd, LessFunction _less)
{
	if(NULL == _destBegin || NULL == _firstBegin || NULL == _firstEnd || NULL == _secondBegin || NULL == _secondEnd)
	{
		return NULL;
	}
	
	if(_less != NULL)
	{
		while(_firstBegin != _firstEnd && _secondBegin != _secondEnd)
		{		
			if(_less(_firstBegin, _secondBegin) != 0)
			{
				InsertBefore(_firstBegin, _destBegin);				
			}
			else
			{
				InsertBefore(_secondBegin, _destBegin);				
			}
			
			_firstBegin = ((Node*)_firstBegin)->m_next;
			_secondBegin = ((Node*)_secondBegin)->m_next;
		}
	}

	while(_firstBegin != _firstEnd)
	{
		InsertBefore(_firstBegin, _destBegin);	
		_firstBegin = ((Node*)_firstBegin)->m_next;
	}
	
	while(_secondBegin != _secondEnd)
	{
		InsertBefore(_secondBegin, _destBegin);	
		_secondBegin = ((Node*)_secondBegin)->m_next;
	}

	return _destBegin;
}

List* ListItr_Cut(ListItr _begin, ListItr _end)
{
	List* list = NULL;
	Node* temp;
	if(NULL == _begin || NULL == _end)
	{
		return NULL;
	}

	list = List_Create();
	if(NULL == list)
	{
		return NULL;
	}

	while(_begin != _end)
	{
		temp = (Node*) _begin;		
		List_PushHead(list, temp->m_item);
		temp->m_next->m_prev = temp->m_prev;
		temp->m_prev->m_next = temp->m_next;
		free(temp);
		_begin = ((Node*)_begin)->m_next;
	}

	return list;
}

List* ListItr_Unique(ListItr _begin, ListItr _end, EqualsFunction _equals)
{
	List* list = NULL;
	Node* temp;	
	if(NULL == _begin || NULL == _end || NULL == _equals)
	{
		return NULL;
	}

	list = List_Create();
	if(NULL == list)
	{
		return NULL;
	}	

	while(_begin != _end)
	{
		temp = (Node*)_begin;		
		if(_equals(_begin, ((Node*)_begin)->m_next) != 0)
		{
			List_PushHead(list, temp->m_item);
			temp->m_next->m_prev = temp->m_prev;
			temp->m_prev->m_next = temp->m_next;
			free(temp);
		}		
		_begin = ((Node*)_begin)->m_next;
	}

	return list;
}








































































