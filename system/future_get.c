#include <future.h>
syscall future_get(future *fut,int32 *value)
{
  intmask mask;
  mask = disable();
  if(fut == NULL ||fut->state == FUTURE_WAITING) {
    restore(mask);
    return SYSERR;
  }
  if(fut->state == FUTURE_EMPTY) {
    fut->pid = getpid();
    fut->state = FUTURE_WAITING;
    suspend(fut->pid);
  }
  *value = fut->value;
  fut->state = FUTURE_EMPTY;
  restore(mask);
  return OK;
}