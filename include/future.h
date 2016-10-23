#include <xinu.h>
#ifndef _FUTURE_H_
#define _FUTURE_H_
 
/* define states */
#define FUTURE_EMPTY	  0
#define FUTURE_WAITING 	  1         
#define FUTURE_VALID 	  2         

/* modes of operation for future*/
#define FUTURE_EXCLUSIVE  1
#define FUTURE_SHARED     2	    
#define FUTURE_QUEUE      3     
	
// futent structure
typedef struct futent
{
   int32 *value;		
   int32 flag;		
   int32 state;         	
   pid32 pid;
   qid16 set_queue;
   qid16 get_queue;
} future;

/* Interface for system call */
future* future_alloc(int32 future_flags);
syscall future_free(future*);
syscall future_get(future*, int32*);
syscall future_set(future*, int32*);

/* Prod and Cons Definitions */
uint32 future_cons(future *);
uint32 future_prod(future *);
 
#endif /* _FUTURE_H_ */