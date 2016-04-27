#include<iostream>
#include<stdlib.h>
#include<string.h>
#include<assert.h>
#include<unistd.h>
#include<string>
#include"mysql_connect.h"
using namespace std;

#define _BLOCK_SIZE_ 1024

void register_sql(char* data)
{
	assert(data);
//eg:name=Nick Wang&sex=man&birthday=1982.8.18&height=182
	int i = 0;
	char *arr[4] = {NULL};
	char buf[_BLOCK_SIZE_];
	memset(buf,'\0',sizeof(buf));
	strncpy(buf,data,strlen(data));

	char *cur= buf;
	//Get value
	while(*cur != '\0')
	{
		if(*cur == '=')
		{
			if((arr[i] == NULL) && (i < 4))
			{
				arr[i++] = cur + 1;
			}
		}
		if(*cur == '&' && *cur != '\0')
		{
			*cur = '\0';
		}
		cur++;
	}

	string _info;
	for(i = 0;i < 4;i++)
	{
		_info += "'";
		_info += arr[i];
		_info += "'";
		if(i < 3)
			_info += ",";
	}
	
	sql_connect _sql;
	_sql.connect_mysql();
	//_sql.insert_mysql(_info);
//	_sql.select_mysql();
//	_sql.updata_mysql();
//	_sql.delete_mysql();
	_sql.creat_table();
}


int main()
{
	char *method;
	char *query_string;
	char *content_length;
	char buf[_BLOCK_SIZE_];
	memset(buf,'\0',sizeof(buf));

	method = getenv("REQUEST_METHOD");
	if(method == NULL)
	{
		cout<<"getenv REQUEST_METHOD failed"<<endl;
		exit(1);
	}
//	cout<<"getenv REQUEST_METHOD success"<<endl;
	if(strcasecmp(method,"GET") == 0)
	{
		query_string = getenv("QUERY_STRING");
		if(query_string == NULL)
		{
			cout<<"getenv QUERY_STRING failed"<<endl;
			exit(1);
		}
		register_sql(query_string);
	}
    else if( strcasecmp(method,"POST") == 0)
	{
		content_length = getenv("CONTENT_LENGTH");
		if(content_length == NULL)
		{
			cout<<"getenv CONTENT_LENGTH failed"<<endl;
			exit(1);
		}
		int len = atoi(content_length);
	
		int i = 0;
		for(; i < len;i++)
		{
			read(0,&buf[i],1);
		}
		buf[i] = '\0';
		register_sql(buf);
	}
	else
	  exit(1);

	return 0;
}
