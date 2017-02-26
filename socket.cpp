#include "socket.h"

int setConnect(){
    int sockfd;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0){
        printf("invalid socket descriptor: %d\n", sockfd);
        exit(-1);
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(realip);
    addr.sin_port = htons(realport);

    int res;
    if((res = connect(sockfd, (struct sockaddr *) &addr, sizeof(addr))) == -1){
        printf("connect failed, return: %d\n", res);
        exit(-1);
    }
    return sockfd;
}