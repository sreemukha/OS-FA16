
#include <xinu.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 * xhs_hello - write argument strings to stdout
 *------------------------------------------------------------------------
 */
shellcmd xsh_hello(int nargs, char *args[])
{
	int32	i;
	if (nargs == 2) {
		printf("Hello %s, Welcome to the world of Xinu!!", args[1]);
	}
	else if(nargs <= 1){
		printf("\nToo few arguments");
	//	printf("\n%d",nargs);
	}
	else if(nargs >2){
		printf("\nToo many arguments");
	//	printf("\n%d",nargs);
	}
	printf("\n");

	return 0;
}
