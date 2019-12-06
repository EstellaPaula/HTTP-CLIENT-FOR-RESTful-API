#include <stdio.h>      /* printf, sprintf */
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "helpers.h"
#include "requests.h"
#include "parson.h"
#include <sys/types.h>


void get_ip(const char* name, const char* ip)
{
    struct addrinfo hints, *result, *p;
    hints.ai_socktype = 0;
    hints.ai_protocol = 0;
    hints.ai_flags = 0;
    hints.ai_family = AF_INET;
    getaddrinfo(name, NULL, &hints, &result);
    struct sockaddr_in *adr;
    p = result;

    while(p!= NULL) {
        adr = (struct sockaddr_in*) p->ai_addr;
        strncpy((char*)ip, inet_ntoa(adr->sin_addr),15);
        p = p->ai_next;
    }

    freeaddrinfo(result);
}

int main(int argc, char *argv[])
{
    char *message;
    char *responseS;
    int sockfd;

    sockfd = open_connection("185.118.200.35", 8081, AF_INET, SOCK_STREAM, 0);
    message = compute_get_request("185.118.200.35", "/task1/start", NULL);
    printf("%s\n", message);
    send_to_server(sockfd, message);
    responseS = receive_from_server(sockfd);
    printf("%s\n", responseS);
    close_connection(sockfd);

    //data set task1
    char* resp1 = NULL;
    resp1 = strtok(responseS, "\n");
    char* cookie1[5];
    int c1 = 0;

    //------------------get cookies---------------
    while( resp1 != NULL ) {
        if(strncmp(resp1,"{",1) == 0) {
            break;
        } else if (strncmp(resp1,"Set-Cookie:",11) == 0) {
            char* tmp = strstr(resp1, ";");
            cookie1[c1] = malloc(strlen(resp1) - strlen(tmp));
            memset(cookie1[c1], 0, strlen(resp1) - strlen(tmp));
            strncpy(cookie1[c1], resp1, strlen(resp1) - strlen(tmp));
            cookie1[c1] = cookie1[c1] + 4;
            c1++;
        }
      resp1 = strtok(NULL, "\n");
    }

    //-----------------parse JSON----------------------
    JSON_Value *schema = json_parse_string(resp1);
    JSON_Object *data = NULL;
    const char *username = NULL;
    const char *password = NULL;
    const char *url = NULL;
    const char *method = NULL;

    url = json_object_get_string(json_object(schema), "url");
    data = json_object_dotget_object (json_object(schema), "data");
    method = json_object_get_string(json_object(schema), "method");
    username = json_object_get_string(data, "username");
    password = json_object_get_string(data, "password");

    sockfd = open_connection("185.118.200.35", 8081, AF_INET, SOCK_STREAM, 0);
    int len = strlen(username) + strlen(password) + strlen("username=") + strlen("&password=");
    char *credentials = malloc(len + 1);
    strcpy(credentials, "username=");
    strcat(credentials, username);
    strcat(credentials, "&password=");
    strcat(credentials, password);

    //------------------------------authorization TASK 2----------------
    c1--;
    printf("nr cookies %d\n", c1);
    if(strcmp(method,"POST") == 0) {
        message = compute_post_request("185.118.200.35", url, credentials, cookie1, c1);
    } else {
         message = compute_get("185.118.200.35", url, credentials, cookie1, c1);
    }
    send_to_server(sockfd, message);
    printf("------>Sent : \n%s\n", message);
    responseS = receive_from_server(sockfd);
    printf("------>Received : \n%s\n", responseS);
    close_connection(sockfd);

    
    char* resp2 = NULL;
    resp2 = strtok(responseS, "\n");
    printf("%s\n", resp2);
    char* cookie2[5];
    int c2 = 0;

    //-----------get cookies -----------------------
    while( resp2 != NULL ) {
        if(strncmp(resp2,"{",1) == 0) {
            break;
        } else if (strncmp(resp2,"Set-Cookie:",11) == 0) {
            char* tmp = strstr(resp2, ";");
            cookie2[c2] = malloc(strlen(resp2) - strlen(tmp));
            memset(cookie2[c2], 0, strlen(resp2) - strlen(tmp));
            strncpy(cookie2[c2], resp2, strlen(resp2) - strlen(tmp));
            cookie2[c2] = cookie2[c2] + 4;
            c2++;
        }
      resp2 = strtok(NULL, "\n");
    }

    //------------parse JSON-----------------
    JSON_Value *obj = json_parse_string(resp2);
    JSON_Object *data2 = NULL;
    JSON_Object *queryParams = NULL;
    const char *id2 = NULL;
    const char *token = NULL;
    const char *url2 = NULL;
    const char *method2 = NULL;

    url2 = json_object_get_string(json_object(obj), "url");
    method2 = json_object_get_string(json_object(obj), "method");
    strcat((char*)url2, "?raspuns1=omul&raspuns2=numele&id=");
    data2 = json_object_dotget_object(json_object(obj), "data");
    token = json_object_get_string(data2, "token");
    queryParams = json_object_dotget_object(data2, "queryParams");
    id2 = json_object_get_string(queryParams, "id");
    strcat((char*)url2,id2);

    //-----------------send token TASK3-------------------------
    sockfd = open_connection("185.118.200.35", 8081, AF_INET, SOCK_STREAM, 0);
     if(strcmp(method2,"GET") == 0) {
        message = compute_get("185.118.200.35", url2, token, cookie2, c2);
    } else {
        message = compute_post_request("185.118.200.35", url2, token, cookie2, c2);
    }
    send_to_server(sockfd, message);
    printf("----->Sent : \n%s\n", message);
    responseS = receive_from_server(sockfd);
    printf("----->Received : \n%s\n", responseS);
    close_connection(sockfd);


    
    char* resp3 = NULL;
    resp3 = strtok(responseS, "\n");
    char* cookie3[5];
    int c3 = 0;

    //-------------get cookies-------------------
    while( resp3 != NULL ) {
        if(strncmp(resp3,"{",1) == 0) {
            break;
        } else if (strncmp(resp3,"Set-Cookie:",11) == 0) {
            char* tmp = strstr(resp3, ";");
            cookie3[c3] = malloc(strlen(resp3) - strlen(tmp));
            memset(cookie3[c3], 0, strlen(resp3) - strlen(tmp));
            strncpy(cookie3[c3], resp3, strlen(resp3) - strlen(tmp));
            cookie3[c3] = cookie3[c3] + 4;
            c3++;
        }
      resp3 = strtok(NULL, "\n");
    }

    //------------parse JSON---------------------------
    JSON_Value *obj2 = json_parse_string(resp3);
    const char *url3 = NULL;
    const char *method3 = NULL;
    url3 = json_object_get_string(json_object(obj2), "url");
    method3 = json_object_get_string(json_object(obj2), "method");

    //-------------------get free points TASK4-------------------------------
    sockfd = open_connection("185.118.200.35", 8081, AF_INET, SOCK_STREAM, 0);
     if(strcmp(method3,"GET") == 0) {
        message = compute_get("185.118.200.35", url3, token, cookie3, c3);
    } else {
        message = compute_post_request("185.118.200.35", url3, token, cookie3, c3);
    }
    send_to_server(sockfd, message);
    printf("----->Sent : \n%s\n", message);
    responseS = receive_from_server(sockfd);
    printf("----->Received : \n%s\n", responseS);
    close_connection(sockfd);

    char* resp4 = NULL;
    resp4 = strtok(responseS, "\n");
    char* cookie4[5];
    int c4 = 0;

    //-------------get cookies----------------------
    while( resp4 != NULL ) {
        if(strncmp(resp4,"{",1) == 0) {
            break;
        } else if (strncmp(resp4,"Set-Cookie:",11) == 0) {
            char* tmp = strstr(resp4, ";");
            cookie4[c4] = malloc(strlen(resp4) - strlen(tmp));
            memset(cookie4[c4], 0, strlen(resp4) - strlen(tmp));
            strncpy(cookie4[c4], resp4, strlen(resp4) - strlen(tmp));
            cookie4[c4] = cookie4[c4] + 4;
            c4++;
        }
      resp4 = strtok(NULL, "\n");
    }
    char* te = *cookie4;
    int l = strlen(cookie4[0]) - 1;
    *(te + l) = NULL; 
    //---------parse JSON-----------------
    JSON_Value *obj4 = json_parse_string(resp4);
    JSON_Object *data4 = NULL;
    const char *url4 = NULL;
    const char *urlWheather = NULL;
    const char *method4 = NULL;
    const char *type = NULL;
    const char *urlToSend = NULL;
    const char *urlToSendTrue = NULL;
    const char *urlFinalWheather = malloc(17);
    char *ipWheather = malloc(15);
    const char* q = NULL;
    const char* aPPID = NULL;
    const char* methodInner = NULL;
    url4 = json_object_get_string(json_object(obj4), "url");
    method4 = json_object_get_string(json_object(obj4), "method");
    type = json_object_get_string(json_object(obj4), "type");
    data4 = json_object_dotget_object(json_object(obj4), "data");
    urlToSend = json_object_get_string(data4, "url");
    queryParams = json_object_dotget_object(data4, "queryParams");
    q = json_object_get_string(queryParams, "q");
    aPPID = json_object_get_string(queryParams, "APPID");
    methodInner = json_object_get_string(data4, "method");

    //---------------parse IP domain------------------
    const char s[2] = "/";
    urlWheather = strtok((char*)urlToSend, s);
    urlToSendTrue = strtok(NULL, "\n");
    strcat((char*)urlFinalWheather,"/");
    strcat((char*)urlFinalWheather,urlToSendTrue);
    strcat((char*)urlFinalWheather,"?q=");
    strcat((char*)urlFinalWheather,q);
    strcat((char*)urlFinalWheather,"&APPID=");
    strcat((char*)urlFinalWheather,aPPID);
    

    //---------------get IP---------------------------
    get_ip(urlWheather, ipWheather);
    sockfd = open_connection(ipWheather, 80, AF_INET, SOCK_STREAM, 0);
     if(strcmp(methodInner,"GET") == 0) {
        message = compute_get(ipWheather, urlFinalWheather, NULL, NULL, 0);
    } else {
        message = compute_post_request(ipWheather, urlFinalWheather, NULL, NULL, 0);
    }
    send_to_server(sockfd, message);
    responseS = receive_from_server(sockfd);
    printf("%s\n", responseS);
    close_connection(sockfd);

    char* resp5 = NULL;
    resp5 = strtok(responseS, "\n");
    char* cookie5[5];
    int c5 = 0;

    //------get cookies-------------------
    while( resp5 != NULL ) {
        if(strncmp(resp5,"{",1) == 0) {
            break;
        } else if (strncmp(resp5,"Set-Cookie:",11) == 0) {
            char* tmp = strstr(resp5, ";");
            cookie5[c5] = malloc(strlen(resp5) - strlen(tmp));
            memset(cookie5[c5], 0, strlen(resp5) - strlen(tmp));
            strncpy(cookie5[c5], resp5, strlen(resp5) - strlen(tmp));
            cookie5[c5] = cookie5[c5] + 4;
            c5++;
        }
      resp5 = strtok(NULL, "\n");
    }


    //-----------------------send wheather details TASK5---------------------
    sockfd = open_connection("185.118.200.35", 8081, AF_INET, SOCK_STREAM, 0);
         if(strcmp(method4,"GET") == 0) {
            message = compute_get("185.118.200.35", url4, token, cookie4, c4);
        } else {
            message = compute_post("185.118.200.35", url4, type, resp5, token, cookie4, c4);
    }
    printf("-------> Sent : \n%s\n",message);
    send_to_server(sockfd, message);
    responseS = receive_from_server(sockfd);
    printf("-------> Received : \n%s\n", responseS);
    close_connection(sockfd);
    

    free(message);
    return 0;
}