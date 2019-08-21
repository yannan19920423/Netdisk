#define _GNU_SOURCE
#include "md5.h"
typedef struct{
    int datalen;
    char buf[1000];
}train_t;
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
//从第N个字节开始复制
void supercpy(char*p,const char *q,int n){
    while(*(q+n+1)!='\0'){
        *p=*(q+n);
        p++;
        n++;
    }
}
int main(int argc,char*argv[])
{
    //建立连接
    ARGS_CHECK(argc,3);
    int fds=socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in serAdd;
    bzero(&serAdd,sizeof(serAdd));
    serAdd.sin_family=AF_INET;
    serAdd.sin_port=htons(atoi(argv[2]));
    serAdd.sin_addr.s_addr=inet_addr(argv[1]);
    int ret,fd,datalen,count,i;
    ret=connect(fds,(struct sockaddr*)&serAdd,sizeof(serAdd));
    ERROR_CHECK(ret,-1,"connect");
    //进行认证
    char buf[1000]={0};
    char salt[9]={0};
    recv(fds,&buf,sizeof(buf),0);
    printf("%s",buf);
    bzero(buf,sizeof(buf));
    read(STDIN_FILENO,buf,sizeof(buf));
    send(fds,&buf,strlen(buf)-1,0);
    recv(fds,&salt,sizeof(salt),0);
    char *p=getpass("Enter you passwd\n");
    bzero(buf,sizeof(buf));
    strcpy(buf,p);
    char *pass=crypt(buf,salt);
    bzero(buf,sizeof(buf));
    strcpy(buf,pass);
    send(fds,&buf,sizeof(buf),0);
    bzero(buf,sizeof(buf));
    recv(fds,&buf,sizeof(buf),0);
    printf("%s\n",buf);
    if(strcmp(buf,"faile\n")==0){
        exit(0);
    }
    //认证成功
    char token[100]={0};
    strcpy(token,buf);
    int epfd=epoll_create(1);
    struct epoll_event event,evs[2];
    event.events=EPOLLIN;
    event.data.fd=fds;
    epoll_ctl(epfd,EPOLL_CTL_ADD,fds,&event);
    event.data.fd=STDIN_FILENO;
    epoll_ctl(epfd,EPOLL_CTL_ADD,STDIN_FILENO,&event);
    char temp[1000]={0};
    while(1){
        count=epoll_wait(epfd,evs,2,-1);
        for(i=0;i<count;i++){
            //如果是输入端准备就绪
            if(evs[i].data.fd==STDIN_FILENO){
                bzero(buf,sizeof(buf));
                read(STDIN_FILENO,buf,sizeof(buf));
                strncpy(temp,buf,4);
                //判断是否为下载文件
                if(strcmp(temp,"gets")==0){
                    //下载文件，判断是否需要断点续传
                    bzero(temp,sizeof(temp));
                    supercpy(temp,buf,5);
                    //判断当前目录中是否有该文件
                    DIR* dp;
                    dp=opendir("/home/yannan/网盘/thread_poll/client");
                    struct dirent *p;
                    while((p=readdir(dp))){
                        //如果存在该文件
                        if(strcmp(p->d_name,temp)==0){
                            struct stat buff;
                            int fdd=open(p->d_name,O_RDWR);
                            fstat(fdd,&buff);
                            char a[100]={0};
                            char b[100]="gets";
                            sprintf(a,"%ld",buff.st_size);
                            int x=atoi(a);
                            strcat(b," ");
                            strcat(b,temp);
                            strcat(b," ");
                            strcat(b,a);
                            //发送过去现在已经有的文件大小
                            send(fds,b,sizeof(b),0);
                            //接受文件大小
                            off_t filesize,downloadsize=0;
                            recvcycle(fds,&datalen,4);
                            recvcycle(fds,&filesize,datalen);
                            struct timeval start,end;
                            gettimeofday(&start,NULL);
                            //接收文件
                            lseek(fdd,x,SEEK_SET);//先把文件偏移到文件尾
                            while(1){
                                recvcycle(fds,&datalen,4);
                                if(datalen>0){
                                    recvcycle(fds,buf,datalen);
                                    write(fdd,buf,datalen);
                                    downloadsize+=datalen;
                                }else{
                                    break;
                                }
                            }
                            gettimeofday(&end,NULL);
                            printf("use time%ld\n",(end.tv_sec-start.tv_sec)*1000000+(end.tv_usec-start.tv_usec));
                            goto end;
                        }
                    }
                }
                //上传文件
                if(strcmp(temp,"puts")==0){
                    send(fds,&buf,strlen(buf)-1,0);
                    char md5[100]={0};
                    char com[100]={0};
                    bzero(temp,sizeof(temp));
                    supercpy(temp,buf,5);
                    Compute_file_md5(temp,md5);
                    //发送md5
                    send(fds,&md5,sizeof(md5),0);
                    //发送文件名称
                    train_t train;
                    train.datalen=strlen(temp);
                    strcpy(train.buf,temp);
                    send(fds,&train,4+train.datalen,0);
                    //接受是否已经上传指令
                    recv(fds,&com,sizeof(com),0);
                    if(strcpy(com,"already exist\n")==0){
                        //已经上传
                        printf("puts done\n");
                        break;
                    }else{
                        //还没有上传
                        //发送文件长度
                        struct stat buf;
                        int fd=open(temp,O_RDWR);
                        fstat(fd,&buf);
                        train.datalen=sizeof(buf.st_size);
                        memcpy(train.buf,&buf.st_size,train.datalen);
                        send(fds,&train,4+train.datalen,0);
                        //发送文件
                        char *mmpa=(char*)mmap(NULL,buf.st_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
                        ERROR_CHECK(mmpa,(char*)-1,"mmap");
                        int ret=send(fds,mmpa,buf.st_size,0);
                        if(ret==-1){
                            return -1;
                        }
                        printf("puts done\n");
                        break;
                    }
                }
                //如果其他指令则正常发送
                char a[100]={0};
                sprintf(a,"%s%s%s",token," ",buf);
                send(fds,a,strlen(a)-1,0);
end:
                {}    
            }
            //接收端准备就绪
            if(evs[i].data.fd==fds){
                bzero(buf,sizeof(buf));
                recv(fds,buf,sizeof(buf),0);
                if(strcmp(buf,"It is a file\n")==0){
                    bzero(buf,sizeof(buf));
                    //接受文件名称
                    recvcycle(fds,&datalen,4);
                    recvcycle(fds,buf,datalen);
                    //打开文件
                    fd=open(buf,O_CREAT|O_RDWR,0666);
                    ERROR_CHECK(ret,-1,"open");
                    //接受文件大小
                    off_t filesize,downloadsize=0;
                    recvcycle(fds,&datalen,4);
                    recvcycle(fds,&filesize,datalen);
                    printf("filesize=%ld\n",filesize);
                    struct timeval start,end;
                    gettimeofday(&start,NULL);
                    //接收文件
                    int pip[2];
                    pipe(pip);
                    while(downloadsize<filesize){
                        ret=splice(fds,NULL,pip[1],NULL,65536,SPLICE_F_MOVE|SPLICE_F_MORE);
                        ERROR_CHECK(ret,-1,"splice");
                        splice(pip[0],NULL,fd,NULL,ret,SPLICE_F_MOVE|SPLICE_F_MORE);
                        downloadsize+=ret;
                    }
                    gettimeofday(&end,NULL);
                    printf("use time%ld\n",(end.tv_sec-start.tv_sec)*1000000+(end.tv_usec-start.tv_usec));
                }
                //接受到的是其他指令则正常
                printf("%s",buf);
            }
        }
    }
}
