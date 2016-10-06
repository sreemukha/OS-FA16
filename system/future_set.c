#include <future.h>
syscall future_set(future *fut, int *value)
{
  if(fut == NULL || fut->state == FUTURE_VALID) {
    return SYSERR;
  }
  fut->value = *value;
  fut->state = FUTURE_VALID;
  resume(fut->pid);
  return OK;
}