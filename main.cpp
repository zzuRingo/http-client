#include "download.h"

int main(char argc, char const *argv[]){
    char url[URLLEN] = "127.0.0.1";
    char domain[DOMLEN] = {0};
    char ipAddr[16] = {0};
    int port = HTTPPORT;
    char fileName[FILENAMELEN] = {0};

    getURL(argc, argv, url);
    //printf("url:%s\n",url);
    
    puts("1: Parsing url...");
    paresURL(url, domain, &port, fileName);

    puts("2: Get ip address...");
    getIP(domain, ipAddr);
    setConst(ipAddr, domain, url, port);

    puts("\n>>>>Detail<<<<");
    printf("URL: %s\n", url);
    printf("DOMAIN: %s\n", domain);
    printf("IP: %s\n", ipAddr);
    printf("PORT: %d\n", port);
    printf("FILENAME: %s\n\n", fileName); 

    //设置http请求头信息
    char header[REQURELEN] = {0};
    setRequrement(header, domain, url);

    puts("3: Connect server...");
    int sockfd = setConnect();

    puts("4: Send request...");
    prepareDownload(sockfd, header, fileName);

    printf("5: Start thread to download...\n");
    multiThreadDownload();
    
    return 0;
}


