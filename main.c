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
   char *code_url = (char *) malloc( sizeof(char) * URL_LEN );
   code_url = obtain_auth_code_url( code_url );

   printf("Copy this link and paste it into your browser:\n%s\n",code_url);

   return 0;
}
