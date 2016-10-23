#include <xinu.h>
#include <ctype.h>
#include <future.h>
#include <prodcons.h>

int n;              
sid32 produced,consumed;
shellcmd xsh_prodcons(int nargs, char *args[])
{
  int count;
  char *argument;
  struct procent *prptr;  //Pointer to process table
  int i, ind=0,cntproc=0;
  pid32 conpid[NPROC],cpid;
  int flag=0;
  //check args[1] if present assign value to count
  if(nargs == 2) {
    if(strncmp(args[1],"-f",3) == 0) {
      future *f_exlcusive, *f_shared, *f_queue;
      f_exclusive = future_alloc(FUTURE_EXCLUSIVE);
      f_shared = future_alloc(FUTURE_SHARED);
      f_queue = future_alloc(FUTURE_QUEUE);

      // Test FUTURE_EXCLUSIVE
      conpid[conind] = create(future_cons, 1024, 20, "fcons1",1,f_exclusive);
      ind++;
      conpid[conind] = create(future_prod, 1024, 20, "fprod1", 1, f_exclusive);
      ind++;

      // Test FUTURE_SHARED
      conpid[conind] = create(future_cons, 1024, 20, "fcons2",1,f_shared);
      ind++;
      conpid[conind] = create(future_cons, 1024, 20, "fcons3",1,f_shared);
      ind++;
      conpid[conind] = create(future_cons, 1024, 20, "fcons4",1,f_shared);
      ind++;
      conpid[conind] = create(future_cons, 1024, 20, "fcons5",1,f_shared);
      ind++;
      conpid[conind] = create(future_prod, 1024, 20, "fprod2",1,f_shared);
      ind++;	 	 

      // Test FUTURE_QUEUE
      conpid[conind] = create(future_cons, 1024, 20, "fcons6",1,f_queue);
      ind++;
      conpid[conind] = create(future_cons, 1024, 20, "fcons7",1,f_queue);
      ind++;
      conpid[conind] = create(future_cons, 1024, 20, "fcons7",1,f_queue);
      ind++;
      conpid[conind] = create(future_cons, 1024, 20, "fcons7",1,f_queue);
      ind++;
      conpid[conind] = create(future_prod, 1024, 20, "fprod3",1,f_queue);
      ind++;
      conpid[conind] = create(future_prod, 1024, 20, "fprod4",1,f_queue);
      ind++;
      conpid[conind] = create(future_prod, 1024, 20, "fprod5",1,f_queue);
      ind++;
      conpid[conind] = create(future_prod, 1024, 20, "fprod6",1,f_queue);
      ind++;

      // Resuming above processes
      for(i=0;i<ind;i++) {
        resume(conpid[i]);
      }
      sleep(2);
      while(cntproc<ind) {
        for(i=0;i<ind;i++) {
	  cpid=conpid[i];
	  if(cpid!=0) {
	    prptr = &proctab[tpid];
	    if(prptr->prstate==PR_FREE) {
	      cntproc++;
	      conpid[i]=0;
	      cpid=0;
	    } //end inner if
	  }// end if
        } //end for
      } //end while
      
      // Calling future_free
      future_free(f_queue);
      kprintf("future queue free\n");
      future_free(f_shared);
      kprintf("future shared free\n");
      future_free(f_exclusive);
      kprintf("future exclusive free"\n);
      return 0;
    } // end if for -f flag
    if(strncmp(args[1], "--help", 7) ==  0) {
       kprintf("\nUsage: %s -f for futures implementation or %s [optional integer]\n\n",args[0]);
       kprintf("Description:");
       kprintf("To achieve synchronization between 2 processes\n");
       kprintf("\nSyntax:");
       kprintf("prodcons <parameter value>\n");
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
		kprintf("\nEnter a valid input.\n prodcons --help for more details\n");
		return 1;
	}
  }
  else if (nargs > 2) {
    kprintf("/nToo many arguments");
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