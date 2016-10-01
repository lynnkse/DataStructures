#include "../../inc/gen_stack.h"
#include "../../inc/vector.h"
#include "../../inc/ADTDefs.h"

#define MAGIC_NUMBER 0xdeafbeef

#define CHECK_AND_RETURN(check, checkAgainst,returnVal) if(checkAgainst == check) \
															{ \
															return returnVal; \
															} 
#define IF_BOTH_ZERO_RETURN(first, second) if(0 == first && 0 == second)\
											{\
												return NULL;\
											}

struct Stack
{
	Vector* m_vec;
	size_t m_magicNumber;
};

/*----------------------------------------------------------------------------*/
/*--------------------Static functions defenitions----------------------------*/
/*----------------------------------------------------------------------------*/
static ADTErr ListErrToQueueErr(Vector_Result _err);

/*----------------------------------------------------------------------------*/
/*--------------------API functions implementation----------------------------*/
/*----------------------------------------------------------------------------*/

/*--------------------------StackCreate---------------------------------------*/
Stack* StackCreate(size_t _size, size_t _blockSize)
{
	Stack* stack;

	IF_BOTH_ZERO_RETURN(_size, _blockSize);

	stack = (Stack*) malloc(sizeof(Stack));
	CHECK_AND_RETURN(stack, NULL, NULL);
	
	stack->m_vec = Vector_Create(_size, _blockSize);

	if(NULL == stack->m_vec)
	{
		free(stack);
		return NULL;
	}

	stack->m_magicNumber = MAGIC_NUMBER;

	return stack;
}

/*--------------------------StackDestroy--------------------------------------*/
void StackDestroy(Stack* _stack, void(*destroyElement)(void* _item))
{
	if(NULL != _stack && MAGIC_NUMBER == _stack->m_magicNumber)
	{
		Vector_Destroy(&_stack->m_vec, destroyElement);
		_stack->m_magicNumber = 0;
		free(_stack);
	}
}

/*----------------------------StackPush---------------------------------------*/
ADTErr StackPush(Stack* _stack, void* _item)
{
	Vector_Result result;

	CHECK_AND_RETURN(_stack, NULL ,ERR_NOT_INITIALIZED);
	CHECK_AND_RETURN(_item, NULL ,ERR_NOT_INITIALIZED);

	result = Vector_Append(_stack->m_vec, _item);

	return ListErrToQueueErr(result);
}

/*----------------------------StackPop----------------------------------------*/
ADTErr StackPop(Stack* _stack, void** _item)
{
	Vector_Result result;

	CHECK_AND_RETURN(_stack, NULL ,ERR_NOT_INITIALIZED);
	CHECK_AND_RETURN(_item, NULL ,ERR_NOT_INITIALIZED);

	result = Vector_Remove(_stack->m_vec, _item);

	return ListErrToQueueErr(result);
}

/*----------------------------StackTop----------------------------------------*/
ADTErr StackTop(Stack* _stack, void** _item)
{
	Vector_Result result;

	CHECK_AND_RETURN(_stack, NULL ,ERR_NOT_INITIALIZED);
	CHECK_AND_RETURN(_item, NULL ,ERR_NOT_INITIALIZED);

	result = Vector_Get(_stack->m_vec, Vector_Size(_stack->m_vec), _item);

	return ListErrToQueueErr(result);
}

/*---------------------------StackIsEmpty-------------------------------------*/
int StackIsEmpty(Stack* _stack)
{
	CHECK_AND_RETURN(_stack, NULL , 1);
	return (Vector_Size(_stack->m_vec) == 0 ? 1 : 0);
}

/*---------------------------StackForEach-------------------------------------*/
size_t StackForEach(const Stack* _stack, StackElementAction _action, void* _context)
{
	CHECK_AND_RETURN(_stack, NULL , 0);
	CHECK_AND_RETURN(_action, NULL , 0);
	return Vector_ForEach(_stack->m_vec, (VectorElementAction) _action, _context);
}


/*----------------------------------------------------------------------------*/
/*--------------------Static functions implementation-------------------------*/
/*----------------------------------------------------------------------------*/
static ADTErr ListErrToQueueErr(Vector_Result _err)
{
	ADTErr queueRes = ERR_GENERAL;
	switch(_err)
	{
		case VECTOR_SUCCESS:
			queueRes = ERR_OK;
			break;
		case VECTOR_UNITIALIZED_ERROR:
			queueRes = ERR_GENERAL;
			break;
		case VECTOR_ALLOCATION_ERROR:
			queueRes = ERR_ALLOCATION_FAILED;
			break;
	    case VECTOR_INDEX_OUT_OF_BOUNDS_ERROR:
			queueRes = ERR_WRONG_INDEX;
			break;
	}
	return queueRes;
}

















































































