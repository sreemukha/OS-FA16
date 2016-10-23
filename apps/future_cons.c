#include <xinu.h>
#include <prodcons.h>
<<<<<<< HEAD
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
  //future_free(fut);    // Now that the future is not needed.
=======
uint future_cons(future *fut) {
  int i,status=0;
  status = future_get(fut,&i); //Get the produced value from producer
  if(status<1) {
    kprintf("future_get failed\n");
    return -1;
  }
  kprintf("\nIt produced: %d\n",i);
>>>>>>> origin/master
  return OK;
}
