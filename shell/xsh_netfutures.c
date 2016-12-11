
#include <xinu.h>
#include <stdlib.h>
#include <ctype.h>
#include <future.h>

int32 n;                 //Definition for global variable 'n'
sid32 produced, consumed;
/*Now global variable n will be on Heap so it is accessible all the processes i.e. consume and produce*/
//extern uint32 future_prod(future *fut);
//extern uint32 future_cons(future *fut);

shellcmd xsh_netfutures(int32 nargs, char *args[])
{
	int32 count;
	char *arg;
	int32 i;
	n = 0;
	//Argument verifications and validations
	if (nargs == 2) {
		if (strncmp(args[1], "--help", 7) == 0) {
			printf("\nUsage: %s\n\n", args[0]);
			printf("Description:\n");
			printf("Syntax:\n");
			printf("\tnetfutures <argument>\n");
			printf("\tThe argument must be '-f' if you want to implement Network futures.\n\n");
			return 0;
		}
		if (strncmp(args[1], "-f", 3) == 0) {
			future *f_exclusive;

  			f_exclusive = future_alloc(FUTURE_EXCLUSIVE);
			printf("\nNetwork Futures Implementation");
			
			// Test FUTURE_EXCLUSIVE
			resume( create(future_cons, 1024, 20, "fcons1", 1, f_exclusive));			
			resume( create(future_prod, 1024, 20, "fprod1", 1, f_exclusive));			
			
			

			return 0;
		}
		
		arg = args[1];
		for(i=0;arg[i]!='\0';i++) {
			if (!isdigit(arg[i]))
				break;
		}
		if (arg[i] == '\0') {
			count = atoi(args[1]);
			if (count < 1) {
				fprintf(stderr, "%s: Invalid argument entered\n", args[0]);
				fprintf(stderr, "Try '%s --help' for more information\n", args[0]);
				return 1;
			}
		}
		else {
			fprintf(stderr, "%s: Invalid argument entered\n", args[0]);
			fprintf(stderr, "Try '%s --help' for more information\n", args[0]);
			return 1;
		}
	}
	else {
		fprintf(stderr, "%s: Incorrect number of arguments\n", args[0]);
		fprintf(stderr, "Try '%s --help' for more information\n", args[0]);
		return 1;
	}
	return 0;
}