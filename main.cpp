#include <sys/socket.h>
#include <arpa/inet.h>
#include <signal.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
using namespace std;

#define BUFFER_SIZE 1024

int main(int arg, char* argv[])
{
    if(arg<=2){
        printf("usage:%s ip port\n",basename(argv[0]));
        return 0;
    }
    const char* ip=argv[1];
    int port = atoi(argv[2]);

    struct sockaddr_in serveraddr;
    bzero(&serveraddr,sizeof(serveraddr));

    //地址族，源ip，存放的目标串
    inet_pton(AF_INET, ip, &serveraddr.sin_addr);
    serveraddr.sin_family=AF_INET;
    serveraddr.sin_port=port;

    //协议族，流服务，默认0
    int sockfd=socket(PF_INET, SOCK_STREAM, 0);
    assert(sockfd>=0);

    int ret;
    ret=bind(sockfd,(struct sockaddr*)&serveraddr, sizeof(serveraddr));
    assert(ret>=0);

    ret=listen(sockfd,5);
    assert(ret>=0);
    printf(" im listening\n");

    char remote[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(serveraddr.sin_addr),remote,sizeof(remote));
    printf("my ip is%s,port is %d\n",remote,ntohs(serveraddr.sin_port));

    struct sockaddr_in clientaddr;
    bzero(&clientaddr,sizeof(clientaddr));
    socklen_t clientlen=sizeof(clientaddr);


    int connfd=accept(sockfd,(struct sockaddr *)&clientaddr,&clientlen);
    if(connfd<0){
        printf("connect error!\n");
    }
    else{

    char buffer[BUFFER_SIZE];

    memset(buffer, '\0',BUFFER_SIZE);
    int n=recv(connfd,buffer,BUFFER_SIZE,0);
    printf("got %d byte,%s\n", n, buffer);

    memset(buffer, '\0',BUFFER_SIZE);
    n=recv(connfd,buffer,BUFFER_SIZE,MSG_OOB);
    printf("got %d byte OOB %s\n", n, buffer);

    memset(buffer, '\0',BUFFER_SIZE);
    n=recv(connfd,buffer,BUFFER_SIZE,0);
    printf("got %d byte, %s\n",n,buffer);
    }


    close(connfd);
    close(sockfd);
    return 0;
}