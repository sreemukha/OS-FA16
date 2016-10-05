#include <xinu.h>
#include <prodcons.h>

void consumer(int count) {
    //Code to consume values of global variable 'n'
    //print consumed value e.g. consumed : 8
  int i;
  for( i=1;i<=count;i++ ) {
    wait(consumed);
    kprintf("\nconsumed : %d\n",n);
    signal(produced);
  }
}

