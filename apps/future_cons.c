#include <xinu.h>
#include <prodcons.h>
#include <future.h>

uint32 future_cons(future *fut) {
  int32 i, status;
  intmask im;
  status = future_get(fut, &i);
  if (status < 1) {
    fprintf(stderr,"Error: future_get failed\n\r");
    return -1;
  }
  im = disable();
  kprintf("Produced Value:: %d\n", i);
  restore(im);
  return OK;
}