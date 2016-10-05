#include <future.h>
syscall future_free(future *fut)
{
  if(fut == NULL) {
    return SYSERR;
  }
return freemem((char*)fut,sizeof(future));
}

