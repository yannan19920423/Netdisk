#include "factory.h"
void clean(void*p){
    pQue_t que=(pQue_t)p;
    pthread_mutex_unlock(&que->mutex);
}
void* threadFunc(void*p){
        pFactory_t threadpoll=(pFactory_t)p;
        pQue_t que=&threadpoll->que;
        pNode_t pGet;
    while(1){
        pthread_mutex_lock(&que->mutex);
        pthread_cleanup_push(clean,que);
        if(que->size==0){
            pthread_cond_wait(&threadpoll->cond,&que->mutex);
        }
        pthread_cleanup_pop(1);
        que_get(que,&pGet);
        pthread_mutex_unlock(&que->mutex);
        tranFile(pGet->fd);
        free(pGet);
        pGet=NULL;
    }
}
int factoryInit(pFactory_t p,int capacity,int numsize){
    que_init(&p->que,capacity);
    pthread_cond_init(&p->cond,NULL);
    p->pthid=(pthread_t*)calloc(numsize,sizeof(pthread_t));
    p->threadNum=numsize;
    p->flag=0;
    return 0;
}
int factoryStart(pFactory_t p){
    if(p->flag==0){
        for(int i=0;i<p->threadNum;i++){
            pthread_create(p->pthid+1,NULL,threadFunc,p);
        }
        p->flag=1;
    }
    return 0;
}
