/*
 * This file exposes a set of functionalities
 * for uploading a file into a user's Google Drive
 */

#include "gdriveUploader.h"
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

/*callback function for curl_easy_perform
 * it just saves the received data in a buffer
 */
size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata)
{
   memcpy(userdata, ptr, size*nmemb);
   userdata += size*nmemb;
   return size;
}

/* json parser
 * saves the content of "json" in json_data
 */
jsmntok_t *json_parser(char *json, jsmntok_t *json_data)
{
   jsmn_parser parser;
   jsmnerr_t err;

   /*init the parser*/
   jsmn_init(&parser);

   /* parse the content of "json"*/
   if( (err = jsmn_parse(&parser, json, strlen(json), json_data,JSON_RESPONSE_SIZE) ) < 0 ){
      switch( err ){
         case JSMN_ERROR_INVAL:
            printf("bad token, JSON string is corrupted\n");
            break;
         case JSMN_ERROR_NOMEM:
            printf("not enough tokens, JSON string is too large\n");
            break;
         case JSMN_ERROR_PART:
            printf("JSON string is too short, expecting more JSON data\n");
            break;
      }
      return NULL;
   }

   return json_data;
}

/* this function perform a POST request
 * to obtain the access token from the
 * Google API authorization service
 */
jsmntok_t *exchange_code_for_token(char *code, jsmntok_t *json_data, char *json_buffer)
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

   curl_easy_setopt(handle, CURLOPT_URL, AUTH2 );
   curl_easy_setopt(handle, CURLOPT_POST, 1L );
   curl_easy_setopt(handle, CURLOPT_POSTFIELDS, post_data);
   curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_callback);
   curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void *) json_buffer);
   curl_easy_perform(handle);

   curl_easy_cleanup(handle);

   return json_parser(json_buffer, json_data);
}
