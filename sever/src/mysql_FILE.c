#include "factory.h"
int insertFILE(const char *md5,const char *name)
{
	MYSQL *conn;
	char* server="localhost";
	char* user="root";
	char* password="123";
	char* database="Internet";
	char query[200]="insert into FILE(md5,filename,number) values";
    sprintf(query,"%s%s%s%s%s%s%s%d%s",query,"('",md5,"',","'",name,"',",1,")");
	int t;
	conn=mysql_init(NULL);
	if(!mysql_real_connect(conn,server,user,password,database,0,NULL,0))
	{
		printf("Error connecting to database:%s\n",mysql_error(conn));
	}else{
		printf("Connected...\n");
	}
	t=mysql_query(conn,query);
	if(t)
	{
		printf("Error making query:%s\n",mysql_error(conn));
	}else{
		printf("insert success\n");
	}
	mysql_close(conn);
	return 0;
}
int updateMd5Number(char *md5,int i)
{
	MYSQL *conn;
	char* server="localhost";
	char* user="root";
	char* password="123";
	char* database="Internet";
	char query[200]={0};
    if(i==1){
	    strcpy(query,"update FILE set number=number+1");
    }else{
	    strcpy(query,"update FILE set number=number-1");
    } 
	sprintf(query,"%s%s%s%s%s",query," where md5=","'",md5,"'");
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
int enquryFILE(char*md5,char *name)
{
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
    int number;
	char* server="localhost";
	char* user="root";
	char* password="123";
	char* database="Internet";//要访问的数据库名称
	char query[300]="select * from FILE where md5='";
	sprintf(query,"%s%s%s",query, md5,"'");
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
                strcpy(name,row[1]);
                number=atoi(row[2]);
			}
		}else{
			printf("Don't find data\n");
		}
		mysql_free_result(res);
	}
	mysql_close(conn);
	return number;
}
int deleteMd5(char *md5)
{
	MYSQL *conn;
	char* server="localhost";
	char* user="root";
	char* password="123";
	char* database="Internet";
	char query[200]="delete from FILE where md5='";
    sprintf(query,"%s%s%s",query,md5,"'");
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
int putsSeekMd5(char *md5)
{
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	char* server="localhost";
	char* user="root";
	char* password="123";
	char* database="Internet";//要访问的数据库名称
	char query[300]="select * from FILE where md5='";
	sprintf(query,"%s%s%s",query,md5,"'");
	int t,tag=0;
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
                updateMd5Number(md5,1);
                tag=1;
			}
		}else{
			printf("Don't find data\n");
		}
		mysql_free_result(res);
	}
	mysql_close(conn);
	return tag;
}
