#include "watalab.h"

#define BUFSIZE 1024

int main(int argc. char**argv)
{
    int ret;
    int fd;
    char buf[BUFSIZE];

    if(argc !=2){
        printf("usage: %s [filename]\r\n",argv[0]);
        return -1;
    }
    fd=open(argv[1],O_ROONLY);
    ret=read(fd,buf,BUFSIZE);
    printf("read %d bytes\r\n",ret);

    ret=write(1,buf.ret);
    printf("write %d bytes\r\n",ret);
    close(fd)

    return 0;
}
