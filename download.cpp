#include "download.h"

void test_func()
{
    int cur_size = 0;
    long total_size = resp.content_length;
    printf("total_size:%ld",total_size);
    
    int offset = 0;
    for(int i = 0; i<THREADAMOUNT; i++){
        cur_size += (begin[i] - init[i]);
        pwrite(mfnfd, &begin[i], sizeof(long), offset);
        offset += sizeof(long);
    }
    totalDownload = cur_size;

    float percent = (float) cur_size / total_size;
    const int numTotal = 50;
    int numShow = (int)(numTotal * percent);

    if (numShow == 0)
        numShow = 1;

    if (numShow > numTotal)
        numShow = numTotal;

    char sign[51] = {0};
    memset(sign, '=', numTotal);
    if(percent > 1)
        percent = 1;
    printf("\r%.2f%%\t[%-*.*s] %.2f/%.2fMB\n", percent * 100, numTotal, numShow, sign, cur_size / 1024.0 / 1024.0, total_size / 1024.0 / 1024.0);
    fflush(stdout);

    if (numShow == numTotal){
        printf("\n");
    }
} 



void *countProcess(void * threadnum){
    while(1){
        test_func();
        for(int i = 0; i<THREADAMOUNT; i++)
            printf("thread%d :%ld\n",i,begin[i]);
        if(totalDownload >= resp.content_length)
            break; 
        usleep(500000);
    }
    printf("\n");
    printf("Download successful ^_^\n\n");
}

void setDowloadRequr(char *header, int threadnum){
    sprintf(header, \
            "GET %s HTTP/1.1\r\n"\
            "Host:%s\r\n"\
            "Range: bytes=%ld-%ld\r\n"\
            "Connection:close\r\n"\
            "\r\n"\
        ,realurl, realdomain, begin[threadnum], begin[threadnum+1]-1);
    printf("header:%s\n",header);
}


void *download(void * threadnum)
{
    /*下载文件函数, 放在线程中执行*/
    int length = 0;
    int len;
    int threadNum = *(int *)threadnum;
    char header[REQURELEN] = {0};

    setDowloadRequr(header, threadNum);
    int client_socket = setConnect();
    getResp(client_socket, header, resp.file_name, 1);

    char *buf = (char *) malloc(BUFFSIZE * sizeof(char));
    int fileSize = begin[threadNum+1] - begin[threadNum] - 1;
    int offset = begin[threadNum];
    int rt;
    while ((len = read(client_socket, buf, BUFFSIZE)) != 0 && length < fileSize)
    {
        rt = pwrite(fd, buf, len, offset);
        length += len;
        offset += len;
        begin[threadNum] += (long)len; 
       // progressBar(length, resp.content_length);
    }

}

void multiThreadDownload(){
    fd = open(resp.file_name, O_WRONLY, S_IRWXG | S_IRWXO | S_IRWXU);
    mfnfd = open(mfn, O_WRONLY, S_IRWXG | S_IRWXO | S_IRWXU);
    int n0,n1,n2,n3;
    n0 = 0;
    n1 = 1;
    n2 = 2;
    n3 = 3;
    pthread_t t0,t1,t2,t3;
    pthread_create(&t3, NULL, countProcess, (void *)&n3);
    pthread_create(&t0, NULL, download, (void *)&n0);
    pthread_create(&t1, NULL, download, (void *)&n1);
    pthread_create(&t2, NULL, download, (void *)&n2);
    
    pthread_join(t0, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);
} 