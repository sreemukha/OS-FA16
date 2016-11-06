#include <future.h>
syscall future_free(future *fut)
{
  if(fut == NULL) {
    return SYSERR;
  }
  if(fut->state == FUTURE_WAITING) {
    ready(fut->pid);
 }
  return freemem((char*)fut,sizeof(future));
}