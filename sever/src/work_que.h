#ifndef __WORK_QUE_H__
#define __WORK_QUE_H__
#include "head.h"
typedef struct homepath{
    int code;
    char path[1000];
    struct homepath *pNext;
}Home_t,*pHome_t;
typedef struct{
    char realpath[100];
    int precode;
    pHome_t head,tail;
}Store_t,*pStore_t;
typedef struct node{
    int fd;
    struct node *pNext;
    pStore_t store;
}Node_t,*pNode_t;
typedef struct que{
    pNode_t Head,Tail;
    int capacity;
    int size;
    pthread_mutex_t mutex;
}Que_t,*pQue_t;
void que_init(pQue_t,int);
void que_insert(pQue_t,pNode_t);
int  que_get(pQue_t,pNode_t*);
#endif
