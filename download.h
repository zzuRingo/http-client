#ifndef _DOWNLOAD_H
#define _DOWNLOAD_H

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
#include "socket.h"
#include "basicfunction.h"

void test_func();

void *countProcess(void * threadnum);

void setDowloadRequr(char *header, int threadnum);

void *download(void * threadnum);

void multiThreadDownload();


#endif