#ifndef _BASICFUNCTION_H
#define _BASICFUNCTION_H

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <netdb.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/stat.h>
#include "glovari.h"

//从argv[]中获取url
void getURL(char argc, char const*argv[], char *url); 

//	从URL中解析所需要的ip,端口，文件名等信息。	
void paresURL(const char *url, char *domain, int *port, char *fileName);

//  通过域名得到相应的ip地址
void getIP(char *domain, char *ipAddr);

//拼接请求报文
void setRequrement(char *header, char *domain, char *url);

//
int creatFile();

struct respHeader getRespHeader(char *response); //提取所需信息

void getResp(int sockfd, char *header, const char *fileName, int threadOrNot); //获得服务器回应

int resumeOrNot(const char *fileName);//判断是否需要断点续传

void cacuBegin(const char *masgeFileName);//计算每个线程的起始点
char* creatMasgetext(const char*fileName);//生成massagetext的名字
void prepareDownload(int sockfd, char* header, const char *fileName);
void setConst(const char* ipAddr, const char *domain, const char *url, const int port);


#endif