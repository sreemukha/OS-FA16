#include <future.h>
syscall future_get(future *fut,int *value)
{
  if(fut->state==FUTURE_EMPTY) {
    fut->pid=getpid();
    fut->state=FUTURE_WAITING;
    return OK;
  }
  if(fut->state==FUTURE_WAITING) { 	
    return SYSERR;
  }
  *value=fut->value;
  fut->state=FUTURE_EMPTY;
  return OK;  
}
