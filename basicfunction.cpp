#include "basicfunction.h"

//获得url
void getURL(char argc, char const*argv[], char *url){
    if(argc == 1){
        printf("Input a valid URL please\n");
        exit(0);
    }
    else
        strcpy(url, argv[1]);
}

//解析域名 
void paresURL(const char *url, char *domain, int *port, char *fileName){
    int j = 0;
    int start = 0;
    *port = 80;
    char *patterns[] = {"http://", "https://", NULL};

    //确定从哪个地方开始解析
    for (int i = 0; patterns[i]; i++)
        if (strncmp(url, patterns[i], strlen(patterns[i])) == 0)
                start = strlen(patterns[i]);

    //解析域名，如果有端口号会保留端口号
    for(int i = start; url[i] != '/' && url[i] != '\0';i++, j++)
        domain[j] = url[i];
    domain[j] = '\0';

    //解析端口号，如果没有就设为80
    char *pos = strstr(domain, ":");
    if(pos){
        sscanf(pos, "%d", port);
        //删除域名里的端口号
        for (int i = 0; i < (int)strlen(domain); i++){
            if (domain[i] == ':'){
                domain[i] = '\0';
                break;
            }
        }
    }

    //获取下载文件名
    j = 0;
    for (int i = start; url[i] != '\0'; i++){
        if (url[i] == '/'){
            if (i != strlen(url) - 1)
                j = 0;
            continue;
        }
        else
            fileName[j++] = url[i];
    }
    fileName[j] = '\0';
}

//  通过域名得到相应的ip地址
void getIP(char *domain, char *ipAddr){
  
    struct hostent *host = gethostbyname(domain);
    if (!host){
        ipAddr = NULL;
        return;
    }

    for (int i = 0; host->h_addr_list[i]; i++){
        strcpy(ipAddr, inet_ntoa(*(struct in_addr*) host->h_addr_list[i]));
        break;
    }

    if (strlen(ipAddr) == 0){
        printf("can not get ip address\n");
        exit(0);
    }
}

//拼接要求
void setRequrement(char *header, char *domain, char *url){

    sprintf(header, \
            "GET %s HTTP/1.1\r\n"\
            "Host:%s\r\n"\
            "Connection:close\r\n"\
            "\r\n"\
        ,url, domain);
    printf("header:%s\n",header);
}

int creatFile(){
    long fileSize = resp.content_length;
    int fd = open(resp.file_name, O_CREAT | O_WRONLY, S_IRWXG | S_IRWXO | S_IRWXU);
    lseek(fd, fileSize - 1, SEEK_END);
    char ch = 'a';
    write(fd, &ch, sizeof(char));
    close(fd);
    return fd;
}

struct respHeader getRespHeader(char *response){
    /*获取响应头的信息*/
    struct respHeader resp;

    char *pos = strstr(response, "HTTP/");
    if (pos)
        sscanf(pos, "%*s %d", &resp.status_code);//返回状态码

    pos = strstr(response, "Content-Type:");//返回内容类型
    if (pos)
        sscanf(pos, "%*s %s", resp.content_type);

    pos = strstr(response, "Content-Length:");//内容的长度(字节)
    if (pos)
        sscanf(pos, "%*s %ld", &resp.content_length);
    return resp;
}

void getResp(int sockfd, char *header, const char *fileName, int threadOrNot){
    write(sockfd, header, strlen(header));    
    int memSize = BUFFSIZE;
    int length = 0;
    int len;
    char *buf  = (char *) malloc(BUFFSIZE * sizeof(char));
    char *response = (char *) malloc(BUFFSIZE * sizeof(char));

    while ((len = read(sockfd, buf, 1)) != 0){
        if (length + len > memSize){
            //无法确定响应头内容长度
            memSize *= 2;
            char * temp = (char *) realloc(response, sizeof(char) * memSize);
            if (temp == NULL){
                printf("realloc failed\n");
                exit(-1);
            }
            response = temp;
        }

        buf[len] = '\0';
        strcat(response, buf);

        //找到响应头的头部信息, 两个"\n\r"为分割点
        int flag = 0;
        for (int i = strlen(response) - 1; response[i] == '\n' || response[i] == '\r'; i--, flag++);
        if (flag == 4)
            break;
        length += len;
    }

    printf("\n>>>>Response header:<<<<\n%s", response);
    if(threadOrNot == 0){
        resp = getRespHeader(response);
    }
   
    if (resp.status_code == 404) {
        printf("error: %d 404 Not Found\n",resp.status_code);
        exit(0);
    }
    else if(resp.status_code == 400){
    	printf("error: %d 400 Bad Request\n",resp.status_code);
        exit(0);
    }
    else if(resp.status_code == 304){
    	printf("error: %d 304 Not Modified\n",resp.status_code);
        exit(0);
    }

    strcpy(resp.file_name, fileName);
}

char* creatMasgetext(const char*fileName){
    static char masgeFileName[FILENAMELEN];
    memset(masgeFileName, 0, FILENAMELEN);
    sprintf(masgeFileName, "%s%s",fileName, fileExtention);
    sprintf(mfn, "%s",masgeFileName);
  //  printf("name : %s",masgeFileName);
    return masgeFileName;
}
int resumeOrNot(const char *fileName){
    char *masgeFileName = creatMasgetext(fileName);
    int massagefd = open(masgeFileName, O_WRONLY, S_IRWXG | S_IRWXO | S_IRWXU);
    if(massagefd == -1){
        //close(massagefd);
        return -1;
    }
    else{
        close(massagefd);
        printf("\n------continue...--------\n\n");
        return massagefd;
    }
}

void cacuBegin(const char *masgeFileName){    //建立文本文件        
    chunklen = resp.content_length / THREADAMOUNT;
    long total = 0;
    FILE *fp = fopen(masgeFileName,"w+t");
    int i;
    for(i = 0; i<THREADAMOUNT; i++){
        begin[i] = total;
        total += chunklen;
    }
    begin[i] = resp.content_length + 1;
   // printf("thread %d : %ld\n",i,begin[i]);
    fwrite(begin, sizeof(long), THREADAMOUNT+1, fp);
    fclose(fp);
}

void prepareDownload(int sockfd, char* header, const char *fileName){
    getResp(sockfd, header, fileName, 0);
    int massagefd = resumeOrNot(fileName);
    char *masgeFileName = creatMasgetext(fileName);
    if(massagefd == -1){
        cacuBegin(masgeFileName);
        creatFile(); 
    }
    else{
        FILE *massagefp = fopen(masgeFileName,"r+t");  
        fread(begin, sizeof(long), THREADAMOUNT+1, massagefp);
    }
    //计算所有起点
    long total = 0;
    int chunklen = resp.content_length / THREADAMOUNT;
    int i;
    for(i = 0; i<3; i++){
        init[i] = total;
        total += chunklen;
    }
    init[i] = resp.content_length;
    close(sockfd);
}

void setConst(const char* ipAddr, const char *domain, const char *url, const int port){
    realport = port;
    sprintf(realip, "%s", ipAddr);
    sprintf(realurl, "%s", url);
    sprintf(realdomain, "%s", domain);
    printf("port : %d\nurl : %s\ndomain : %s\nip%s\n",realport,realurl,realdomain,realip);
}

