/*
 * GDriveFileMonitor.
 * Copyright (C) 2015  Antonio Cardace.
 *
 * This file is part of GDriveFileMonitor.
 *
 * Wavetrack is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * Wavetrack is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */

/*
 * this a set of functions which takes in input
 * a file to watch and whenever the file is updated
 * the function "callback" is executed
 */

#ifndef __GDRIVEFILEMONITOR_NOTIFIER__
#define __GDRIVEFILEMONITOR_NOTIFIER__

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
