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
 * This file exposes a set of functionalities
 * for uploading a file into a user's Google Drive
 */

#ifndef __AUTOUPLOADER_UPLOADER__
#define __AUTOUPLOADER_UPLOADER__

#include "jsmn/jsmn.h"

#define URL_LEN 256
#define ACCESS_RESPONSE_SIZE 9
#define REFRESH_RESPONSE_SIZE 7
#define JSON_FILENAME "/.google-token.json"
#define RESPONSE_ERROR "error"

#define AUTH "https://accounts.google.com/o/oauth2/auth?"
#define SCOPE "scope=https://www.googleapis.com/auth/drive.file&"
#define CLIENT_ID "client_id=110801271884-5b1rsuq2k7efitrq5tpkb8crdmp4ev38.apps.googleusercontent.com&"
#define CLIENT_SECRET "client_secret=Pdb90_9EjZa-V712MVIwMaor&"
#define RESPONSE_TYPE "response_type=code&"
#define REDIRECT_URI "redirect_uri=urn:ietf:wg:oauth:2.0:oob"
#define AUTH_URL AUTH SCOPE CLIENT_ID RESPONSE_TYPE REDIRECT_URI

#define AUTH2 "https://www.googleapis.com/oauth2/v3/token"
#define GRANT_TYPE_REFRESH "grant_type=refresh_token"
#define GRANT_TYPE_AUTH "grant_type=authorization_code"

#define ACCESS_REQUEST 0
#define REFRESH_REQUEST 1

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
json_struct *exchange_code_for_token(char *code, json_struct *json_data, char *json_filepath);


/* read json from previously saved file, and
 * if needed get an access token using a refresh token
 */
json_struct *get_access_token(json_struct *json_data, char *json_filepath);

#endif
