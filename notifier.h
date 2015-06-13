/*
 * this a set of functions which takes in input
 * a file to watch and whenever the file is updated
 * the function "callback" is executed
 */

#ifndef __AUTOUPLOADER_NOTIFIER__
#define __AUTOUPLOADER_NOTIFIER__

#include <stdio.h>

#define BUF_LEN sizeof(struct inotify_event) + NAME_MAX + 1
#define ERROR -1

/* just a wrapper for read() */
int xread(int fd, void *buf, size_t len);

/*this function will monitor modifications
 * on the file "path", and when one
 * occurs the "callback" function gets called
 */
int file_watch(char *path, void (*callback)(char *filepath) );

#endif
