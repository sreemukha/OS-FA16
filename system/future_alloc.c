#include <xinu.h>
#include <future.h>
future* future_alloc(int future_flag)
{	
  future *fut = (future*)getmem(sizeof(future));
  if(fut != NULL) {
    fut->flag = future_flag;
    fut->state = FUTURE_EMPTY;
  }
  else {
    printf("\nError!\n");
    return NULL;
  }
  return fut;
}
