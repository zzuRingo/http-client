#include "glovari.h"

struct respHeader resp;//全局
long begin[THREADAMOUNT+1] = {0};
long init[THREADAMOUNT+1] = {0};
long chunklen;

char realip[16];
char realdomain[DOMLEN];
char realurl[URLLEN];
int realport;
int fd; //下载文件的描述符
int mfnfd;//信息存储文件描述符
char mfn[FILENAMELEN];
long totalDownload;