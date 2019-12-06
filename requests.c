#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <stdio.h>
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "helpers.h"
#include "requests.h"

char *compute_get_request(char *host, char *url, char *url_params) {
    char *message = calloc(BUFLEN, sizeof(char));
    char *line = calloc(LINELEN, sizeof(char));

    if (url_params != NULL) {
        sprintf(line, "GET %s?%s HTTP/1.1", url, url_params);
    } else {
        sprintf(line, "GET %s HTTP/1.1", url);
    }
    compute_message(message, line);
    sprintf(line, "Host: %s", host);
    compute_message(message, line);
    compute_message(message, "");
    return message;
}

char *compute_get(char *host, const char *url, const char *url_params, char **cookie,int nrCookies) {
    char *message = calloc(BUFLEN, sizeof(char));
    char *line = calloc(LINELEN, sizeof(char));
    sprintf(line, "GET %s HTTP/1.1", url);
    compute_message(message, line);
    sprintf(line, "Host: %s", host);
    compute_message(message, line);

   	if (url_params != NULL) {
   		sprintf(line, "Authorization: Bearer %s", url_params);
   		compute_message(message, line);
   	}

   	if (cookie != NULL) {
	   	strcat(message, cookie[0]);
	    strcat(message, "; ");
	    strcat(message, (cookie[1] + 7));
	    int i = 2;
	    while((i < nrCookies) && (nrCookies >= 2)){
	    	strcat(message, cookie[i] + 7);
	    	nrCookies--;
	    	i++;
    	}
    	strcat(message, "\n");
	}
    
    compute_message(message, "");
    return message;
}

char *compute_post_request(char *host, const char *url, const char *form_data, char **cookie, int nrCookies) {

    char *message = calloc(BUFLEN, sizeof(char));
    char *line = calloc(LINELEN, sizeof(char));
    sprintf(line, "POST %s HTTP/1.1", url);
    compute_message(message, line);
    sprintf(line, "Host: %s", host);
    compute_message(message, line);
    sprintf(line, "Content-Type: application/x-www-form-urlencoded");
    compute_message(message, line);
    sprintf(line, "Content-Length: %ld", strlen(form_data));
    compute_message(message, line);
    //sprintf(line,"%s",cookie[0]);
    strcat(message, cookie[0]);
    strcat(message, "; ");
    strcat(message, (cookie[1] + 7));
    int i = 2;
    while((i < nrCookies) && (nrCookies >= 2)){
    	strcat(message, cookie[i] + 7);
    	nrCookies--;
    	i++;
    }
    strcat(message,"\n");
    compute_message(message, "");
    sprintf(line, "%s", form_data);
    compute_message(message, line);
    return message;
}

char *compute_post(char *host, const char *url, const char* type, const char *form_data, const char* token, char **cookie,int nrCookies) {

    char *message = calloc(BUFLEN, sizeof(char));
    char *line = calloc(LINELEN, sizeof(char));
 
    sprintf(line, "POST %s HTTP/1.1", url);
    compute_message(message, line);
    sprintf(line, "Host: %s", host);
    compute_message(message, line);
    sprintf(line, "Content-Type: %s",type);
    compute_message(message, line);
    sprintf(line, "Content-Length: %ld", strlen(form_data));
    compute_message(message, line);
    sprintf(line, "Authorization: Bearer %s", token);
   	compute_message(message, line);
    if (cookie != NULL) {
	   	strcat(message, cookie[0]);
	    strcat(message, "; ");
	    strcat(message, (cookie[1] + 7));
	    int i = 2;
	    while((i < nrCookies) && (nrCookies >= 2)){
	    	strcat(message, cookie[i] + 7);
	    	nrCookies--;
	    	i++;
    	}
    	strcat(message, "\n");
	}
    
    compute_message(message, "");
    sprintf(line, "%s", form_data);
    compute_message(message, line);
    return message;
}