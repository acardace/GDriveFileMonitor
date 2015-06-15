/*
 * the real daemon itself
 * which monitors a file and uploads it
 * in Google Drive whenever a modification
 * occurs
 */

#include "notifier.h"
#include "googleOAuth.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[])
{
   json_struct *json_data = (json_struct *) malloc( sizeof(json_struct) * JSON_RESPONSE_SIZE );
   char *json_file_path = (char *) malloc( sizeof(char) * 128 );

   json_file_path = strcpy( json_file_path, getenv("HOME") );
   json_file_path = strcat( json_file_path, JSON_FILENAME );

   /*check if already have an access token*/
   if( access(json_file_path, R_OK) == -1 ){
      char *url = (char *) malloc( sizeof(char) * URL_LEN );
      char *code = (char *) malloc( sizeof(char) * URL_LEN );

      url = obtain_auth_code_url( url );

      printf("Copy this link and paste it into your browser:\n%s\n\n",url);
      printf("Paste the code you obtained:\n");

      scanf("%s",code);

      if( !(json_data = exchange_code_for_token( code, json_data, json_file_path) ) )
         exit(1);

      free(url);
      free(code);
   }else{
      if( ! ( json_data = get_access_token(json_data, json_file_path) ) )
         exit(1);
   }

   free(json_file_path);
   free(json_data);
   return 0;
}
