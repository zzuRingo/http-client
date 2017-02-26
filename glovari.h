#ifndef _GLOVARI_H
#define _GLOVARI_H

#define URLLEN 2048
#define REQURELEN 1024
#define DOMLEN 64
#define HTTPPORT 80
#define FILENAMELEN 256
#define BUFFSIZE 4096
#define THREADAMOUNT 3
#define fileExtention ".txt"

typedef struct respHeader//保持相应头信息
{
    int status_code;//HTTP/1.1 '200' OK
    char content_type[FILENAMELEN];//Content-Type: application/gzip
    long content_length;//Content-Length: 11683079
    char file_name[FILENAMELEN];
}respHeader;

extern struct respHeader resp;//全局
extern long begin[THREADAMOUNT+1];
extern long init[THREADAMOUNT+1];
extern long chunklen;
extern char realip[16];
extern char realdomain[DOMLEN];
extern char realurl[URLLEN];
extern int realport;
extern int fd; //下载文件的描述符
extern int mfnfd;//信息存储文件描述符
extern char mfn[FILENAMELEN];
extern long totalDownload;

#endif