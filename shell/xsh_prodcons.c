#include <xinu.h>
#include <prodcons.h>
#include<ctype.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
int n;                 //Definition for global variable 'n'
/*Now global variable n will be on Heap so it is accessible all the processes i.e. consume and produce*/

shellcmd xsh_prodcons(int nargs, char *args[])
{
  //Argument verifications and validations
  int count;            //local varible to hold count
  char *argument;
  int i;
  //check args[1] if present assign value to count
  if (nargs == 2)
  {
		if(strncmp(args[1], "--help", 7) ==  0)
		{
			printf("\nUsage: %s\n\n",args[0]);
			printf("Description:");
			printf(" Takes one optional parameter and performs producer and consumer synchronization\n");
			printf("\nSyntax:");
			printf(" prodcons <parameter value>\n");
			return 0;
		}
		argument = args[1];
				for(i=0;argument[i]!='\0';i++)
		{
 			if(!isdigit(argument[i]))
			{
				fprintf(stderr, "%s: Please enter a valid integer\n", args[0]);
				return 1;
			}
		}
		count = atoi(args[1]);
	        //printf("\nCount:%d\n",count);
  }
 else if (nargs > 2)
 {
        printf("/nToo many arguments");
        return SYSERR;
 }
else if(nargs < 2)
{
	count=2000;
}
  //create the process producer and consumer and put them in ready queue.
  //Look at the definations of function create and resume in the system folder for reference.
  resume(create(producer, 1024, 20, "producer", 1, count));
  resume(create(consumer, 1024, 20, "consumer", 1, count));
  return (0);
}
