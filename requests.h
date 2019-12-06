#ifndef _REQUESTS_
#define _REQUESTS_

char *compute_get_request(char *host, char *url, char *url_params);
char *compute_get(char *host, const char *url, const char *url_params,char **cookie,int nrCookies);
char *compute_post_request(char *host, const char *url, const char *form_data, char **cookie,int nrCookies);
char *compute_post(char *host, const char *url, const char* type, const char *form_data, const char* token, char **cookie,int nrCookies);

#endif