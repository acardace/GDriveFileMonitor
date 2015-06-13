/*
 * this a set of functions which takes in input
 * a file to watch and whenever the file is updated
 * the function "callback" is executed
 */

#include "notifier.h"
#include <sys/inotify.h>
#include <unistd.h>
#include <limits.h>
#include <errno.h>
#include <stdlib.h>

/* just a wrapper for read() */
int xread(int fd, void *buf, size_t len)
{
   ssize_t ret;

   while ( (ret = read (fd, buf, len)) == -1) {
      if (errno == EINTR){
         buf += ret;
         continue;
      }
      return ERROR;
   }
   return ret;
}

/*this function will monitor modifications
 * on the file "path", and when one
 * occurs the "callback" function gets called
 */
int file_watch(char *path, void (*callback)(char *filepath) )
{
   char buf[BUF_LEN];
   struct inotify_event *event;
   int fd, wd;

   /* fd is the inotify istance*/
   if ( ( fd = inotify_init() ) == -1 ){
      perror("inotify_init");
      return ERROR;
   }

   /* wd is the unique watch descriptor */
   if( ( wd = inotify_add_watch( fd, path, IN_MODIFY ) ) == -1 ){
      perror("inotify_add_watch");
      return ERROR;
   }

   while (1){
      /*reading from wd, if nothing is ready we block!*/
      if( ( xread( fd, buf, BUF_LEN) ) == ERROR ){
         perror ("read");
         return ERROR;
      }

      event = (struct inotify_event *) buf;

      if( ( event->mask & IN_MODIFY ) == IN_MODIFY)
         callback(path);
   }
   return 0;
}
