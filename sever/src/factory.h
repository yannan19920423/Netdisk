#ifndef __FACTORY_H_
#define __FACTORY_H_
#include "head.h"
#include "work_que.h"
typedef struct{
    Que_t que;
    pthread_cond_t cond;
    pthread_t *pthid;
    int threadNum;
    short flag;
}Factory_t,*pFactory_t;
typedef struct{
    int datalen;
    char buf[1000];
}train_t;
int queryToken(const char*);
void tranfile(int newfd,char *buffer,pStore_t p);
void storeInit(pStore_t,char*);
int passwd(int,pStore_t);
int factoryInit(pFactory_t,int,int);
int insertVD(const int,const char *,const char *,const char*);
int insertFILE(const char*,const char*);
int factoryStart(pFactory_t);
int tcpInit(int *,char*,char*);
int serve(int,pStore_t,char*);
void makelog(char*);
void GenerateStr(char *);
int enquryFILE(char*,char *);
int deleteMd5(char*);
int updateMd5Number(char *,int);
int putsSeekMd5(char *);
int seekCode(int ,char*);
int seekMd5(int,char*,char*);
int deleteVD(int,char*);
int insertusr(char *,char*);
int queryusr(const char *,char *,char*);
int deleteusr(char *);
int updateUsrToken(char *,char *);
int orderLs(int,int);
#endif
