#include <future.h>
syscall future_set(future *fut, int32 *value)
{
  intmask mask;
  mask = disable();
  if(fut == NULL || fut->state == FUTURE_VALID) {
    restore(mask);
    return SYSERR;
  }
  if(fut->state == FUTURE_WAITING || fut->state == FUTURE_EMPTY) {
    fut->value = *value;
    fut->state = FUTURE_VALID;
    resume(fut->pid);
    restore(mask);
    return OK;
  }
}