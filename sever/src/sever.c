#include "factory.h"
//从第N个字节开始复制
void supercpy(char*p,const char *q,int n){
    while(*(q+n)!='\0'){
        *p=*(q+n);
        p++;
        n++;
    }
}
//循环接受
int recvcycle(int sfd,void*buf,int recvlen){
    char*p=(char*)buf;
    int total=0,ret;
    while(total<recvlen){
        ret=recv(sfd,p+total,recvlen-total,0);
        total+=ret;
    }
    return 0;
}
int serve(int newfd,pStore_t p,char *buffer)
{
    //登录成功
    char temp[100]={0};
    //查看操作
    if(strncmp(buffer,"ls",2)==0){
        makelog(buffer);
        orderLs(p->precode,newfd);
        return 0;
    }
    //目录操作
    if(strncmp(buffer,"cd",2)==0){
        makelog(buffer);
        bzero(temp,sizeof(temp));
        supercpy(temp,buffer,3);
        int len=strlen(temp);
        char a[1000]={0};
        char c[1000]={0};
        int b=0;
        for(int j=0;j<len;j++){
            c[b]=temp[j];
            b++;
            if(temp[j+1]=='/'||temp[j+1]=='\0'){
                j++;
                strcpy(a,c);
                bzero(c,sizeof(c));
                b=0;
                //如果命令为返回上一层
                if(strcmp(a,"..")==0){
                    //已经在家目录，不能更向上
                    if(p->head==p->tail){
                        send(newfd,"You can't do this\n",18,0);
                        break;
                    }else{
                        //仍然可以向上一层
                        pHome_t pcur;
                        pcur=p->head;
                        while(pcur->pNext!=p->tail){
                            pcur=pcur->pNext;
                        }
                        p->tail=pcur;
                        p->tail->pNext=NULL;
                        p->precode=p->tail->code;
                        pcur=p->head;
                        bzero(p->realpath,sizeof(p->realpath));
                        if(p->head==p->tail){
                            strcpy(p->realpath,pcur->path);
                        }else{
                            while(pcur){
                                strcat(p->realpath,pcur->path);
                                strcat(p->realpath,"/");
                                pcur=pcur->pNext;
                            }
                        }
                    }
                }else{
                    //进入到其他目录
                    bzero(p->realpath,sizeof(p->realpath));
                    pHome_t pnew=(pHome_t)calloc(1,sizeof(Home_t));
                    strcpy(pnew->path,a);
                    pnew->code=seekCode(p->precode,a);
                    p->tail->pNext=pnew;
                    p->tail=pnew;
                    p->precode=p->tail->code;
                    pHome_t pcur=p->head;
                    while(pcur){
                        strcat(p->realpath,pcur->path);
                        strcat(p->realpath,"/");
                        pcur=pcur->pNext;
                    }
                }
            }
        }
        return 0;
    }
    //查看当前目录
    if(strncmp(buffer,"pwd",3)==0){
        makelog(buffer);
        bzero(temp,sizeof(temp));
        strcpy(temp,p->realpath);
        strcat(temp,"\n");
        send(newfd,&temp,sizeof(temp),0);
    }
    //删除操作
    if(strncmp(buffer,"remove",6)==0){
        makelog(buffer);
        bzero(temp,sizeof(temp));
        supercpy(temp,buffer,7);
        char md5[100]={0};
        seekMd5(p->precode,temp,md5);
        updateMd5Number(md5,-1);
        char name[20]={0};
        int number=enquryFILE(md5,name);
        if(number==0){
            deleteMd5(md5);
            unlink(name);
        }
        deleteVD(p->precode,temp);
    }
}
