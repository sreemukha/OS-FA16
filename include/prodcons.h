#include<xinu.h>
#include<stddef.h>
#include<stdio.h>
#include<future.h>
/*Global variable for producer consumer*/
extern int n; 
extern sid32 consumed, produced;
extern future *f1, *f2, *f3;
/*function Prototype*/
void consumer(int);
void producer(int);

uint future_cons(future *fut);
uint future_prod(future *fut);
