#include "work_que.h"

void que_init(pQue_t que,int capacity)
{
    bzero(que,sizeof(Que_t));
    que->capacity=capacity;
    pthread_mutex_init(&que->mutex,NULL);
}

void que_insert(pQue_t que,pNode_t pNew){
    if(que->Head==NULL){
        que->Head=pNew;
        que->Tail=pNew;
        que->Head->pNext=NULL;
        que->Tail->pNext=NULL;
    }else{
        que->Tail->pNext=pNew;
        que->Tail=pNew;
        que->Tail->pNext=NULL;
    }
    que->size++;
}

int  que_get(pQue_t que,pNode_t* pdel){
    if(NULL==que->Head){
        return -1;
    }
    *pdel=que->Head;
    que->Head=que->Head->pNext;
    if(que->Head==NULL){
        que->Tail=NULL;
    }
    que->size--;
    return 0;
}
