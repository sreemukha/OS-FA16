#include <future.h>
syscall future_get(future *fut,int *value)
{
  if(fut == NULL ||fut->state == FUTURE_WAITING) {
    return SYSERR;
  }
  if(fut->state == FUTURE_EMPTY) {
    fut->pid = getpid();
    fut->state = FUTURE_WAITING;
    suspend(fut->pid);
  }
  *value = fut->value;
  fut->state = FUTURE_EMPTY;
  return OK;
}