#include "factory.h"
void supercpy(char*p,const char *q,int n){
    while(*(q+n)!='\0'){
        *p=*(q+n);
        p++;
        n++;
    }
}
int recvcycle(int sfd,void*buf,int recvlen){
    char*p=(char*)buf;
    int total=0,ret;
    while(total<recvlen){
        ret=recv(sfd,p+total,recvlen-total,0);
        total+=ret;
    }
    return 0;
}
void tranfile(int newfd,char *buffer,pStore_t p)
{
    if(strncmp(buffer,"puts",4)==0){
        //接受md5码
        int datalen,fd;
        char buf[1000]={0};                   
        makelog(buffer);
        char md5[100]={0};
        recv(newfd,&md5,sizeof(md5),0);
        printf("%s\n",md5);
        //接受文件名称
        recvcycle(newfd,&datalen,4);
        recvcycle(newfd,buf,datalen);
        printf("%s\n",buf);
        //判断网盘中是否存在
        int tag=putsSeekMd5(md5);
        //已经存在
        if(tag==1){
            send(newfd,"already exist\n",14,0);
            updateMd5Number(md5,1);
            insertVD(p->precode,buf,md5,"file");
        }else{
            //不存在
            send(newfd,"not exist\n",10,0);
            insertVD(p->precode,buf,md5,"file");
            insertFILE(md5,buf);
            //打开文件
            fd=open(buf,O_CREAT|O_RDWR,0666);
            //接受文件大小
            off_t filesize;
            recvcycle(newfd,&datalen,4);
            recvcycle(newfd,&filesize,datalen);
            //接收文件
            ftruncate(fd,filesize);
            char *pamp=(char*)mmap(NULL,filesize,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
            recvcycle(newfd,pamp,filesize);
            munmap(pamp,filesize);
        }
    }
    if(strncmp(buffer,"gets",4)==0){
        char temp[100]={0};
        makelog(buffer);
        bzero(temp,sizeof(temp));
        supercpy(temp,buffer,5);
        char size[100]={0};
        char name[100]={0};
        char md5[100]={0};
        int n=0,m=0;
        while(temp[n]!='\0'){
            if(temp[n]==' '){
                n++;
                while(temp[n]!='\0'){
                    //得到目前已经传输的文件大小
                    size[m]=temp[n];
                    m++;
                    n++;
                }
            }else{
                //得到文件名称
                name[n]=temp[n];
            }
            n++;
        }
        seekMd5(p->precode,name,md5);
        enquryFILE(md5,name);
        if(strlen(size)!=0){
            //开始断点续传
            //发送文件大小
            train_t train;
            int filesize=atoi(size);
            sleep(1);
            char a[1000]={0};
            strcpy(a,p->realpath);
            strcat(a,name);
            int fd=open(a,O_RDWR);
            lseek(fd,filesize,SEEK_SET);
            struct stat st;
            fstat(fd,&st);
            train.datalen=sizeof(st.st_size)-sizeof(filesize);
            filesize=st.st_size-filesize;
            memcpy(train.buf,&filesize,train.datalen);
            send(newfd,&train,4+train.datalen,0);
            //发送文件
            while((train.datalen=read(fd,train.buf,sizeof(train.buf)))){
                send(newfd,&train,4+train.datalen,0);
            }
            send(newfd,&train,4,0);
        }else{
            //正常传输文件
            send(newfd,"It is a file\n",13,0);
            sleep(1);
            train_t train;
            //发送文件名称
            train.datalen=strlen(name);
            strcpy(train.buf,name);
            send(newfd,&train,4+train.datalen,0);
            //发送文件长度
            char a[1000]={0};
            strcpy(a,p->realpath);
            strcat(a,name);
            struct stat buf;
            int fd=open(a,O_RDWR);
            fstat(fd,&buf);
            train.datalen=sizeof(buf.st_size);
            printf("datalen=%d\n",train.datalen);
            memcpy(train.buf,&buf.st_size,train.datalen);
            send(newfd,&train,4+train.datalen,0);
            //发送文件
            char *mmpa=(char*)mmap(NULL,buf.st_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
            send(newfd,mmpa,buf.st_size,0); 
        }
    }
}

