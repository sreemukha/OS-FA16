/* netfuture.h - Contains the function definitions for the implementation of Network futures */

#ifndef _NETFUTURE_H_
#define _NETFUTURE_H_

/* limit of number of queue entries */
#define NFUTQ 10

/* define states */
#define NFUTURE_EMPTY	0
#define NFUTURE_WAITING	1         
#define NFUTURE_VALID		2         

/* modes of operation for future*/
#define NFUTURE_EXCLUSIVE  1

typedef struct nfutent
{
   int32 *value;		
   int32 flag;		
   int32 state;         	
   pid32 pid;
} nfuture;

/* Interface for system call */
nfuture* nfuture_alloc(int32 nfuture_flags);
syscall nfuture_free(nfuture*);
syscall nfuture_get(nfuture*, int32*);
syscall nfuture_set(nfuture*, int32*);
uint32 nfuture_prod(nfuture *fut);
uint32 nfuture_cons(nfuture *fut);

#endif /* _NETFUTURE_H_ */