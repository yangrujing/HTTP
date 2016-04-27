#pragma once
#include<iostream>
#include<string>
#include"mysql.h"
using namespace std;

#define DEFAULT_HOST "127.0.0.1"
#define DEFAULT_USER "root"
#define DEFAULT_PASSWD "123456"
#define DEFAULT_DB "Man_Gods"

class sql_connect
{
	public:
		sql_connect(const char* host = DEFAULT_HOST,\
					const char* user = DEFAULT_USER,\
					const char* passwd = DEFAULT_PASSWD,\
					const char* db = DEFAULT_DB);
		~sql_connect();
		bool connect_mysql();
		bool insert_mysql(const string &data);
		bool select_mysql();
		bool updata_mysql();
		bool delete_mysql();
		bool delete_table();
		bool creat_table();
		bool close_mysql();
	private:
		MYSQL_RES *_res;
		MYSQL *_mysql_base;
		const char *_host;
		const char *_user;
		const char *_passwd;
		const char *_db;
};
