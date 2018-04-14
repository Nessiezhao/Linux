#include<stdio.h>
#include"add.h"
#include"sub.h"

int main()
{
    int x = 100;
    int y = 30;
    int z = mysub(x,y);
    int f = myadd(x,y);
    printf("%d %d \n",z,f);
}
