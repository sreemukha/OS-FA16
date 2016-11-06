#include <xinu.h>
#include <future.h>
future* future_alloc(int32 future_flag)
{	
  intmask mask;
  mask=disable();
  future *fut = (future*)getmem(sizeof(future));
  if(fut != NULL) {
    fut->flag = future_flag;
    fut->state = FUTURE_EMPTY;
  }
  restore(mask);
  return fut;
}