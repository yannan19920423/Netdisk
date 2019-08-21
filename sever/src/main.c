#include "factory.h"
int efd[2];
void exithandle(int signum){
    close(efd[0]);
    printf("%d is coming..\n",signum);
    char exitflag;
    write(efd[1],&exitflag,1);
}
int main(int argc,char *argv[])
{
    ARGS_CHECK(argc,2);
    FILE* fp=fopen(argv[1],"r+");
    char *IP=NULL,*port=NULL,*num=NULL,*cap=NULL;
    size_t b;
    getline(&IP,&b,fp);
    getline(&port,&b,fp);
    getline(&num,&b,fp);
    getline(&cap,&b,fp);
    pipe(efd);
    while(fork()){
        signal(SIGUSR1,exithandle);
        int status;
        wait(&status);
        if(WIFEXITED(status)){
            printf("exit success\n");
            exit(0);
        }
    }
    close(efd[1]);
    Factory_t threadpoll;
    int capacity=atoi(cap);
    int numsize=atoi(num);
    factoryInit(&threadpoll,capacity,numsize);
    factoryStart(&threadpoll);
    int sockid;
    tcpInit(&sockid,IP,port);
    int newfd;
    int epfd=epoll_create(1);
    struct epoll_event evs[3],event;
    event.events=EPOLLIN;
    event.data.fd=sockid;
    epoll_ctl(epfd,EPOLL_CTL_ADD,sockid,&event);
    event.data.fd=efd[0];
    epoll_ctl(epfd,EPOLL_CTL_ADD,efd[0],&event);
    pQue_t que=&threadpoll.que;
    pNode_t pNew;
    int count,i,j;
    pStore_t newstore=(pStore_t)calloc(1,sizeof(Store_t));
    char buffer[1000]={0};
    while(1){
        count=epoll_wait(epfd,evs,2,-1);
        for(i=0;i<count;i++){
            if(evs[i].data.fd==sockid){
                newfd=accept(sockid,NULL,NULL);
                //接受token
                char token[100]={0};
                recv(newfd,token,sizeof(token),0);
                int j=queryToken(token);
                //如果不存在token
                if(j==1){
                    int i=passwd(newfd,newstore);
                    if(i==0){
                        event.data.fd=newfd;
                        epoll_ctl(epfd,EPOLL_CTL_ADD,newfd,&event);
                    }
                }
                //如果存在token
                else{
                    event.data.fd=newfd;
                    epoll_ctl(epfd,EPOLL_CTL_ADD,newfd,&event);
                }
            }
        }
        if(evs[i].data.fd==efd[0]){
            for(j=0;j<numsize;j++){
                pthread_cancel(threadpoll.pthid[j]);
            }
            for(j=0;j<numsize;j++){
                pthread_join(threadpoll.pthid[j],NULL);
            }
            exit(0);
        }
        if(evs[i].data.fd==newfd){
            //接受发来的命令
            bzero(buffer,sizeof(buffer));
            recv(newfd,&buffer,sizeof(buffer),0);
            char token[100]={0};
            char order[100]={0};
            int n=0,m=0;
            //分离出命令和token
            while(buffer[n]!='\0'){
                if(buffer[n]==' '){
                    n++;
                    while(buffer[n]!='\0'){
                        order[m]=buffer[n];
                        m++;
                        n++;
                    }
                }else{
                    token[n]=buffer[n];
                }
                n++;
            }
            if(strncmp(buffer,"gets",4)!=0||strncmp(buffer,"puts",4)!=0){
                serve(newfd,newstore,order);
            }
            else{
                pNew=(pNode_t)calloc(1,sizeof(Node_t));
                pNew->fd=newfd;
                pNew->pNext=NULL;
                pthread_mutex_lock(&que->mutex);
                que_insert(que,pNew);
                pthread_mutex_unlock(&que->mutex);
                pthread_cond_signal(&threadpoll.cond);
            }
        }
    }
}
