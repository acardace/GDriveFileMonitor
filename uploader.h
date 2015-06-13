/*
 * This file exposes a set of functionalities
 * for uploading a file into a user's Google Drive
 */

#ifndef __AUTOUPLOADER_UPLOADER__
#define __AUTOUPLOADER_UPLOADER__

#define AUTH "https://accounts.google.com/o/oauth2/auth?"
#define SCOPE "scope=https://www.googleapis.com/auth/drive&"
#define CLIENT_ID "client_id=110801271884-5b1rsuq2k7efitrq5tpkb8crdmp4ev38.apps.googleusercontent.com&"
#define CLIENT_SECRET "client_secret=Pdb90_9EjZa-V712MVIwMaor&"
#define RESPONSE_TYPE "response_type=code&"
#define REDIRECT_URI "redirect_uri=urn:ietf:wg:oauth:2.0:oob"

#define AUTH_URL AUTH SCOPE CLIENT_ID RESPONSE_TYPE REDIRECT_URI
#define URL_LEN 256

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

#endif
