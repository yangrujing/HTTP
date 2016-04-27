#include<iostream>
#include<string.h>
#include<string>
#include<stdlib.h>
#include<assert.h>
using namespace std;

#define _BLOCK_SIZE_ 1024

//eg:my_math(text=32.3%2B37.2%3D)
// %2B->+  %2F->/ %25->% %3D->=
void my_math(char *str)
{
	assert(str);

	char data_arr[_BLOCK_SIZE_];
	memset(data_arr,'\0',sizeof(data_arr));
	strcpy(data_arr,str);
	char symbol_arr[_BLOCK_SIZE_];
	memset(symbol_arr,'\0',sizeof(symbol_arr));
	strcpy(symbol_arr,str);

	char *data = data_arr;
	char *symbol = symbol_arr;
	char *data1 = NULL;
	char *data2 = NULL;

	//get data
	while(*data != '\0')
	{
		if(*data == '=')
		{
			data1 = data + 1;
		}
		if(*data == '%' || *data == '-' || *data == '*')
		{
			if(*data == '%' && data2 == NULL)
			{
				data2 = data + 3;
			}
			else if(*data == '-' || *data == '*')
			{
				data2 = data + 1;				
			}
			*data = '\0';
		}
		data++;
	}

	double res = 0.0;
	double val1 = strtod(data1,NULL);
	double val2 = strtod(data2,NULL);

	//get symbol
	while(*symbol != '\0')
	{
		if(*symbol == '%')
		{
			char *cur = symbol + 1;
			if(*cur == '2')
			{
				cur++;
				if(*cur == 'B')
				{//%2B->+
					res = val1 + val2;
					cout<<val1<<" + "<<val2<<" = "<<res<<endl;
				}
				else if(*cur == 'F')
				{//%2F->/
					res = val1 / val2;
					cout<<val1<<" / "<<val2<<" = "<<res<<endl;
				}
				else
				{//%25->%
					res = (int)val1 % (int)val2;
					cout<<val1<<" % "<<val2<<" = "<<res<<endl;
				}
			}
		}
		if(*symbol == '*')
		{
			res = val1 * val2;
			cout<<val1<<" * "<<val2<<" = "<<res<<endl;
		}
		if(*symbol == '-')
		{
			res = val1 - val2;
			cout<<val1<<" - "<<val2<<" = "<<res<<endl;
		}
		symbol++;
	}
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
		cout<<"get method failed"<<endl;
		exit(1);
	}
	if(strcmp(method,"GET") == 0)
	{//get method
		query_string = getenv("QUERY_STRING");
		if(query_string == NULL)
		{
			cout<<"get query_string failed"<<endl;
			exit(1);
		}
		my_math(query_string);
	}
	else if(strcmp(method,"POST") == 0)
	{//POST method
		content_length = getenv("CONTENT_LENGTH");
		if(content_length == NULL)
		{
			cout<<"get content_length failed"<<endl;
			exit(1);
		}
		int len = atoi(content_length);
		for(int i = 0;i < len;i++)
		{
			read(0,&buf[i],1);
		}
		my_math(buf);
	}
	else
	  exit(1);

	return 0;

}
