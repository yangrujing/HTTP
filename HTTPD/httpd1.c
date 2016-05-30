#include"httpd.h"

static void usage(const char *arg)
{
	printf("usage: %s [port]\n",arg);
}

static void bad_request(int client)
{
	char buf[1024];
	memset(buf,'\0',sizeof(buf) - 1);
	sprintf(buf, "HTTP/1.1 400 Bad Request\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "Content-type: text/html\r\n");
    send(client, buf, strlen(buf), 0);
	sprintf(buf, "\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "<html></br><p>Unable to resolve this request</p></br></html>\r\n");
	send(client, buf, strlen(buf), 0);
}

static void forbidden_access(int client)
{
	char buf[1024];
	memset(buf,'\0',sizeof(buf) - 1);
	sprintf(buf, "HTTP/1.1 403 Forbidden\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "Content-type: text/html\r\n");
    send(client, buf, strlen(buf), 0);
	sprintf(buf, "\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "<html></br><p>Server refused to provide services</p></br></html>\r\n");
	send(client, buf, strlen(buf), 0);
}

static void not_found(int client)
{
	char buf[1024];
	memset(buf,'\0',sizeof(buf) - 1);
	sprintf(buf, "HTTP/1.1 404 Not Found\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "Content-type: text/html\r\n");
    send(client, buf, strlen(buf), 0);
	sprintf(buf, "\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "<html></br><p>Request resource dose not exist</p></br></html>\r\n");
	send(client, buf, strlen(buf), 0);
}

static void file_delete(int client)
{
	char buf[1024];
	memset(buf,'\0',sizeof(buf) - 1);
	sprintf(buf, "HTTP/1.1 410 File Deleted\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "Content-type: text/html\r\n");
    send(client, buf, strlen(buf), 0);
	sprintf(buf, "\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "<html></br><p>Request file deleted</p></br></html>\r\n");
	send(client, buf, strlen(buf), 0);
}

static void url_too_long(int client)
{
	char buf[1024];
	memset(buf,'\0',sizeof(buf) - 1);
	sprintf(buf, "HTTP/1.1 414 Url Too Long\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "Content-type: text/html\r\n");
    send(client, buf, strlen(buf), 0);
	sprintf(buf, "\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "<html></br><p>Request url too long,the servse cann't accept</p></br></html>\r\n");
	send(client, buf, strlen(buf), 0);
}

static void  server_error(int client)
{
	char buf[1024];
	memset(buf,'\0',sizeof(buf) - 1);
	sprintf(buf, "HTTP/1.1 500 Internal Server Error\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "Content-type: text/html\r\n");
    send(client, buf, strlen(buf), 0);
	sprintf(buf, "\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "<html></br><p>Server cann't be expected error</p></br></html>\r\n");
	send(client, buf, strlen(buf), 0);
}

static void  server_unavailable(int client)
{
	char buf[1024];
	memset(buf,'\0',sizeof(buf) - 1);
	sprintf(buf, "HTTP/1.1 503 Server Unavailable\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "Content-type: text/html\r\n");
    send(client, buf, strlen(buf), 0);
	sprintf(buf, "\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "<html></br><p>The server is currently unable to process the client's request</p></br></html>\r\n");
	send(client, buf, strlen(buf), 0);
}

static void  default_error(int client)
{
	char buf[1024];
	memset(buf,'\0',sizeof(buf) - 1);
	sprintf(buf, "HTTP/1.1 000 Default Error\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "Content-type: text/html\r\n");
    send(client, buf, strlen(buf), 0);
	sprintf(buf, "\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "<html></br><p>Unable to resolve this request</p></br></html>\r\n");
	send(client, buf, strlen(buf), 0);
}

void return_error_client(int client, int error_code)
{
	switch(error_code)
	{
		case 400://request error
			bad_request(client);
			break;
		case 403://forbidden access
			forbidden_access(client);
			break;
		case 404://not found
			not_found(client);
			break;
		case 410:
			file_delete(client);
			break;
		case 414:
			url_too_long(client);
			break;
		case 500://server error
			server_error(client);
	    	break;
		case 503://server unavailable
			server_unavailable(client);
			break;
		default:
			default_error(client);
 			break;	
	}
}

int startup(int port)
{
	int listen_sock = socket(AF_INET,SOCK_STREAM,0);
	if(listen_sock < 0)
	{
		perror("socket");
		exit(1);
	}
    
	int flag = 1;
	setsockopt(listen_sock,SOL_SOCKET,SO_REUSEADDR,&flag,sizeof(flag)); //Avoid port reuse

	struct sockaddr_in local;
	local.sin_family = AF_INET;
	local.sin_port = htons(port);
	local.sin_addr.s_addr = htonl(INADDR_ANY);
	
	if(bind(listen_sock,(struct sockaddr*)&local,sizeof(local)) < 0)
	{
		perror("bind");
		exit(2);
	}

	if(listen(listen_sock,_BACK_LOG_) < 0)
	{
		perror("listen");
		exit(3);
	}

	return listen_sock;
}

static int get_line(int sock,char *buf,size_t size)
{
	if(!buf && size < 0)
	  return -1;

	int i = 0;
	int n = 0;
	char c = '\0';
	while((i < size - 1) && (c != '\n'))
	{
		//ssize_t recv(int sockfd, void *buf, size_t len, int flags);
		n = recv(sock,&c,1,0);
		if(n > 0)
		{
			if(c == '\r')
			{
				//MSG_PEEK This  flag  causes the receive operation to return data from the
				// beginning of the receive queue without removing that  data  from the queue.
				n = recv(sock,&c,1,MSG_PEEK);
				if(( n > 0) && (c == '\n'))
					recv(sock,&c,1,0);
				else
				  c = '\n';
			}
			buf[i] = c;
			i++;
		}
		else
		{
			c = '\n';
		}
	}
	buf[i] = '\0';
	return i;
}

void clear_header(int sock_client)
{
	int n = 0;
	char buf[_BLOCK_SIZE_];
	do
	{
		memset(buf,'\0',sizeof(buf));
		n = get_line(sock_client,buf,sizeof(buf) - 1);
	}while((n > 0) && (strcmp(buf,"\n") != 0));
}

void print_debug(const char* str)
{
#ifdef _DEBUG_
	printf("%s\n",str);
#endif
}

void exe_cgi(int sock_client,const char *path,const char *method,const char *query_string)
{
	print_debug("enter cgi");
	assert(path);
	assert(method);

	int content_length = -1;
	int numchars = -1;
	char buf[_BLOCK_SIZE_];

//	create one-way pipe
//	int cgi_input[2] = {0,0};
//	int cgi_output[2] = {0,0};
//	create two-way pipe
	int pipe[2] = {0,0};

	print_debug(method);
	if(strcasecmp(method, "GET") == 0)
	{
		assert(query_string);
	    clear_header(sock_client);
	}
	else
	{//POST 
		do
		{//get content_length
			memset(buf,'\0',sizeof(buf));
			numchars = get_line(sock_client,buf,sizeof(buf));
			if(strncmp(buf,"Content-Length:",strlen("Content-Length:")) == 0)
			{
				content_length = atoi(&buf[16]);
			}
		}while((numchars > 0) && (strcmp(buf,"\n") != 0));
		print_debug("get content_length");
		if(content_length < 0)
		{
		  return_error_client(sock_client,400);//bad request
		  print_debug("get content_length failed");
		  return;
		} 
	}
	
	//set stat line
	memset(buf,'\0',sizeof(buf));
	strcpy(buf,HTTP_VERSION);
	strcat(buf," 200 OK\r\n\r\n");
	send(sock_client,buf,strlen(buf),0);

//	if(pipe(cgi_input) < 0)
//	{
//		return_error_client(sock_client,500);//server error
//		perror("pipe");
//		return;
//	}
//	if(pipe(cgi_output) < 0)
//	{
//		return_error_client(sock_client,500);//server error
//		perror("pipe");
//		close(cgi_input[0]);
//		close(cgi_input[1]);
//		return;
//	}
//	print_debug("create pipe success");

	int ret = socketpair(PF_LOCAL,SOCK_STREAM,0,pipe);
	if(ret < 0)
	{
		return_error_client(sock_client,500);//server error
		perror("socketpair");
		return;
	}

	pid_t id = fork();
	if(id < 0)
	{
		perror("fork");	
		return_error_client(sock_client,500);//server error
//		close(cgi_input[0]);
//		close(cgi_input[1]);
//		close(cgi_output[0]);
//		close(cgi_output[1]);
		close(pipe[0]);
		close(pipe[1]);
		return;
	}
	else if(id == 0)
	{//child
		char query_env[_BLOCK_SIZE_];
		char method_env[_BLOCK_SIZE_ / 2];
		char content_len_env[_BLOCK_SIZE_];
		memset(query_env,'\0',sizeof(query_env));
		memset(method_env,'\0',sizeof(method_env));
		memset(content_len_env,'\0',sizeof(content_len_env));

	//	close(cgi_input[1]);
	//	close(cgi_output[0]);
		close(pipe[1]);
		
//		dup2(cgi_input[0],0);
//		dup2(cgi_output[1],1);
		dup2(pipe[0],0);
		dup2(pipe[0],1);

		sprintf(method_env,"REQUEST_METHOD=%s",method);
		if(putenv(method_env) != 0)
		{
			return_error_client(sock_client,500);//server error
			print_debug("putenv REQUEST_METHOD failed");
			return;
		}
		if(strcasecmp(method,"GET") == 0)
		{//GET
			sprintf(query_env,"QUERY_STRING=%s",query_string);
			if(putenv(query_env) != 0)
			{
				return_error_client(sock_client,500);//server error
				print_debug("putenv QUERY_STRING failed");
				return;
			}	
		}
		else
		{//POST
			sprintf(content_len_env,"CONTENT_LENGTH=%d",content_length);
			if(putenv(content_len_env) != 0)
			{
				return_error_client(sock_client,500);//server error
				print_debug("putenv CONTENT_LENGTH failed");
				return;
			}
		}
		execl(path,path,NULL);
		exit(1);	
	}
	else
	{//father
	//	close(cgi_input[0]);
	//	close(cgi_output[1]);
		close(pipe[0]);

		int i = 0;
		char c = '\0';
		if(strcasecmp(method,"POST") == 0)
		{
			for(;i < content_length;i++)
			{
				recv(sock_client,&c,1,0);
				printf("%c",c);
			//	write(cgi_input[1],&c,1);
				write(pipe[1],&c,1);
			}
			printf("\n");
			print_debug("write to cgi_input succes");
		}
		while(read(pipe[1],&c,1) > 0)
		{
			printf("%c",c);
			send(sock_client,&c,1,0);
		}
		printf("\n");
		print_debug("send to client success");

	//	close(cgi_input[1]);
	//	close(cgi_output[0]);
		close(pipe[1]);

		waitpid(id,NULL,0);
	}	
}

void echo_html(int sock_client,const char *path,size_t file_size)
{
	assert(path);
	assert(file_size > 0);
	int fd = open(path,O_RDONLY);
	if(fd < 0)
	{
		return_error_client(sock_client,404);//not found
		print_debug("open file failed");
		return;
	}

	char state_line[_BLOCK_SIZE_ / 2];
	memset(state_line,'\0',sizeof(state_line));
	strncpy(state_line,HTTP_VERSION,strlen(HTTP_VERSION) + 1);
	strcat(state_line,"200 OK");
	strcat(state_line,"\r\n\r\n");
	if(send(sock_client,state_line,strlen(state_line),0) < 0)
	{
		return_error_client(sock_client,503);//server unavailable
	    perror("send");
	    return;
	}
	
	//ssize_t sendfile(int out_fd, int in_fd, off_t *offset, size_t count);
	if(sendfile(sock_client,fd,NULL,file_size) < 0)
	{
		return_error_client(sock_client,503);//server unavailable
		perror("sendfile");
		close(fd);
		return;
	}
	close(fd);
}

static void* accept_request(int new_client)
{
	char buf[_BLOCK_SIZE_];
	char method[_BLOCK_SIZE_ / 10];
	char url[_BLOCK_SIZE_];
	char path[_BLOCK_SIZE_];
	memset(buf,'\0',sizeof(buf));
	memset(method,'\0',sizeof(method));
	memset(url,'\0',sizeof(url));
	memset(path,'\0',sizeof(path));
//#ifdef _DEBUG_
//	printf("debug...\n");
//	while(get_line(new_client,buf,sizeof(buf)) > 0)
//	{
//	  printf("%s");
//	}
//#endif
	int cgi = 0;
	char *query_string = NULL;
	
	int numchars = get_line(new_client,buf,sizeof(buf));
	if(numchars < 0)
	{
		return_error_client(new_client,400);
		close(new_client);
		return(void *) -1;
	}
	print_debug("buf");

	int i = 0; //index -> method
	int	j = 0; //index -> buf

	//Get method
	while(!isspace(buf[j]) && (j < numchars) && (i < sizeof(method) - 1))
	{
		method[i] = buf[j];
		i++;
		j++;
	}
	if(isspace(buf[j]))
	  method[i] = '\0';

	print_debug("get method success");
	
	if(strcasecmp(method,"GET") && strcasecmp(method,"POST"))
	{
		return_error_client(new_client,400);
		close(new_client);
		return (void *) -1;
	}

	i = 0; //index -> url
	//skip space
	while(isspace(buf[j]) && (j < numchars))
		j++;
	//Get url
	while(!isspace(buf[j]) && (i < sizeof(url) - 1) && (j < numchars))
	{
		url[i] = buf[j];
		i++;
		j++;
	}
	url[i] = '\0';
	print_debug("url:");
	print_debug(url);

	if(strcasecmp(method,"POST") == 0)
	{
	  cgi = 1;
	  print_debug("POST method");
	}
	if(strcasecmp(method,"GET") == 0)
	{
		print_debug("GET method");
		//get parameter eg:https://www.so.com/s?ie=utf-8&src=hao_search&shb=1&hsid=b6f46cd7d77cf4ba&q=百度
		query_string = url;
		while(*query_string != '?' && *query_string != '\0')
		  query_string++;
		if(*query_string == '?')
		{
			*query_string = '\0';
			query_string++;
			cgi = 1;
			print_debug(query_string);
		}
	}
	sprintf(path,"htdocs%s",url);
	if(path[strlen(path) - 1] == '/')
	{//root directory
		  strcat(path,MAIN_PAGE);
		  print_debug(path);
	}

	struct stat st; 
	// int stat(const char *path, struct stat *buf); get file property
	if(stat(path,&st) < 0) //failed file not exist
	{
		return_error_client(new_client,404);//not found
		print_debug(path);
		perror("stat");
		close(new_client);
		return(void *)-1;
	}
	else
	{
		if(S_ISDIR(st.st_mode)) 
		{
			strcat(path,"/");
			strcat(path,MAIN_PAGE);
			print_debug(path);
		}
		else if((st.st_mode & S_IXUSR) || (st.st_mode & S_IXGRP) || (st.st_mode & S_IXOTH))
		{
			cgi = 1;
			print_debug("cgi = 1");
		}
		else
		  //do nothing
			print_debug("do nothing");
		if(cgi == 1)
		{
			print_debug("exec cgi method");
			exe_cgi(new_client,path,method,query_string);
		}
		else
		{
			print_debug("exec echo_html");
			clear_header(new_client);
			echo_html(new_client,path,st.st_size);
		}
	}
	close(new_client);
	return NULL;
}

void set_nonblock(int fd)
{
	int fl = fcntl(fd,F_GETFL);//Read the file status flags 
	if(fl < 0)
	{
		perror("fcntl");
		return;
	}
	fcntl(fd,fl | O_NONBLOCK);
	return;
}

int main(int argc,char *argv[])
{
	if(argc != 2)
	{
		usage(argv[0]);
		exit(1);
	}
	
	int port = atoi(argv[1]);
	struct sockaddr_in client;
	socklen_t len  = sizeof(client);

	int sock = startup(port);

	//int epoll_create(int size);
	int epoll_fd = epoll_create(256);
	if(epoll_fd < 0)
	{
		perror("epoll_create");
		exit(1);
	}

	struct epoll_event _ev;
	_ev.events = EPOLLIN;
	_ev.data.fd = sock;

	 // int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
	 // add sock
	 if(epoll_ctl(epoll_fd,EPOLL_CTL_ADD,sock,&_ev) < 0)
	 {
		 perror("epoll_ctl");
		 close(sock);
		 exit(1);
	 }

	 int timeout = 10000;//10 second
	 char buf[_BLOCK_SIZE_ * 5];
	 memset(buf,'\0',sizeof(buf));

	 struct epoll_event _ev_out[_MAX_NUM_];
	 int ready_num = -1;
	
	while(1)
	{
		switch(ready_num = epoll_wait(epoll_fd,_ev_out,_MAX_NUM_,timeout))
		{
			case 0: //timeout
				printf("server timeout...\n");
				break;
			case -1: //error
				perror("epoll_wait");
				break;
			default:
				{
					int i = 0;
					for(; i < ready_num;i++)
					{
						int fd = _ev_out[i].data.fd;
						if((fd == sock) && (_ev_out[i].events & EPOLLIN))
						{
							int new_client = accept(fd,(struct sockaddr*)&client,&len);
							if(new_client < 0)
							{
								return_error_client(new_client,503);//server unavailable
								perror("accept");
								continue;
							}
							print_debug("Get a connect...");
							set_nonblock(new_client);
							_ev.events = EPOLLIN;
							_ev.data.fd = new_client;

							if(epoll_ctl(epoll_fd,EPOLL_CTL_ADD,new_client,&_ev) < 0)
							{
								perror("epoll_ctl");
								close(new_client);
								continue;
							}
							print_debug("Insert new_client success");
							continue;
						}

						if(_ev_out[i].events & EPOLLIN)
						{
							accept_request(fd);	
						}
					}
				}
			break;
		}
//		pthread_t tid;
//		pthread_create(&tid,NULL,accept_request,(void*)new_client);
	}
	close(sock);
	close(epoll_fd);
	return 0;
}

