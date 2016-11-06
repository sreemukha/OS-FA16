#include <xinu.h>
#include <prodcons.h>
#include <future.h>

uint32 future_cons(future *fut) {
  int32 i, status;
  intmask im;
  im = disable();
  status = future_get(fut, &i);
  if (status < 1) {
    fprintf(stderr,"Error: future_get failed\n\r");
    restore(im);
    return -1;
  }
  kprintf("Consumed Value:: %d\n", i);
  restore(im);
  
  return OK;
}