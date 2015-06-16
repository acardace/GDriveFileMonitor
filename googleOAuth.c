/*
 * This file exposes a set of functionalities
 * for uploading a file into a user's Google Drive
 */

#include "googleOAuth.h"
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <curl/curl.h>
#include <sys/types.h>
#include <sys/stat.h>

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
json_struct *json_parser(char *json, json_struct *json_data)
{
   jsmn_parser parser;
   jsmnerr_t err;
   jsmntok_t jsmn_json[JSON_RESPONSE_SIZE];
   char expires_in_str[8];

   /*init the parser*/
   jsmn_init(&parser);

   /* parse the content of "json"*/
   if( (err = jsmn_parse(&parser, json, strlen(json), jsmn_json,JSON_RESPONSE_SIZE) ) < 0 ){
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

   /* put the parsed content in a json_struct*/
   memcpy( json_data->access_token, json + jsmn_json[2].start, jsmn_json[2].end - jsmn_json[2].start);
   memcpy( json_data->token_type, json + jsmn_json[4].start, jsmn_json[4].end - jsmn_json[4].start);
   memcpy( expires_in_str, json + jsmn_json[6].start, jsmn_json[6].end - jsmn_json[6].start);
   json_data->expires_in = strtol( expires_in_str, NULL, 10);
   memcpy( json_data->refresh_token, json + jsmn_json[8].start, jsmn_json[8].end - jsmn_json[8].start);

   return json_data;
}

int init_curl(){
   CURLcode error;
   if( !curl_init ){
      if( ( error = curl_global_init(CURL_GLOBAL_SSL) ) != 0 ){
         printf("%s\n", curl_easy_strerror(error) );
         return -1;
      }
      curl_init = 1;
   }
   return 0;
}

/*
 * write the obtained json content
 * into a file
 */
int json_to_file(char *path, json_struct *json_data){
   FILE *out;
   if( !( out = fopen(path, "w+") ) ){
      perror("fopen");
      return -1;
   }

   if( fprintf(out, "access_token=%s\nrefresh_token=%s\ntoken_type=%s\nexpires_in=%ld", json_data->access_token, json_data->refresh_token, json_data->token_type, json_data->expires_in ) < 0 ){
      perror("fprintf");
      return -1;
   }
   if( fclose(out) ){
      perror("fclose");
      return -1;
   }
   return 0;
}

/*
 * read the json into
 * a json_struct
 */
json_struct *json_from_file(char *path, json_struct *json_data)
{
   FILE *in;
   if( !( in = fopen(path, "r") ) ){
      perror("fopen");
      return NULL;
   }

   if( fscanf(in, "access_token=%s\nrefresh_token=%s\ntoken_type=%s\nexpires_in=%ld", json_data->access_token, json_data->refresh_token, json_data->token_type, &json_data->expires_in ) < 0 ){
      perror("fscanf");
      return NULL;
   }
   if( fclose(in) ){
      perror("fclose");
      return NULL;
   }
   return json_data;
}

/* this function perform a POST request
 * to obtain the access token from the
 * Google API authorization service
 */
json_struct *exchange_code_for_token(char *code, json_struct *json_data, char *json_filepath)
{
   CURL *handle;
   char *json_buffer = (char *) malloc(sizeof(char) * URL_LEN );
   json_struct *ret;
   CURLcode error;

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

   if( init_curl() == -1 )
      return NULL;

   if( !( handle = curl_easy_init() ) ){
      printf("curl_easy_init: something went very wrong with curl initialization\n");
      return NULL;
   }

   curl_easy_setopt(handle, CURLOPT_URL, AUTH2 );
   curl_easy_setopt(handle, CURLOPT_POST, 1L );
   curl_easy_setopt(handle, CURLOPT_POSTFIELDS, post_data);
   curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_callback);
   curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void *) json_buffer);
   if( ( error = curl_easy_perform(handle) ) < 0 ){
      printf("%s\n", curl_easy_strerror(error) );
      return NULL;
   }

   ret = json_parser(json_buffer, json_data);

   if( json_to_file(json_filepath, json_data) < 0 ){
      return NULL;
   }

   curl_easy_cleanup(handle);
   free(json_buffer);
   return ret;
}

/* read json from previously saved file, and
 * if needed get an access token using a refresh token
 */
json_struct *get_access_token(json_struct *json_data, char *json_filepath){
   struct stat buf;
   long int creation_elapsed_time;
   long int token_elapsed_time;
   struct timespec tm_now;

   if( stat(json_filepath, &buf) == -1 ){
      perror("stat");
      return NULL;
   }

   creation_elapsed_time = buf.st_mtim.tv_sec;
   json_data = json_from_file(json_filepath, json_data);

   if( clock_gettime( CLOCK_REALTIME, &tm_now ) == -1 ){
      perror("clock_gettime");
      return NULL;
   }
   token_elapsed_time = tm_now.tv_sec - creation_elapsed_time;

   /* the access token has expired (less than 10 seconds remaining)
    * , we have to get a new one using the refresh token*/
   if( json_data->expires_in - token_elapsed_time < 10 ){
   }
   else

   return json_data;
}
