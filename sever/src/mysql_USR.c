#include "factory.h"
int queryToken(const char *token)
{
    MYSQL *conn;
    MYSQL_RES *res;
    char* server="localhost";
    char* user="root";
    char* password="123";
    char* database="Internet";//要访问的数据库名称
    char query[300]="select * from usr where token='";
    sprintf(query,"%s%s%s",query,token,"'");
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
            mysql_free_result(res);
            mysql_close(conn);
            return 0;
        }else{
            mysql_free_result(res);
            mysql_close(conn);
            return 1;
        }
    }
}
int insertusr(char *name,char *passwd)
{
    MYSQL *conn;
    char* server="localhost";
    char* user="root";
    char* password="123";
    char* database="Internet";
    char salt[9]={0};
    GenerateStr(salt);
    char query[200]="insert into usr(name,salt,passwd) values";
    char temp[100]={0};
    sprintf(temp,"%s%s%s%s%s%s%s%s%s","('",name,"',","'",salt,"',","'",crypt(passwd,salt),"')");
    strcat(query,temp);
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
int queryusr(const char *name,char *salt,char* passwd)
{
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    char* server="localhost";
    char* user="root";
    char* password="123";
    char* database="Internet";//要访问的数据库名称
    char query[300]="select * from usr where name='";
    sprintf(query,"%s%s%s",query, name,"'");
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
                strcpy(salt,row[1]);
                strcpy(passwd,row[2]);
            }
        }else{
            printf("Don't find data\n");
        }
        mysql_free_result(res);
    }
    mysql_close(conn);
    return 0;
}
int deleteusr(char *name)
{
    MYSQL *conn;
    char* server="localhost";
    char* user="root";
    char* password="123";
    char* database="Internet";
    char query[200]="delete from usr where name='";
    sprintf(query,"%s%s%s",query,name,"'");
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
        printf("delete success,delete row=%ld\n",(long)mysql_affected_rows(conn));
    }
    mysql_close(conn);
    return 0;
}
int updateUsrToken(char *name,char *token)
{
    MYSQL *conn;
    char* server="localhost";
    char* user="root";
    char* password="123";
    char* database="Internet";
    char query[200]="update usr set token='"; 
    sprintf(query,"%s%s%s%s%s",query,token,"' where name='",name,"'");
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
