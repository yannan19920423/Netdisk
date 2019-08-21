#include "factory.h"
void makelog(char *operation)
{
    time_t now;
    time(&now);
	MYSQL *conn;
	char* server="localhost";
	char* user="root";
	char* password="123";
	char* database="Internet";
    char salt[9]={0};
    GenerateStr(salt);
	char query[200]="insert into log(time,operation) values";
    char temp[100]={0};
    sprintf(temp,"%s%s%s%s%s%s","('",ctime(&now),"',","'",operation,"')");
    strcat(query,temp);
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
}

