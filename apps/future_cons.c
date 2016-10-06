#include <prodcons.h>
uint future_cons(future *fut) {
  int i,status=0;
  status = future_get(fut,&i); //Get the produced value from producer
  if(status<1) {
    printf("future_get failed\n");
    return -1;
  }
  printf("\nIt produced: %d\n",i);
  return OK;
}
