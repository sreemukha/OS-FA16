#include <prodcons.h>
uint future_cons(future *fut) {
  int i,status = 0;
  while(1)
  {
    status = future_get(fut,&i); //Get the produced value from producer
    if(status<1) {
      printf("future_get failed\n");
      return -1;
    }
    else if(status == 1) {
      printf("\nConsumed value: %d\n",i);
      break;
    }
  }
  return OK;
}
