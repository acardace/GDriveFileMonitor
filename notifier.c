/* this a set of functions which takes in input
 * a file to watch and whenever the file is updated
 * the function "callback" is executed
 */

#include<sys/inotify.h>
#include<unistd.h>
#include<stdio.h>
#include<limits.h>
#include<errno.h>
#include<stdlib.h>

#define BUF_LEN sizeof(struct inotify_event) + NAME_MAX + 1

/* just a wrapper for read() */
void xread(int fd, void *buf, size_t len){
   ssize_t ret;

   while ( (ret = read (fd, buf, len)) == -1) {
      if (errno == EINTR){
         buf += ret;
         continue;
      }
      perror ("read");
      break;
   }
}

/*this function will monitor modifications
 * on the file "path", and when one
 * occurs the "callback" function gets called
 */
void file_watch(char *path, void (*callback)(char *filepath) ){
   char buf[BUF_LEN];
   struct inotify_event *event;
   int fd, wd;

   /* fd is the inotify istance*/
   if ( ( fd = inotify_init() ) == -1 ){
      perror("inotify_init");
      exit(1);
   }

   /* wd is the unique watch descriptor */
   if( ( wd = inotify_add_watch( fd, path, IN_MODIFY ) ) == -1 ){
      perror("inotify_add_watch");
      exit(1);
   }

   while ( 1 ){
      /*reading from wd, if nothing is ready we block!*/
      xread( fd, buf, BUF_LEN);

      event = (struct inotify_event *) buf;

      if( ( event->mask & IN_MODIFY ) == IN_MODIFY)
         callback(path);
   }
}
