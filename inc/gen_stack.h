#ifndef __GEN_STACK_H__
#define __GEN_STACK_H__
#include <stddef.h>
#include <stdlib.h>
#include "../inc/ADTDefs.h"

/** 
 * @brief Create a Generic Stack data type
 * that stores pointer to user provided elements of generic type
 * The Stack is heap allocated and can grow and shrink on demand.
 * 
 * @author Anton Gulyaev 
 */ 

typedef struct Stack Stack;
typedef int	(*StackElementAction)(void* _element, size_t _index, void* _context);

/**  
 * @brief Dynamically create a new stack object of given capacity and  
 * @param[in] _size - initial capacity, number of elements that can be stored initially
 * @param[in] _blockSize - the vector will grow or shrink on demand by this size 
 * @return Stack * - on success / NULL on fail 
 */
Stack* StackCreate(size_t _size, size_t _blockSize);

/**  
 * @brief Destroy existing vector
 * @param[in] _size - initial capacity, number of elements that can be stored initially
 * @param[in] destroyElement - pointer to function that destroys elements or NULL if no such function required 
 * @return void
 */
void StackDestroy(Stack* _stack, void(*destroyElement)(void* _item));

/**  
 * @brief Pushes item onto a stack
 * @param[in] _stack - stack pointer
 * @param[in] _item - item to be pushed
 * @return error codes
 * @retval ERR_OK
 * @retval ERR_NOT_INITIALIZED
 */
ADTErr StackPush(Stack* _stack, void* _item);

/**  
 * @brief Extracts item from the top of a stack
 * @param[in] _stack - stack pointer
 * @param[out] _item - stores extracted item
 * @return error codes
 * @retval ERR_OK on success
 * @retval ERR_NOT_INITIALIZED if either _stack or _item is NULL
 */
ADTErr StackPop(Stack* _stack, void** _item);

/**  
 * @brief Returns pointer to the item on the top of a stack
 * @param[in] _stack - stack pointer
 * @param[out] _item - stores extracted item
 * @return error codes
 * @retval ERR_OK on success
 * @retval ERR_NOT_INITIALIZED if either _stack or _item is NULL
 */
ADTErr StackTop(Stack* _stack, void** _item);

/**  
 * @brief Returns non-zero value if stack is empty
 * @param[in] _stack - stack pointer
 * @return int
 * @retval 0 - stack is not empty
 * @retval 1 - stack is empty
 */
int StackIsEmpty(Stack* _stack);

/**  
 * @brief Runs given function on each item of the stack
 * @param[in] _stack - stack pointer
 * @return size_t
 * @retval 0 - stack is not empty
 * @retval 1 - stack is empty
 */
size_t StackForEach(const Stack* _stack, StackElementAction _action, void* _context);

#endif




























































































