#include "../../inc/list.h"
#include "../../inc/intern_list.h"
#include <stdlib.h>

#define MAGIC_NUMBER 0xdeadbeef
#define DEALLOCATED 0



/*----------------------------------------------------------------------------*/
/*----------------------Static Functions Declarations-------------------------*/
/*----------------------------------------------------------------------------*/

static List_Result ListPop(List* _list, void** _pItem, int _isHead);
static List_Result List_Push(List* _list, void* _item, int _isHead);

/*----------------------------------------------------------------------------*/
/*----------------------API Functions Implementation--------------------------*/
/*----------------------------------------------------------------------------*/

List* List_Create(void)
{
	List* list;
	
	list = (List*) calloc(1, sizeof(List));
	
	if(NULL == list)
	{
		return NULL;
	}
	
	(list->m_head).m_next = &(list->m_tail);
	(list->m_tail).m_prev = &(list->m_head);	
	
	list->m_magicNumber = MAGIC_NUMBER;
	
	return list;
}

void List_Destroy(List** _pList, void (*_elementDestroy)(void* _item))
{
	Node* temp;
	
	if(NULL == _pList || NULL == *_pList || (*_pList)->m_magicNumber != MAGIC_NUMBER)
	{
		return;
	}
	
	temp = ((*_pList)->m_head).m_next;
	
	if(_elementDestroy != NULL)
	{
		while(temp != &((*_pList)->m_tail))
		{
			_elementDestroy(temp->m_item);
			temp = temp->m_next;
			free(temp->m_prev);
		}
	}
	else
	{
				
		while(temp != &((*_pList)->m_tail))
		{
			temp = temp->m_next;			
			free(temp->m_prev);
		}
	}
	
	(*_pList)->m_magicNumber = DEALLOCATED;
	free(*_pList);
	*_pList = NULL;
}

List_Result List_PushHead(List* _list, void* _item)
{
	return List_Push(_list, _item, 1);
}

List_Result List_PushTail(List* _list, void* _item)
{
	return List_Push(_list, _item, 0);
}

List_Result List_PopHead(List* _list, void** _pItem)
{
	return ListPop(_list, _pItem, 1);	
}

List_Result List_PopTail(List* _list, void** _pItem)
{
	return ListPop(_list, _pItem, 0);	
}


size_t List_Size(const List* _list)
{
	if(NULL == _list)
	{
		return 0;
	}
	
	return _list->m_numOfItems;
}

/*----------------------------------------------------------------------------*/
/*-------------------Static Functions Implementation--------------------------*/
/*----------------------------------------------------------------------------*/
static List_Result ListPop(List* _list, void** _pItem, int _isHead)
{
	Node* temp;	
	/*TODO this could be in API function*/
	if(NULL == _list || NULL == _pItem)
	{
		return LIST_UNINITIALIZED_ERROR;
	}
	
	if(_list->m_numOfItems == 0)
	{
		return LIST_UNDERFLOW;
	}		

	temp = (_isHead == 1 ? (_list->m_head).m_next : (_list->m_tail).m_prev);
	
	temp->m_prev->m_next = temp->m_next;
	temp->m_next->m_prev = temp->m_prev;
	
	*_pItem = temp->m_item;
	free(temp);
	
	--_list->m_numOfItems;
	
	return LIST_SUCCESS;
}

static List_Result List_Push(List* _list, void* _item, int _isHead)
{
	Node* newNode;
	
	if(NULL == _list)
	{
		return LIST_UNINITIALIZED_ERROR;
	}
	if(NULL == _item)
	{
		return LIST_NULL_ELEMENT_ERROR;
	}
	
	newNode = (Node*) malloc(sizeof(Node));
	newNode->m_item = _item;
	
	newNode->m_next = (_isHead == 1 ? (_list->m_head).m_next : &(_list->m_tail));
	newNode->m_prev = (_isHead == 1 ? &(_list->m_head) : (_list->m_tail).m_prev);
	newNode->m_prev->m_next = newNode;
	newNode->m_next->m_prev = newNode;
	
	++_list->m_numOfItems;
	
	return LIST_SUCCESS;
}




































