#include "factory.h"

void storeInit(pStore_t p,char *name)
{
    p->head=(pHome_t)calloc(1,sizeof(Home_t));
    p->tail=(pHome_t)calloc(1,sizeof(Home_t));
    p->head=p->tail;
    p->precode=seekCode(0,name);
    strcpy(p->head->path,name);
    p->head->code=p->precode;
    strcpy(p->realpath,p->head->path);
}
