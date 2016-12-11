/* xsh_prodcons.c - xsh_prodcons */
/* This file contains the code for the shell command xsh_prodcons */

#include <xinu.h>
#include<stdio.h>

shellcmd xsh_udprecv(int32 nargs, char *args[])
{
	int32	retval;			/* Return value from call	*/
	uid32	slot;			/* Slot in UDP table		*/
	uint32	serverip;		/* IP address of a 	*/
	uint32	rport = 8888;
	uint32	lport = 64999;
	uint32	msg;
	int i;
	byte *mac;
	
	kprintf("\nProcess : %d",getpid());
	/* Convert time server IP address to binary */

	if (dot2ip("192.168.1.100", &serverip) == SYSERR) {
		return SYSERR;
	}
	//arp_resolve (serverip, mac);
	
	/* Contact the time server to get the date and time */

	slot = udp_register(serverip, rport, lport);
	if (slot == SYSERR) {
		fprintf(stderr,"udptest: cannot register a udp port %d\n",
					rport);
		return SYSERR;
	}
	kprintf("\nPort registered");

	/* Send arbitrary message VM server */

	/*if (getlocalip() == SYSERR) {
		return SYSERR;
	}*/
	kprintf("\nbefore udp recv");
	retval = udp_recv(slot, (char *) &msg, 4, 20000);
	kprintf("\nafter udp recv");
	if ( (retval == SYSERR) || (retval == TIMEOUT) ) {
		udp_release(slot);
		return SYSERR;
	}
	udp_release(slot);
	kprintf ("\nThe message received is: %s", msg);
	//now = ntim2xtim( ntohl(nnow) );
	//Date.dt_boot = now - clktime;
	//Date.dt_bootvalid = TRUE;
	//*timvar = now;

	return 0;
}