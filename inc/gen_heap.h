#ifndef __GEN_HEAP_H__
#define __GEN_HEAP_H__

/** 
 * @brief Generic heap implementation
 * 
 * @author Anton Gulyaev 
 */ 

#include "../inc/vector.h"
#include "../inc/ADTDefs.h"
#include <stddef.h>

typedef struct Rect
{
	size_t m_length;
	size_t m_height;
} Rect;

typedef struct Heap Heap;

typedef int(*LessComparator)(void* _a, void* _b);
typedef void(*DestroyItem)(void* _item);
typedef int	(*HeapElementAction)(void* _element, void* _context);

/*TODO what if vector is NULL?*/
/*TODO better build my own vector than using and changing user's one*/

/** 
 * @brief Build heap from given vector
 * @param _vec: vector to be built on
 * @param _less: pointer to function to compare elements
 * @return pointer to created heap
 */
Heap* HeapBuild(Vector* _vec, LessComparator _less);

/** 
 * @brief Destroys heap and returns its vector
 * @param _heap: pointer to the heap to be destroyed
 * @return pointer to vector that heap was built on
 */
Vector* HeapDestroy(Heap* _heap);

/** 
 * @brief Insert item into heap
 * @param _heap: heap to insert into
 * @param _item: item to be inserted
 * @return error codes
 * @retval: ERR_OK if succeeded
 * @retval: ERR_NOT_INITIALIZED if heap or item pointer was NULL. NULL item cannot be inserted
 */
ADTErr HeapInsert(Heap* _heap, void* _item);

/** 
 * @brief Returns maximal item from the heap
 * @param _heap: heap pointer
 * @param _item: pointer to hold returned item
 * @return error codes
 * @retval: ERR_OK if succeeded
 * @retval: ERR_NOT_INITIALIZED if heap or item pointer was NULL
 */
ADTErr HeapMax(Heap* _heap, void** _item);

/** 
 * @brief Removes and returns maximal item from the heap
 * @param _heap: heap pointer
 * @param _item: pointer to hold returned item
 * @return error codes
 * @retval: ERR_OK if succeeded
 * @retval: ERR_NOT_INITIALIZED if heap or item pointer was NULL
 */
ADTErr HeapExtractMax(Heap* _heap, void** _item);

/** 
 * @brief Returns maximal item from the heap
 * @param _heap: heap pointer
 * @param _item: pointer to hold returned item
 * @return error codes
 * @retval: ERR_OK if succeeded
 * @retval: ERR_NOT_INITIALIZED if heap or item pointer was NULL
 */
size_t HeapItemsNum(Heap* _heap);

/** 
 * @brief Runs given function and each item in the heap. Function stops iterating if _action returns zero value on a given element
 * @param _heap: heap pointer
 * @param _action: pointer to the function to run on elements
 * @param _context: parameter to the function to be ran on elements. NULL if no such parameter is required
 * @return number of elements iterated over
 */
size_t Heap_ForEach(const Heap* _heap, HeapElementAction _action, void* _context);

#endif
