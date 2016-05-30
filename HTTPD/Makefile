ROOT_PATH = $(shell pwd)

SERVER_BIN = httpd
SERVER_SRC = httpd1.c
CGI_PATH = $(ROOT_PATH)/cgi_bin

CC = gcc
FLAGS = -D_DEBUG_ 
#LDFLAGS = -lpthread
LDFLAGS = 

.PHONY:all
all:$(SERVER_BIN) $(CLIENT_BIN) cgi

$(SERVER_BIN):$(SERVER_SRC)
	$(CC) -o $@ $^ $(FLAGS) $(LDFLAGS)

.PHONY:clean
clean:
	rm -rf	$(SERVER_BIN)  output
	for name in `echo $(CGI_PATH)`;\
		do\
		cd $$name;\
		make clean;\
		cd -;\
		done

.PHONY:cgi
cgi:
	for name in `echo $(CGI_PATH)`;\
		do\
		cd $$name;\
		make;\
		cd -;\
		done

.PHONY:output
output:all
	mkdir output
	cp httpd output
	cp start.sh output
	cp -rf htdocs output
	mkdir -p output/htdocs/cgi-bin
	for name in `echo $(CGI_PATH)`;\
		do\
		cd $$name;\
		make output;\
		cd -;\
		done
	cp -rf mysql-lib output
