#include <xinu.h>
#include <future.h>
future* future_alloc(int32 future_flag){
  register future* fut;
  fut = getmem(sizeof(future));	// Allocate memory for future
  if(fut == NULL) {
    return SYSERR;		//Return Error if getmem failed
  }
  fut->flag = future_flag;
  fut->state = FUTURE_EMPTY;	// Initialize state to empty
  fut->get_queue = newqueue();
  fut->set_queue = newqueue();
  return fut;			// Return the created Future
}