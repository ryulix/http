#include "watalab.h"

#define BUFSIZE 1024

int main(int argc, char**argv)
{
    int retint rfd, wfd;
    char buf[BUFSIZE];

    if(argc !=3){
        printf("usage:%S[from filename] [to filename]\r\n",argv[0]);
        return -1;
    }

    rfd=open(argv[1],O_ROONLY);
    wfd=open(argv[2],O_WRONLY | O_CREAT | O_TRUNK,S_IRWXU);

    ret= read(rfd,buf,BUFSIZE);
    while(ret>0){
        printf("read %d bytes\r\n",ret);
        write(wfd,buf,ret);
        ret=read(rfd,buf,BUFSIZE);

    }
    close(wfd);
    close(rfd);

    return 0;
}