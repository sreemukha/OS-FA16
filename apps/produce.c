#include <xinu.h>
#include <prodcons.h>

void producer(int count) {
    //Code to produce values less than equal to count,
    //produced value should get assigned to global variable 'n'.
    //print produced value e.g. produced : 8
  int i;
  for(i=1;i<=count;i++) {
     if(i==1) {
       n=i;
       kprintf("\nProduced : %d\n",n);
     }
    else if(i>1){
      wait(produced);
      n=i;
      kprintf("\nProduced : %d",n);
      signal(consumed);		    
    }
  }	
}