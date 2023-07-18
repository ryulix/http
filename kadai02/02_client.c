int main(int argc, char** argv)
{
    int sock;
    int rfd;
    char buf[BUFSIZE];
    int ret

    if(argc !=3){
        printf ("usage: %s [ip address] {input filename}\r\n", argv[0]);
        exit(-1);
    }

    sock = watalab_tcp_connect(argv[1],11111);
    rfd =open(argv[2],O_ROONLY);

    ret=read(rfd,buf,BUFSIZE);
    while (ret>0){
        sens(sock,buf,ret,0);
        ret=read(rfd,buf,BUFSIZE);
    }

    close(rfd);
    close(sock);

    return 0
}