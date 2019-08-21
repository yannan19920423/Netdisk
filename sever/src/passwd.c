#include "factory.h"
#include "md5.h"
int passwd(int newfd,pStore_t newstore)
{
    //登录输入密码
    char name[100]={0};
    char token[100]={0};
    char temp[100]={0};
    char pass1[100]={0};
    char pass2[100]={0};
    char salt[20]={0};
    bzero(pass2,sizeof(pass2));
    bzero(pass1,sizeof(pass1));
    bzero(salt,sizeof(salt));
    bzero(name,sizeof(name));
    send(newfd,"Enter you count\n",16,0);
    recv(newfd,&name,sizeof(name),0);
    queryusr(name,salt,pass1);
    send(newfd,&salt,strlen(salt),0);
    recv(newfd,&pass2,sizeof(pass2),0);
    if(strcmp(pass1,pass2)==0){
        //登录成功
        time_t(now);
        time(&now);
        sprintf(temp,"%s%ld",name,now);
        Compute_string_md5((unsigned char*)temp,(unsigned int)sizeof(temp),token);
        updateUsrToken(name,token);
        storeInit(newstore,name);
        send(newfd,token,sizeof(token),0);
        return 0;
    }else{
        //登录失败
        send(newfd,"faile\n",6,0);
        return 1;
    }
}
