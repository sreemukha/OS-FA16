#include <xinu.h>
#include <prodcons.h>
#include <future.h>
uint32 future_prod(future *fut) {
  int32 i, j;
  intmask mask;
  mask = disable();
  j = (int)fut;
  for (i=0; i<1000; i++) {
    j += i;
  }
  future_set(fut, &j);
  restore(mask);
  return OK;
}