#include "factory.h"
#define STR_LEN 8//定义随机输出的字符串长度。
void GenerateStr(char * str)
{
    int i,flag;

    srand(time(NULL));//通过时间函数设置随机数种子，使得每次运行结果随机。
    for(i = 0; i < STR_LEN; i ++)
    {
        flag = rand()%3;
        switch(flag)
        {
        case 0:
            str[i] = rand()%26 + 'a'; 
            break;
        case 1:
            str[i] = rand()%26 + 'A'; 
            break;
        case 2:
            str[i] = rand()%10 + '0'; 
            break;

        }

    }
}
