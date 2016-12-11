/* this is the code for the consumer thread that is started by the shell command xsh_netfutures */


#include <xinu.h>
#include <netfuture.h>

uint32 nfuture_cons(nfuture *fut) {

	int32  status;
	int32 msg;

	status = nfuture_get(fut, &msg);
	if (status < 1) {
		printf("future_get failed\n");
    		return -1;
	}
  	printf("it produced %d\n", msg);
	nfuture_free(fut);

  	return OK;
}
