/* xsh_prodcons.c - xsh_prodcons */
/* This file contains the code for the shell command xsh_prodcons */

#include <xinu.h>
#include<stdio.h>

shellcmd xsh_udptest(int32 nargs, char *args[])
{
	int32	retval;			/* Return value from call	*/
	uid32	slot;			/* Slot in UDP table		*/
	uint32	serverip;		/* IP address of a 	*/
	uint32	rport = 8888;
	uint32	lport = 64999;
	char	msg[31]="";
	char 	retmsg;
	int32 i =0;
	int32 j,length;
	byte *mac;
	char	prompt[31] = "This is a test message";	/* Message to VM server*/
	
	/* Convert time server IP address to binary */

	if (dot2ip("192.168.1.100", &serverip) == SYSERR) {
		return SYSERR;
	}
	
	/* Contact the time server to get the date and time */

	slot = udp_register(serverip, rport, lport);
	if (slot == SYSERR) {
		fprintf(stderr,"udptest: cannot register a udp port %d\n",
					rport);
		return SYSERR;
	}

	/* Send arbitrary message VM server */

	retval = udp_send(slot,	prompt, 100);
	if (retval == SYSERR) {
		fprintf(stderr,"udptest: cannot send a udp message %d\n",rport);
		udp_release(slot);
		return SYSERR;
	}

	retval = udp_recv(slot, msg, 31, 20000);
	if ( (retval == SYSERR) || (retval == TIMEOUT) ) {
		udp_release(slot);
		return SYSERR;
	}
	kprintf("\nThe reply received from the VM is : %s\n", msg);
	udp_release(slot);

	return 0;
}