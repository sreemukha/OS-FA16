#include <xinu.h>
#include <future.h>
syscall future_get(future* f, int32* value){
  struct	procent *prptr;
  intmask im;
  pid32 cpid;
  im = disable();
  switch(f->flag){
    case FUTURE_EXCLUSIVE:
	if(f->state == FUTURE_VALID){ // Value already produced
	  *value = *(f->value);
	  f->state = FUTURE_EMPTY;
	}
	else if(f->state == FUTURE_EMPTY){ // Value not yet produced
    	  f->state = FUTURE_WAITING;
	  f->pid = getpid();
	  prptr = &proctab[f->pid];
	  prptr->prstate = PR_WAIT;
	  resched(); 
	  im = disable(); // resched() enables interrupts automatically.
	  *value = *(f->value);
	  f->state = FUTURE_EMPTY;
	}
	else if(f->state == FUTURE_WAITING){ // A process is already waiting.
	  restore(im);
	  return SYSERR;
	}		
	restore(im);	// if ctrl reaches here, it means get is successful
	return OK;
    case FUTURE_SHARED:
      if(f->state == FUTURE_EMPTY || f->state == FUTURE_WAITING){
        if(f->state == FUTURE_EMPTY) {
	  f->state = FUTURE_WAITING;
        }
	cpid=getpid();
	enqueue(cpid,f->get_queue);
	prptr = &proctab[getpid()];
	prptr->prstate = PR_WAIT;
	resched();
	im = disable();
	*value = *(f->value);
	restore(im);
	return OK;
	}
        else{
	  *value = *(f->value);
	  restore(im);
	  return OK;
	}
    case FUTURE_QUEUE:	
	if(!isempty(f->set_queue)) {
	  ready(dequeue(f->set_queue));
        }
	cpid = getpid();
	enqueue(cpid,f->get_queue);
	prptr = &proctab[getpid()];
	prptr->prstate = PR_WAIT;
	resched();
	im = disable();
	*value = *(f->value);
	restore(im);
	return OK;
  } // end switch
} //end syscall