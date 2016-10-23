#include <xinu.h>
#include <future.h>
syscall future_set(future* f, int32* value){
  intmask im;
  struct	procent *prptr;
  int32 tval;
  pid32 cpid;
  im = disable();
  tval = *value;
  switch(f->flag){
    case FUTURE_EXCLUSIVE:
	if(f->state == FUTURE_EMPTY){ // No Process is waiting
	  f->state = FUTURE_VALID; 
	  f->value = value;
	}
	else if(f->state == FUTURE_WAITING){ // Consumer already waiting
	  f->value = value;
	  f->state = FUTURE_VALID;
	  ready(f->pid);
	}
	else if(f->state == FUTURE_VALID){ // Produced value not consumed
	  printf("FUTURE_SET: Only One set can be called in FUTURE EXCLUSIVE Mode\n");
	  restore(im);
	  return SYSERR;
	}		
	restore(im);	// if ctrl reaches here, it means get is successful
	return OK;
	break;
    case FUTURE_SHARED:
	if(f->state == FUTURE_VALID){
	  printf("FUTURE_SET: Only One set can be called in FUTURE SHARED Mode\n");
	  restore(im);
	  return SYSERR;
	}
	f->value = value;
	f->state = FUTURE_VALID;
	while(!isempty(f->get_queue)){
	  ready(dequeue(f->get_queue));
	}
	break;
    case FUTURE_QUEUE:
	if(isempty(f->get_queue)){
	  cpid = getpid();
	  enqueue(cpid,f->set_queue);
	  prptr = &proctab[f->pid];
	  prptr->prstate = PR_WAIT;
	  resched(); 
	  im = disable();
	  f->value = value;
	  if(!isempty(f->get_queue)){
	    ready(dequeue(f->get_queue));
          }
	 restore(im);
         return OK;							
	}
	else{
	  f->value = value;
	  ready(dequeue(f->get_queue));
	  restore(im);
	  return OK;
	}
	break;
  } //end switch
} //end syscall