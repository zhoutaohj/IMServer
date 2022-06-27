#include "NetBase.h"

ssize_t readn(int fd,void * buf,size_t nbytes)
{
    ssize_t nread=0;
    size_t nleft =nbytes;
    char * ptr = reinterpret_cast<char*> (buf);

    while(nleft>0)
    {
        int n= read(fd,ptr,nleft);
          
        if (n==0)
        {//endpoint close
           break;
        }
        else if(n<0)
        {//error
           if(errno ==EINTR)
              n=0;
           else
           {
               return -1;
           }
        } 

        nleft-=n;   
        nread+=n; 
        ptr+=n;
    }

    return nread;

}

ssize_t writen(int fd,const void * buf,size_t nbytes)
{
    ssize_t nwrite=0;
    size_t nleft =nbytes;
    const char * ptr = reinterpret_cast<const char*> (buf);

    while(nleft>0)
    {
        int n= write(fd,ptr,nleft);

        if(n<=0)
        {//error
           if(errno ==EINTR)
              n=0;
           else
           {
               return -1;
           }
        } 

        nleft-=n;   
        nwrite+=n; 
        ptr+=n;
    }

    return nwrite;

}