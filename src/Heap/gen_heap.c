#include "../../inc/gen_heap.h"
#include "../../inc/ADTDefs.h"
#include <stddef.h>
#include "../../inc/vector.h"
#include <stdlib.h>
#include <stdio.h>

/*TODO add left and right son offsets*/
/*TODO signed and unsigned - check*/
#define LAST_PARENT_INDX(heap) (((heap->m_heapSize)) / 2)
#define LEFT_CHILD_INDEX(index) (index * 2)
#define RIGHT_CHILD_INDEX(index) (((index) * 2) + 1)
#define PARENT_INDEX(index) ((index) / 2)

#define MAGIC_NUMBER 0xdeadbeef
#define FIRST_ELEMENT_INDEX 1

#define CHECK_AND_RETURN(check, checkAgainst, returnVal) if(checkAgainst == check) \
															{ \
															return returnVal; \
															} 
#define CHECK_AND_RETURN_VOID(check, checkAgainst) if(checkAgainst != check) \
															{ \
															return; \
															} 

struct Heap
{
	Vector* m_vec;
	LessComparator m_less;
	size_t m_magicNumber;
	size_t m_heapSize;
};

static void HeapSort(Heap* _heap);
static void Heapify(Heap* _heap, size_t _index);
static void BubbleUp(Heap* _heap, size_t _index);
static void VectorSwap(Vector* _vec, size_t _indexA, size_t _indexB, void* _itemA, void* _itemB);

/*------------------------------------------------------------*/
/*--------------API functions implementation------------------*/
/*------------------------------------------------------------*/

/*---------------------HeapBuild------------------------------*/
Heap* HeapBuild(Vector* _vec, LessComparator _less)
{
	Heap* heap = NULL;
	
	CHECK_AND_RETURN(_vec, NULL, NULL);
	CHECK_AND_RETURN(_less, NULL, NULL);
	
	heap = (Heap*) malloc(sizeof(Heap));
	
	CHECK_AND_RETURN(heap, NULL, NULL);
	
	/*TODO this could be in initialize*/
	heap->m_vec = _vec;
	/*TODO check if _vec is NULL*/
	heap->m_heapSize = Vector_Size(_vec);
	heap->m_magicNumber = MAGIC_NUMBER;
	heap->m_less = _less;
	
	HeapSort(heap);
	
	return heap;
}

/*--------------------HeapDestroy----------------------------*/
Vector* HeapDestroy(Heap* _heap)
{
	Vector* vector = NULL;

	if(NULL != _heap && _heap->m_magicNumber == MAGIC_NUMBER)
	{	
		vector = _heap->m_vec;
		_heap->m_magicNumber = 0;
		free(_heap);
	}

	return vector;
}

/*--------------------HeapInsert-----------------------------*/
ADTErr HeapInsert(Heap* _heap, void* _item)
{
	CHECK_AND_RETURN(_heap, NULL, ERR_NOT_INITIALIZED);
	CHECK_AND_RETURN(_item, NULL, ERR_NOT_INITIALIZED);

	Vector_Append(_heap->m_vec, _item);
	++(_heap->m_heapSize); 
	BubbleUp(_heap, _heap->m_heapSize);

	return ERR_OK;
}

/*---------------------HeapMax------------------------------*/
ADTErr HeapMax(Heap* _heap, void** _item)
{
	CHECK_AND_RETURN(_heap, NULL, ERR_NOT_INITIALIZED);
	CHECK_AND_RETURN(_item, NULL, ERR_NOT_INITIALIZED);

	Vector_Get(_heap->m_vec, FIRST_ELEMENT_INDEX, _item);

	return ERR_OK;
}

/*-------------------HeapExtractMax-------------------------*/
/*TODO extract top*/
ADTErr HeapExtractMax(Heap* _heap, void** _item)
{
	void* temp;	
	Vector_Result result;

	CHECK_AND_RETURN(_heap, NULL, ERR_NOT_INITIALIZED);
	CHECK_AND_RETURN(_item, NULL, ERR_NOT_INITIALIZED);

	result = Vector_Get(_heap->m_vec, FIRST_ELEMENT_INDEX, _item);
	{
		if(VECTOR_SUCCESS != result)
		return ERR_UNDERFLOW;	
	}

	Vector_Remove(_heap->m_vec, &temp);
	--(_heap->m_heapSize); 
	Vector_Set(_heap->m_vec, FIRST_ELEMENT_INDEX, temp);
	Heapify(_heap, FIRST_ELEMENT_INDEX);

	return ERR_OK;
}

/*---------------------HeapItemsNum-------------------------*/
size_t HeapItemsNum(Heap* _heap)
{
	CHECK_AND_RETURN(_heap, NULL, 0);	

	return _heap->m_heapSize;
}

/*---------------------Heap_ForEach-------------------------*/
size_t Heap_ForEach(const Heap* _heap, HeapElementAction _action, void* _context)
{
	size_t res;	
	
	CHECK_AND_RETURN(_heap, NULL, 0);
	CHECK_AND_RETURN(_action, NULL, 0);
	
	res = Vector_ForEach(_heap->m_vec, (VectorElementAction)_action, _context);

	return res;
}


/*----------------------------------------------------------*/
/*---------Static functions implementation------------------*/
/*----------------------------------------------------------*/

static void HeapSort(Heap* _heap)
{
	int i;
	for(i = LAST_PARENT_INDX(_heap); i >= FIRST_ELEMENT_INDEX; --i)
	{
		Heapify(_heap, i);
	}
}

static void Heapify(Heap* _heap, size_t _index)
{
	/*TODO can this be tail recursion??*/
	void* rightChild;
	void* leftChild;
	void* currItem;
	Vector_Result result;

	/*read current item*/
	Vector_Get(_heap->m_vec, _index, &currItem);	

	/*try to read left child*/
	result = Vector_Get(_heap->m_vec, LEFT_CHILD_INDEX(_index), &leftChild);
	
	/*if couldn't read left child then there are no children. Return*/
	CHECK_AND_RETURN_VOID (result, VECTOR_SUCCESS);	

	/*try to read right child*/
	result = Vector_Get(_heap->m_vec, RIGHT_CHILD_INDEX(_index), &rightChild);	

	/*if there are both left and right children, right child is more than left child and more then current item, swap current item with right child*/
	if(VECTOR_SUCCESS == result && _heap->m_less(leftChild, rightChild) && _heap->m_less(currItem, rightChild))
	{
		VectorSwap(_heap->m_vec, _index, RIGHT_CHILD_INDEX(_index), currItem, rightChild);
		/*enter recursion on swapped child*/		
		Heapify(_heap, RIGHT_CHILD_INDEX(_index));
	}
	/*else check if current item is less then left child and swap if it is*/
	else if(_heap->m_less(currItem, leftChild))
	{
		VectorSwap(_heap->m_vec, _index, LEFT_CHILD_INDEX(_index), currItem, leftChild);
		/*enter recursion on swapped child*/
		Heapify(_heap, LEFT_CHILD_INDEX(_index));
	}
}

static void BubbleUp(Heap* _heap, size_t _index)
{
	void* item;
	void* parent;
	Vector_Result result;

	/*try to read parent*/	
	result = Vector_Get(_heap->m_vec, PARENT_INDEX(_index), &parent);
	
	/*return if there is no parent*/
	CHECK_AND_RETURN_VOID (result, VECTOR_SUCCESS);

	/*read current item*/
	Vector_Get(_heap->m_vec, _index, &item);

	/*swap if current item is greater than its parent*/
	if(_heap->m_less(parent, item))
	{
		VectorSwap(_heap->m_vec, _index, PARENT_INDEX(_index), item, parent);
		/*enter recursion on parent*/		
		BubbleUp(_heap, PARENT_INDEX(_index));
	}
}

static void VectorSwap(Vector* _vec, size_t _indexA, size_t _indexB, void* _itemA, void* _itemB)
{
	Vector_Set(_vec, _indexA, _itemB);
	Vector_Set(_vec, _indexB, _itemA);
}
































