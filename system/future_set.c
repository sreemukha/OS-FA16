#include <future.h>
syscall future_set(future *fut, int *value)
{
  if(fut->state == FUTURE_EMPTY||fut->state == FUTURE_WAITING) {
    fut->state = FUTURE_VALID;
	fut->value = *value;		
	return OK;
  }	
  else {
    return SYSERR;
  }
}