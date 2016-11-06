#include <prodcons.h>
uint future_cons(future *fut) {
  int32 i,status=0;
  intmask mask;
  mask=disable();
  status = future_get(fut,&i); //Get the produced value from producer
  if(status<1) {
    kprintf("future_get failed\n");
    return -1;
  }
  
  kprintf("\nConsumed value: %d\n",i);
  restore(mask);
  return OK;
}