#include "factory.h"
int insertVD(const int precode,const char *name,const char *md5,const char*type)
{
	MYSQL *conn;
	char* server="localhost";
	char* user="root";
	char* password="123";
	char* database="Internet";
	char query[200]="insert into VD(precode,name,md5,type) values";
    sprintf(query,"%s%s%d%s%s%s%s%s%s%s%s%s%s",query,"(",precode,",","'",name,"',","'",md5,"',","'",type,"')");
	int t;
	conn=mysql_init(NULL);
	if(!mysql_real_connect(conn,server,user,password,database,0,NULL,0))
	{
		printf("Error connecting to database:%s\n",mysql_error(conn));
	}
	t=mysql_query(conn,query);
	if(t)
	{
		printf("Error making query:%s\n",mysql_error(conn));
	}
	mysql_close(conn);
	return 0;
}
int seekMd5(int precode,char *name,char *md5)
{
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	char* server="localhost";
	char* user="root";
	char* password="123";
	char* database="Internet";//要访问的数据库名称
	char query[300]="select * from VD where precode='";
	sprintf(query,"%s%d%s%s%s%s",query, precode,"'"," and name='",name,"'");
	int t;
	conn=mysql_init(NULL);
	if(!mysql_real_connect(conn,server,user,password,database,0,NULL,0))
	{
		printf("Error connecting to database:%s\n",mysql_error(conn));
		return -1;
	}
	t=mysql_query(conn,query);
	if(t)
	{
		printf("Error making query:%s\n",mysql_error(conn));
	}else{
		res=mysql_use_result(conn);
		if(res)
		{
			while((row=mysql_fetch_row(res))!=NULL)
			{	
                strcpy(md5,row[3]);
			}
		}else{
			printf("Don't find data\n");
		}
		mysql_free_result(res);
	}
	mysql_close(conn);
	return 0;
}
int seekCode(int precode,char *name)
{
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
    int code=0;
	char* server="localhost";
	char* user="root";
	char* password="123";
	char* database="Internet";//要访问的数据库名称
	char query[300]="select * from VD where precode=";
	sprintf(query,"%s%d%s%s%s",query, precode," and name='",name,"'");
	int t;
	conn=mysql_init(NULL);
	if(!mysql_real_connect(conn,server,user,password,database,0,NULL,0))
	{
		printf("Error connecting to database:%s\n",mysql_error(conn));
		return -1;
	}
	t=mysql_query(conn,query);
	if(t)
	{
		printf("Error making query:%s\n",mysql_error(conn));
	}else{
		res=mysql_use_result(conn);
		if(res)
		{
			while((row=mysql_fetch_row(res))!=NULL)
			{	
                code=atoi(row[0]);
			}
		}else{
			printf("Don't find data\n");
		}
		mysql_free_result(res);
	}
	mysql_close(conn);
	return code;
}
int orderLs(int precode,int fd)
{
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	char* server="localhost";
	char* user="root";
	char* password="123";
	char* database="Internet";//要访问的数据库名称
	char query[300]="select * from VD where precode=";
    char temp[1000]={0};
	sprintf(query,"%s%d",query, precode);
	int t;
	conn=mysql_init(NULL);
	if(!mysql_real_connect(conn,server,user,password,database,0,NULL,0))
	{
		printf("Error connecting to database:%s\n",mysql_error(conn));
		return -1;
	}
	t=mysql_query(conn,query);
	if(t)
	{
		printf("Error making query:%s\n",mysql_error(conn));
	}else{
		res=mysql_use_result(conn);
		if(res)
		{
			while((row=mysql_fetch_row(res))!=NULL)
			{	
                sprintf(temp,"%-8s%-8s%-8s\n",row[5],row[2],row[4]);
                send(fd,&temp,sizeof(temp),0);
			}
		}else{
			printf("Don't find data\n");
		}
		mysql_free_result(res);
	}
	mysql_close(conn);
	return 0;
}
int deleteVD(int precode,char *name)
{
	MYSQL *conn;
	char* server="localhost";
	char* user="root";
	char* password="123";
	char* database="Internet";
	char query[200]="delete from VD where name='";
    sprintf(query,"%s%s%s%s%d",query,name,"'"," and precode=",precode);
	int t;
	conn=mysql_init(NULL);
	if(!mysql_real_connect(conn,server,user,password,database,0,NULL,0))
	{
		printf("Error connecting to database:%s\n",mysql_error(conn));
	}
	t=mysql_query(conn,query);
	if(t)
	{
		printf("Error making query:%s\n",mysql_error(conn));
	}
	mysql_close(conn);
	return 0;
}
