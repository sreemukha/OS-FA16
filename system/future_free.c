#include <future.h>
syscall future_free(future *fut)
{
  intmask mask;
  mask = disable();
  if(fut == NULL) {
    restore(mask);
    return SYSERR;
  }
  if(fut->state == FUTURE_WAITING) {
    ready(fut->pid);
 }
  restore(mask);
  return freemem((char*)fut,sizeof(future));
}