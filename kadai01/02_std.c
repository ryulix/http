#include "watalab.h"

#define BUFSIZE 1024

int main(int argc, char** argv)
{
    int ret;
    char buf[BUFSIZE];

    ret = read(0,buf,BUFSIZE);
    printf("read %d bytes\r\n". ret);

    ret = write(1,buf,ret);

    return 0;
}