#include <xinu.h>
#include <prodcons.h>
#include <future.h>
uint32 future_prod(future *fut) {
  int32 i, j;
  j = (int)fut;
  for (i=0; i<1000; i++) {
    j += i;
  }
  j += rand();
  // calling future_set syscall
  future_set(fut, &j);
  return OK;
}