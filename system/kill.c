/* kill.c - kill */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  kill  -  Kill a process and remove it from the system
 *------------------------------------------------------------------------
 */
syscall	kill(
	  pid32		pid		/* ID of process to kill	*/
	)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	procent *prptr;		/* Ptr to process' table entry	*/
	int32	i;			/* Index into descriptors	*/
	uint32	*tmpstkptr;	/* Temporary Pointer to hold the value and traverse down the stack */
	uint32	*stklowbnd;	/* Lowest boundary of the stack allocted */
	mask = disable();
	if (isbadpid(pid) || (pid == NULLPROC)
	    || ((prptr = &proctab[pid])->prstate) == PR_FREE) {
		restore(mask);
		return SYSERR;
	}

	if (--prcount <= 1) {		/* Last user process completes	*/
		xdone();
	}

	send(prptr->prparent, pid);
	for (i=0; i<3; i++) {
		close(prptr->prdesc[i]);
	}
	// Get Lowerboundary of the stack from the prptr->prstkbase
	stklowbnd = ((uint32 *)((uint32)(prptr->prstkbase)- ((uint32)roundmb(prptr->prstklen))+ (uint32)sizeof(uint32)));
	tmpstkptr = prptr->prstkptr;
	while (tmpstkptr > stklowbnd){
		if(*tmpstkptr == STACKMAGIC)
			break;
		--tmpstkptr;
	}
	//kprintf("\nThe maximum size used by the process %d : %u", pid, ((uint32)(prptr->prstkbase)-(uint32)(tmpstkptr)));
	//kprintf("Maximum space used by the process %d is : %d\n", pid, (uint32)prptr->prstkbase - (uint32)prptr->prstkptr);
	freestk(prptr->prstkbase, prptr->prstklen);

	switch (prptr->prstate) {
	case PR_CURR:
		prptr->prstate = PR_FREE;	/* Suicide */
		resched();

	case PR_SLEEP:
	case PR_RECTIM:
		unsleep(pid);
		prptr->prstate = PR_FREE;
		break;

	case PR_WAIT:
		semtab[prptr->prsem].scount++;
		/* Fall through */

	case PR_READY:
		getitem(pid);		/* Remove from queue */
		/* Fall through */

	default:
		prptr->prstate = PR_FREE;
	}

	restore(mask);
	return OK;
}
