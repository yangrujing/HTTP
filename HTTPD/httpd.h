#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/stat.h>
#include<sys/sendfile.h>
#include<sys/wait.h>
#include<unistd.h>
#include<string.h>
#include<strings.h>
#include<errno.h>
#include<netinet/in.h>
#include<stdlib.h>
#include<pthread.h>
#include<ctype.h>
#include<assert.h>
#include<fcntl.h>

#define _BACK_LOG_ 5
#define _BLOCK_SIZE_ 2048
#define MAIN_PAGE "index.html"
#define HTTP_VERSION "HTTP/1.1"

