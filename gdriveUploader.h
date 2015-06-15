/*
 * This file exposes a set of functionalities
 * for uploading a file into a user's Google Drive
 */

#ifndef __AUTOUPLOADER_UPLOADER__
#define __AUTOUPLOADER_UPLOADER__

#include "jsmn/jsmn.h"

#define URL_LEN 256
#define JSON_RESPONSE_SIZE 9

#define AUTH "https://accounts.google.com/o/oauth2/auth?"
#define SCOPE "scope=https://www.googleapis.com/auth/drive&"
#define CLIENT_ID "client_id=110801271884-5b1rsuq2k7efitrq5tpkb8crdmp4ev38.apps.googleusercontent.com&"
#define CLIENT_SECRET "client_secret=Pdb90_9EjZa-V712MVIwMaor&"
#define RESPONSE_TYPE "response_type=code&"
#define REDIRECT_URI "redirect_uri=urn:ietf:wg:oauth:2.0:oob"
#define AUTH_URL AUTH SCOPE CLIENT_ID RESPONSE_TYPE REDIRECT_URI

#define AUTH2 "https://www.googleapis.com/oauth2/v3/token"
#define GRANT_TYPE "grant_type=authorization_code"

typedef struct {
   char access_token[256];
   long int expires_in;
   char token_type[8];
   char refresh_token[256];
} json_struct;

/*
 * this function builds up the
 * string which is to be copied
 * in the browser to obtain
 * the permissions to use Google API's
 *
 * the url string is to be allocated by
 * the caller
 */
char *obtain_auth_code_url(char *url);

/* this function perform a POST request
 * to obtain the access token from the
 * Google API authorization service
 */
json_struct *exchange_code_for_token(char *code, json_struct *json_data);

#endif
