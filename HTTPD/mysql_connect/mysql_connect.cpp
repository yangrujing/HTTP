#include"mysql_connect.h"
 
sql_connect::sql_connect(const char* host,\
			const char* user,\
			const char* passwd,\
			const char* db)
:_host(host),_user(user),_passwd(passwd),_db(db)
{
	_mysql_base = mysql_init(NULL);
	_res = NULL;
}

sql_connect::~sql_connect()
{
	close_mysql();
}

bool sql_connect::connect_mysql()
{
//	MYSQL *mysql_real_connect(MYSQL *mysql, const char *host, const char *user, const char *passwd, const char *db, unsigned int port, const char *unix_socket, unsigned int client_flag)
	if(!mysql_real_connect(_mysql_base,_host,_user,_passwd,_db,3306,NULL,0))
	{
		cout<<"connect database failed"<<endl;
		return false;
	}
	cout<<"connect database succes"<<endl;
	return true;
}

bool sql_connect::insert_mysql(const string &data)
{
	//eg:insert into Man_Gods (name,sex,birthday,height) values ('Ji Chang Wook','man','1987.7.5','182')
	string table = "insert into Man_Gods (name,sex,birthday,height) values ";
	table += "(";
	table += data;
	table += ")";

//int STDCALL mysql_query(MYSQL *mysql, const char *q);
	if(mysql_query(_mysql_base,table.c_str()) == 0)
	{
		cout<<"insert success"<<endl;
		return true;
	}
	cout<<"insert failed"<<endl;
	return false;
}

bool sql_connect::select_mysql()
{
	string sql_cmd = "select * from Man_Gods";
	if(mysql_query(_mysql_base,sql_cmd.c_str()) != 0)
	{
		cout<<"select failed"<<endl;
		return false;
	}
	// MYSQL_RES * STDCALL mysql_store_result(MYSQL *mysql);
	_res = mysql_store_result(_mysql_base);
	if(_res)
	{
		//my_ulonglong STDCALL mysql_num_rows(MYSQL_RES *res);
		//unsigned int STDCALL mysql_num_fields(MYSQL_RES *res);
		int lines = mysql_num_rows(_res); //get line num
		int columns = mysql_num_fields(_res); //get columns num
		cout<<"lines: "<<lines<<endl;
		cout<<"columns: "<<columns<<endl;

		//get columns name
		int i = 0;
		char *col_name[columns];
		MYSQL_FIELD *fd = NULL;
		//MYSQL_FIELD * STDCALL mysql_fetch_field(MYSQL_RES *res);
		for(;fd = mysql_fetch_field(_res); )
		{
			col_name[i++] = fd->name;
		}

		for(int i = 0;i < columns;i++)
		{
			cout<<col_name[i]<<"\t";
		}
		cout<<endl;

		//show database 
		for(int i = 1;i < lines; i++)
		{
			//MYSQL_ROW   STDCALL mysql_fetch_row(MYSQL_RES *result);
			//return val is char**
			MYSQL_ROW data = mysql_fetch_row(_res);
			for(int j = 0;j < columns;j++)
			{
				cout<<data[j]<<"\t";
			}
			cout<<endl;
		}
		free(_res);
		return true;
	}
	return false;
}

bool sql_connect::updata_mysql()
{
	string com ="update Man_Gods set name = 'Park Have Jin' where name = 'Park+Have+Jin'";
	if(mysql_query(_mysql_base,com.c_str()) == 0)
	{//updata success
		cout<<"updata success,command: "<<com<<endl;
		return true;
	}
	else
	{//updata failed
		cout<<"Updata failed: "<<mysql_errno(_mysql_base)<<" "<<mysql_error(_mysql_base);
		return false;
	}
}

bool sql_connect::delete_mysql()
{
	string com ="delete from Man_Gods where name='Nick+Wang'";
	if(mysql_query(_mysql_base,com.c_str()) == 0)
	{//delete success
		cout<<"Delete success,command: "<<com<<endl;
		return true;
	}
	else
	{//delete failed
		cout<<"Delete failed: "<<mysql_errno(_mysql_base)<<" "<<mysql_error(_mysql_base);
		return false;
	}
	
}

bool sql_connect::creat_table()
{
	string com ="create database students";
	if(mysql_query(_mysql_base,com.c_str()) == 0)
	{// success
		cout<<"Create database success "<<com<<endl;
		return true;
	}
	else
	{//failed
		cout<<"Create database failed "<<mysql_errno(_mysql_base)<<" "<<mysql_error(_mysql_base);
		return false;
	}
}

bool sql_connect::delete_table()
{
	string com ="drop database Students";
	if(mysql_query(_mysql_base,com.c_str()) == 0)
	{//delete success
		cout<<"Delete database success,command: "<<com<<endl;
		return true;
	}
	else
	{//delete failed
		cout<<"Delete database failed: "<<mysql_errno(_mysql_base)<<" "<<mysql_error(_mysql_base);
		return false;
	}

}

bool sql_connect::close_mysql()
{
	mysql_close(_mysql_base);
}

#ifdef _DEBUG_
int main()
{
	string data = "'Ji Chang Wook','man','1987.7.5','182'";
	sql_connect _sql;
	_sql.connect_mysql();
	_sql.insert_mysql(data);
	cout<<"Mysql base info: "<<mysql_get_client_info()<<endl;
	return 0;
}
#endif
