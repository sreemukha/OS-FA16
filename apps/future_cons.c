/* this is the code for the consumer thread that is started by the shell command xsh_prodcons */

#include <xinu.h>
#include <future.h>

uint32 future_cons(future *fut) {

	int32 i, status;
	status = future_get(fut, &i);
	if (status < 1) {
		kprintf("future_get failed\n");
    		return -1;
	}
  	kprintf("Network produced %d\n", i);
	if (fut->state == FUTURE_EMPTY) {
		future_free(fut);
	}
	return OK;
}