#include <xinu.h>
#include <ctype.h>
#include <stdlib.h>
#include <future.h>
#include <prodcons.h>

int n;              
sid32 produced,consumed;
shellcmd xsh_prodcons(int nargs, char *args[])
{
  int count;
  char *argument;
  int i;
  int flag=0;
  //check args[1] if present assign value to count
  if(nargs == 2) {
    if(strncmp(args[1],"-f",3) == 0) {
      future *f1,*f2,*f3;
      f1 = future_alloc(FUTURE_EXCLUSIVE);
      f2 = future_alloc(FUTURE_EXCLUSIVE);
      f3 = future_alloc(FUTURE_EXCLUSIVE);
      printf("\nFutures implementation\n");
      resume(create(future_cons,1024,20,"fcons1",1,f1));
      resume(create(future_prod,1024,20,"fprod1",1,f1));
      resume(create(future_cons,1024,20,"fcons2",1,f2));
      resume(create(future_prod,1024,20,"fprod2",1,f2));
      resume(create(future_cons,1024,20,"fcons3",1,f3));
      resume(create(future_prod,1024,20,"fprod3",1,f3));
      return 0;
    }
    if(strncmp(args[1], "--help", 7) ==  0) {
       printf("\nUsage: %s flag '-f'\n\n",args[0]);
       printf("Description:");
       printf("To achieve synchronization between 2 processes\n");
       printf("\nSyntax:");
       printf(" prodcons <parameter value>\n");
       return 0;
     }
     argument = args[1];
     for(i=0;argument[i]!='\0';i++) {
        if(!isdigit(argument[i])) {
	      flag = 1;
	  }
     }
     if(flag == 0) {
       count = atoi(args[1]);
    }
    else{
		printf("\nEnter a valid input.\n prodcons --help for more details\n");
		return 1;
	}
  }
  else if (nargs > 2) {
    printf("/nToo many arguments");
    return SYSERR;
  }
  else if(nargs < 2) {
    count=2000;
  }
  //create the process producer and consumer and put them in ready queue.
  //Look at the definations of function create and resume in the system folder for reference.
  produced = semcreate(0);
  consumed = semcreate(1);
  resume(create(producer, 1024, 20, "producer",2,count));
  resume(create(consumer, 1024, 20, "consumer",2,count));
  return (0);
}