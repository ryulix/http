int watalab_http_session(int sock)
{
    char bu[BUFSIZE];
    int recv_size = 0;
    watalab_info_type info;
    int ret = 0;

    while(ret == 0){
        int size = recv(sock,buf + recv_size,BUFSIZE,0);

        if(size<=0){
            return -1;
        }

        recv_size += size;
        buf[recv_size] = 0;
        ret = watalab_is_header_complete(buf,recv_size,&info);
    }

    watalab_check_file(&info);
    watalab_reply_http(sock,&info);

    return 0;
}

#include "watalab.h"
#define BUFSIZE 1024

typedef struct
{
char cmd[64];
char path[256];
char real_path[256];
char type[64];
int code;
int size;
}watalab_info_type;

int watalab_is_header_complete(char* buf, int size, watalab_info_type* info)
{
char line[1024];
int i, j;
enum state_type
{
PARSE_READ_REQUEST,
PARSE_END
}state;
state = PARSE_READ_REQUEST;
j = 0;
for(i = 0; i < size; i++){
switch(state){
case PARSE_READ_REQUEST:
if(buf[i] == '\r'){
line[j] = '\0';
j = 0;
watalab_parse_startline(line, info);
state = PARSE_END;
}else{
line[j] = buf[i];
j++;
}
break;
case PARSE_END:
break;
}
}
if(state == PARSE_END){
return 1;
}else{
return 0;
}
}

void watalab_parse_startline(char* line, watalab_info_type *pinfo)
{
char cmd[1024];
char path[1024];
int i, j;
enum state_type
{
SEARCH_CMD,
SEARCH_PATH,
SEARCH_END
}state;
state = SEARCH_CMD;
j = 0;
for(i = 0; i < (int) strlen(line); i++){
    switch(state){
case SEARCH_CMD:
if(line[i] == '␣'){
cmd[j] = '\0';
j = 0;
state = SEARCH_PATH;
}else{
cmd[j] = line[i];
j++;
}
break;
case SEARCH_PATH:
if(line[i] == '␣'){
path[j] = '\0';
j = 0;
state = SEARCH_END;
}else{
path[j] = line[i];
j++;
}
break;
case SEARCH_END:
break;
}
}
strcpy(pinfo->cmd, cmd);
strcpy(pinfo->path, path);
}

void watalab_check_file(watalab_info_type *info)
{
struct stat s;
int ret;
char* pext;
sprintf(info->real_path, "html%s", info->path);
ret = stat(info->real_path, &s);
if(S_ISDIR(s.st_mode) == true){
sprintf(info->real_path, "%s/index.html", info->real_path);
sprintf(info->path, "%s/index.html", info->path);
}
ret = stat(info->real_path, &s);
if(ret == -1){
info->code = 404;
}else{
info->code = 200;
info->size = (int) s.st_size;
}
pext = strstr(info->path, ".");
if(pext != NULL && strcmp(pext, ".html") == 0){
strcpy(info->type, "text/html");
}else if(pext != NULL && strcmp(pext, ".jpg") == 0){
strcpy(info->type, "image/jpeg");
}
}


void watalab_reply_http(int sock, watalab_info_type *info)
{
char buf[16384];
int len;
int ret;
if(info->code == 404){
watalab_send_404(sock);
printf("404␣not␣found␣%s\n", info->path);
return;
}
len = sprintf(buf, "HTTP/1.0␣200␣OK\r\n");
len += sprintf(buf + len, "Content-Length:␣%d\r\n", info->size);
len += sprintf(buf + len, "Content-Type:␣%s\r\n", info->type);
len += sprintf(buf + len, "\r\n");
ret = send(sock, buf, len, 0);
if(ret < 0){
shutdown(sock, SHUT_RDWR);
close(sock);
return;
}
watalab_send_file(sock, info->real_path);
}


void watalab_send_404(int sock)
{
char buf[16384];
int ret;
sprintf(buf, "HTTP/1.0␣404␣Not␣Found\r\n\r\n");
printf("%s", buf);
ret = send(sock, buf, strlen(buf), 0);
if(ret < 0){
shutdown(sock, SHUT_RDWR);
close(sock);
}
}



void watalab_send_file(int sock, char* filename)
{
    int fd;
int len;
char buf[16384];
fd = open(filename, O_RDONLY);
if(fd == -1){
shutdown(sock, SHUT_RDWR);
close(sock);
return;
}
len = read(fd, buf, BUFSIZE);
while(len > 0){
int ret = send(sock, buf, len, 0);
if(ret < 0){
shutdown(sock, SHUT_RDWR);
close(sock);
break;
}
len = read(fd, buf, BUFSIZE);
}
close(fd);
}