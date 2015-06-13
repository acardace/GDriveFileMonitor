/*
 * This file exposes a set of functionalities
 * for uploading a file into a user's Google Drive
 */

#include "uploader.h"
#include <string.h>
#include <stdio.h>

/*
 * this function builds up the
 * string which is to be copied
 * in the browser to obtain
 * the permissions to use Google API's
 *
 * the url string is to be allocated by
 * the caller
 */
char *obtain_auth_code_url(char *url)
{
   url = strcpy(url, AUTH_URL);
   return url;
}
