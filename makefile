CC = gcc
FLAGS = #-ansi -pedantic -Wall -Werror

VEC_PATH = src/Vector/
HEAP_PATH = src/Heap/
QUEUE_PATH = src/Queue/
STACK_PATH = src/Stack/
HASH_PATH = src/HashMap/
LIST_PATH = src/LinkedList/
SAFE_QUEUE_PATH = src/ThreadSafeQueue/
IPC_QUEUE_PATH = src/SharedQueue/
HEADERS_PATH = inc/



OBJC = gen_queue.o gen_heap.o vector.o gen_stack.o HashMap.o list.o list_itr.o list_functions.o safe_queue.o queue.o

#@	Command in terminal: export LD_LIBRARY_PATH=/home/:$LD_LIBRARY_PATH	@#

#Data Structures dynamic lib
libds.so: $(OBJC)
	$(CC) -shared -o libds.so $(OBJC) -lpthread

#Vector	
vector.o: $(VEC_PATH)vector.c $(HEADERS_PATH)vector.h
	$(CC) -g -c -o  vector.o $(FLAGS) -fPIC $(VEC_PATH)vector.c
	
#Heap 
gen_heap.o: $(HEAP_PATH)gen_heap.c $(HEADERS_PATH)gen_heap.h
	$(CC) -g -c -o  gen_heap.o $(FLAGS) -fPIC $(HEAP_PATH)gen_heap.c

#Queue
gen_queue.o: $(QUEUE_PATH)gen_queue.c $(HEADERS_PATH)gen_queue.h
	$(CC) -g -c -o  gen_queue.o $(FLAGS) -fPIC $(QUEUE_PATH)gen_queue.c

#Stack
gen_stack.o: $(STACK_PATH)gen_stack.c $(HEADERS_PATH)gen_stack.h
	$(CC) -g -c -o  gen_stack.o $(FLAGS) -fPIC $(STACK_PATH)gen_stack.c

#Hash
HashMap.o: $(HASH_PATH)HashMap.c $(HEADERS_PATH)HashMap.h
	$(CC) -g -c -o  HashMap.o $(FLAGS) -fPIC $(HASH_PATH)HashMap.c -lm

#DList
list.o: $(LIST_PATH)list.c $(HEADERS_PATH)list.h
	$(CC) -g -c -o  list.o $(FLAGS) -fPIC $(LIST_PATH)list.c

#DList_Itr
list_itr.o: $(LIST_PATH)list_itr.c $(HEADERS_PATH)list_itr.h
	$(CC) -g -c -o  list_itr.o $(FLAGS) -fPIC $(LIST_PATH)list_itr.c

#DList_Itr
list_functions.o: $(LIST_PATH)list_functions.c $(HEADERS_PATH)list_functions.h
	$(CC) -g -c -o  list_functions.o $(FLAGS) -fPIC $(LIST_PATH)list_functions.c

#Thread_Safe_Queue
safe_queue.o: $(SAFE_QUEUE_PATH)safe_queue.c $(HEADERS_PATH)safe_queue.h $(HEADERS_PATH)gen_queue.h
	$(CC) -g -c -o  safe_queue.o $(FLAGS) -fPIC $(SAFE_QUEUE_PATH)safe_queue.c

#IPC Queue(can be stored in shared memory)
queue.o: $(IPC_QUEUE_PATH)queue.c $(HEADERS_PATH)queue.h
	$(CC) -g -c -o  queue.o $(FLAGS) -fPIC $(IPC_QUEUE_PATH)queue.c

#clean
clean: 
	rm -f $(OBJC) libds.so 































