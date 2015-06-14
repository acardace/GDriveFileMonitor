/*
 * This file exposes a set of functionalities
 * for uploading a file into a user's Google Drive
 */

#include "uploader.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>

/* if 1 curl has been init*/
static int curl_init = 0;

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

char *exchange_code_for_token(char *code)
{
   CURL *handle;

   /*set up POST data*/
   char post_data[URL_LEN];
   strcpy(post_data, "code=");
   strcat(post_data, code);
   strcat(post_data, "&");
   strcat(post_data, CLIENT_ID);
   strcat(post_data, CLIENT_SECRET);
   strcat(post_data, REDIRECT_URI);
   strcat(post_data, "&");
   strcat(post_data, GRANT_TYPE);

   /* init curl */
   if( !curl_init ){
      if( curl_global_init(CURL_GLOBAL_SSL) != 0 ){
         perror("curl");
         return NULL;
      }
      curl_init = 1;
   }

   if( !( handle = curl_easy_init() ) ){
      perror("curl");
      return NULL;
   }

   curl_easy_setopt(handle, CURLOPT_VERBOSE, 1L );
   curl_easy_setopt(handle, CURLOPT_HEADER, 1L );
   curl_easy_setopt(handle, CURLOPT_URL, AUTH2 );
   curl_easy_setopt(handle, CURLOPT_POST, 1L );
   curl_easy_setopt(handle, CURLOPT_POSTFIELDS, post_data);
   curl_easy_perform(handle);

   curl_easy_cleanup(handle);

   return NULL;
}
