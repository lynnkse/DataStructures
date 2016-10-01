#include "../../inc/HashMap.h"
#include <math.h>
#include <stdlib.h>
#include "../../inc/defines.h"
#include "../../inc/list.h"
#include "../../inc/list_itr.h"
#include "../../inc/list_functions.h"
#include <pthread.h>

#define MAGIC_NUMBER 0xdeadbeef

typedef void(*DestructionFunction)(void* _item);

struct HashMap
{
	List** m_lists;
	size_t m_capacity;	
	size_t m_magicNumber;
	HashFunction m_hashFunction;
	EqualityFunction m_equalityFunction;
	size_t m_totalItems;
	pthread_mutex_t m_mutex;
	
	#ifndef NDEBUG
	Map_Stats m_stats;
	#endif
};

typedef struct ListItem
{
	void* m_key;
	void* m_value;
} ListItem;

typedef struct Destructor
{
	DestructionFunction m_destroyKey;
	DestructionFunction m_destroyValue;
} Destructor;

typedef struct KeyAndComparator
{
	void* m_key;
	EqualityFunction m_keysEqualFunc;
} KeyAndComparator;

typedef struct ActionAndContext
{
	void* m_action;
	void* m_context;
} ActionAndContext;

typedef int	(*ItemActionFunction)(void* _item, void* _context);

/*---------------------------------Static functions declarations--------------------------------*/

static size_t IterateOverAllElementsInAllLists(List** _lists, int _capacity, PredicateFunction _iterateWith, ActionAndContext* _actionAndContext);
static int IsListItemKeyEqualToKey(ListItem* _item,  KeyAndComparator* _keyAndComparator);
static ListItr KeyItemFoundInList(List* _list, void* _key, EqualityFunction _keysEqualFunc);
static int KeyValueAction(ListItem* _item, ActionAndContext* _actionAndContext);
static void DestroyListItem(ListItem* _item, Destructor* _destructor);
static size_t GetNextPrime(size_t _num);
static Map_Result InsertItemIntoMap(ListItem* _item, HashMap* _map);
static int ItemAction(ListItem* _item, ActionAndContext* _actionAndContext);

/*--------------------------API functions inplementstion----------------------------------*/

HashMap* HashMap_Create(size_t _capacity, HashFunction _hashFunc, EqualityFunction _keysEqualFunc)
{
	HashMap* map = NULL;

	RETURN_IF_EQUAL(_hashFunc, NULL, NULL);
	RETURN_IF_EQUAL(_keysEqualFunc, NULL, NULL);

	_capacity = GetNextPrime(_capacity);

	map = (HashMap*) calloc(1, sizeof(HashMap));
	RETURN_IF_EQUAL(map, NULL, NULL);

	map->m_lists = (List**) calloc(_capacity, sizeof(List*));
	RELEASE_MEMORY_AND_RETURN_IF_EQUAL(map->m_lists, NULL, map, NULL);

	map->m_capacity = _capacity;
	map->m_magicNumber = MAGIC_NUMBER;
	map->m_hashFunction = _hashFunc;
	map->m_equalityFunction = _keysEqualFunc;
	map->m_totalItems = 0;
	pthread_mutex_init(&(map->m_mutex), NULL);

	#ifndef NDEBUG
	(map->m_stats).numberOfBuckets = 0;
	(map->m_stats).numberOfChains = 0;
	(map->m_stats).maxChainLength = 0;
	(map->m_stats).averageChainLength = 0;
	#endif

	return map;
}

void HashMap_Destroy(HashMap** _map, void (*_keyDestroy)(void* _key), void (*_valDestroy)(void* _value))
{
	int index = 0, capacity = 0;	
	Destructor destructor;
	ListItr begin, end;
	List* list;
	
	/*printf("Num of items inside: %d\n", (*_map)->m_totalItems);*/
	
	if(NULL != _map && NULL != *_map && MAGIC_NUMBER == (*_map)->m_magicNumber)
	{
		capacity = (*_map)->m_capacity;

		destructor.m_destroyKey = _keyDestroy;
		destructor.m_destroyValue = _valDestroy;	

		for( ; index < capacity; ++index)
		{
			list = ((*_map)->m_lists)[index];
			if(NULL != list)
			{			
				begin = ListItr_Begin(list);
				/*begin = ListItr_Next(begin);*/
				end = ListItr_End(list);
				ListItr_ForEach(begin, end, (ListActionFunction) DestroyListItem, &destructor);		
				List_Destroy(&list, NULL);			
			}
		}
		pthread_mutex_destroy(&((*_map)->m_mutex));
		free((*_map)->m_lists);

		(*_map)->m_magicNumber = 0;
		free(*_map);
		(*_map) = NULL;
	}	
}

size_t HashMap_ForEach(const HashMap* _map, KeyValueActionFunction _action, void* _context)
{
	int capacity = 0;	
	size_t iterations = 0;
	size_t result;
	ActionAndContext actionAndContext;


	RETURN_IF_EQUAL(_map, NULL, 0);
	RETURN_IF_EQUAL(_action, NULL, 0);

	actionAndContext.m_action = _action;
	actionAndContext.m_context = _context;
	
	capacity = _map->m_capacity;
	
	pthread_mutex_lock(&_map->m_mutex);
	result = IterateOverAllElementsInAllLists(_map->m_lists, capacity, (PredicateFunction) KeyValueAction, &actionAndContext);
	pthread_mutex_unlock(&_map->m_mutex);
	
	return result;
} 

Map_Result HashMap_Rehash(HashMap* _map, size_t _newCapacity)
{
	List** temp = NULL;
	int i;
	size_t oldCapacity;
	ActionAndContext actionAndContext;
	RETURN_IF_EQUAL(_map, NULL, MAP_UNINITIALIZED_ERROR);

	actionAndContext.m_action = (KeyValueActionFunction) InsertItemIntoMap;
	actionAndContext.m_context = _map;

	_newCapacity = GetNextPrime(_newCapacity);

	temp = _map->m_lists;
	_map->m_lists = (List**) malloc(_newCapacity * sizeof(List*));
	RETURN_IF_EQUAL(temp, NULL, MAP_ALLOCATION_ERROR);
	oldCapacity = _map->m_capacity;
	_map->m_capacity = _newCapacity;

	pthread_mutex_lock(&_map->m_mutex);
	IterateOverAllElementsInAllLists(temp, oldCapacity, (PredicateFunction) ItemAction, &actionAndContext);
	

	for(i = 0; i < oldCapacity; ++i)
	{
		List_Destroy(&temp[i], NULL);	
	}
	free(temp);
	pthread_mutex_unlock(&_map->m_mutex);
	
	return MAP_SUCCESS;
}

Map_Result HashMap_Insert(HashMap* _map, const void* _key, const void* _value)
{
	ListItem* item = NULL;	
	List* list = NULL;	
	Map_Result result;

	RETURN_IF_EQUAL(_map, NULL, MAP_UNINITIALIZED_ERROR);
	RETURN_IF_EQUAL(_key, NULL, MAP_KEY_NULL_ERROR);

	item = (ListItem*) malloc(sizeof(ListItem));
	RETURN_IF_EQUAL(item, NULL, MAP_ALLOCATION_ERROR);

	item->m_key = _key;
	item->m_value = _value;
	
	pthread_mutex_lock(&_map->m_mutex);
	result = InsertItemIntoMap(item, _map);
	pthread_mutex_unlock(&_map->m_mutex);
	
	return result;
}

Map_Result HashMap_Remove(HashMap* _map, const void* _searchKey, void** _pValue)
{
	size_t hashValue = 0;	
	List* list = NULL;
	ListItr iter;

	RETURN_IF_EQUAL(_map, NULL, MAP_UNINITIALIZED_ERROR);
	RETURN_IF_EQUAL(_searchKey, NULL, MAP_KEY_NULL_ERROR);

	hashValue = _map->m_hashFunction(_searchKey);
	list = _map->m_lists[hashValue % _map->m_capacity];

	pthread_mutex_lock(&_map->m_mutex);
	iter = KeyItemFoundInList(list, _searchKey, _map->m_equalityFunction);
	
	RETURN_IF_EQUAL(iter, NULL, MAP_KEY_NOT_FOUND_ERROR);

	*_pValue = ((ListItem*)ListItr_Remove(iter))->m_value;

	--_map->m_totalItems;
	pthread_mutex_unlock(&_map->m_mutex);
	
	#ifndef NDEBUG
	(_map->m_stats).maxChainLength = 0;
	(_map->m_stats).averageChainLength = 0;
	#endif

	return MAP_SUCCESS;
}

Map_Result HashMap_Find(const HashMap* _map, const void* _searchKey, void** _pValue)
{
	size_t hashValue = 0;	
	List* list = NULL;
	ListItr iter;

	RETURN_IF_EQUAL(_map, NULL, MAP_UNINITIALIZED_ERROR);
	RETURN_IF_EQUAL(_searchKey, NULL, MAP_KEY_NULL_ERROR);

	hashValue = _map->m_hashFunction(_searchKey);
	list = _map->m_lists[hashValue % _map->m_capacity];

	iter = KeyItemFoundInList(list, _searchKey, _map->m_equalityFunction);
	
	RETURN_IF_EQUAL(iter, NULL, MAP_KEY_NOT_FOUND_ERROR);

	*_pValue = ((ListItem*)ListItr_Get(iter))->m_value;

	return MAP_SUCCESS;
}

size_t HashMap_Size(const HashMap* _map)
{
	int index = 0;	
	size_t size = 0;
	RETURN_IF_EQUAL(_map, NULL, MAP_UNINITIALIZED_ERROR);

	for( ; index < _map->m_capacity; ++index)
	{
		if(NULL != _map->m_lists[index])
		{
			size += List_Size(_map->m_lists[index]);
		}
	}

	return size;
}

/*----------------------------Static functions defenitions-----------------------------*/

static size_t IterateOverAllElementsInAllLists(List** _lists, int _capacity, PredicateFunction _iterateWith, ActionAndContext* _actionAndContext)
{
	List* list;
	ListItr begin, end;	
	int index = 0;
	size_t iterations = 0;

	for( ; index < _capacity; ++index)
	{
		list = (_lists)[index];
		/*printf("==============================> index = %d\n", index);*/
		if(NULL != list)
		{
			begin = ListItr_Begin(list);
			end = ListItr_End(list);
			iterations += ListItr_CountIf(begin, end, _iterateWith, _actionAndContext);		
		} 
	}

	return iterations;
}

static int IsListItemKeyEqualToKey(ListItem* _item,  KeyAndComparator* _keyAndComparator)
{
	return _keyAndComparator->m_keysEqualFunc(_item->m_key, _keyAndComparator->m_key);
}

static ListItr KeyItemFoundInList(List* _list, void* _key, EqualityFunction _keysEqualFunc)
{
	ListItr begin = NULL;
	ListItr end = NULL;
	ListItr res = NULL;
	KeyAndComparator keyAndComparator;
	begin = ListItr_Begin(_list);
	end = ListItr_End(_list);

	keyAndComparator.m_key = _key;
	keyAndComparator.m_keysEqualFunc = _keysEqualFunc;
	
	res = ListItr_FindFirst(begin, end, (PredicateFunction) IsListItemKeyEqualToKey, &keyAndComparator);

	return res == end ? NULL : res;
}

static int KeyValueAction(ListItem* _item, ActionAndContext* _actionAndContext)
{
	return ((KeyValueActionFunction)(_actionAndContext->m_action))(_item->m_key, _item->m_value, _actionAndContext->m_context);
}

static int ItemAction(ListItem* _item, ActionAndContext* _actionAndContext)
{
	return ((ItemActionFunction)(_actionAndContext->m_action))(_item, _actionAndContext->m_context);
}

static void DestroyListItem(ListItem* _item, Destructor* _destructor)
{
	if(NULL != _destructor->m_destroyKey)	
	{
		_destructor->m_destroyKey(_item->m_key);
	}
	if(NULL != _destructor->m_destroyValue)
	{
		_destructor->m_destroyValue(_item->m_value);
	}

	free(_item);
}

static size_t GetNextPrime(size_t _num)
{
	int notPrime = 1;
	int i;
	
	int n = sqrt(_num);
	/*increment and check until find next prime*/
	while(notPrime)
	{
		notPrime = 0;
		for(i = 2; i <= n; i++)
		{
			if(_num % i == 0)
			{
				notPrime = 1;
			}
		}
		_num++;
		n = _num / 2;
	}
	/*last number was prime because of the last increment, so return _num - 1*/
	return _num - 1;
}

static Map_Result InsertItemIntoMap(ListItem* _item, HashMap* _map)
{
	size_t hashValue = _map->m_hashFunction(_item->m_key);
	List* list = _map->m_lists[hashValue % _map->m_capacity];

	if(NULL == list)
	{	
		list = List_Create();
		RETURN_IF_EQUAL(list, NULL, MAP_ALLOCATION_ERROR);
		_map->m_lists[hashValue % _map->m_capacity] = list;
		List_PushHead(list, _item);
		++_map->m_totalItems;

	}
	else if(NULL == KeyItemFoundInList(list, _item->m_key, _map->m_equalityFunction))
	{
		List_PushHead(list, _item);
		++_map->m_totalItems;

	}
	else
	{
		free(_item);
		return MAP_KEY_DUPLICATE_ERROR;
	}

	return MAP_SUCCESS;
}





























































































