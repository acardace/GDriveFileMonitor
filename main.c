/*
 * the real daemon itself
 * which monitors a file and uploads it
 * in Google Drive whenever a modification
 * occurs
 */

#include "notifier.h"
#include "uploader.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
   char *url = (char *) malloc( sizeof(char) * URL_LEN );
   char *json_data = (char *) malloc( sizeof(char) * URL_LEN );
   char *code = (char *) malloc( sizeof(char) * URL_LEN );

   url = obtain_auth_code_url( url );

   printf("Copy this link and paste it into your browser:\n%s\n\n",url);
   printf("Paste the code you obtained:\n");

   scanf("%s",code);

   if( !(json_data = exchange_code_for_token( code ) ) ){
      exit(1);
   }

   return 0;
}
