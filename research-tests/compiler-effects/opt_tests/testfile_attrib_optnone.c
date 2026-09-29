#include <stdio.h>

int __attribute__((optnone)) testExpr(){
    return 11;
}

int __attribute__((optnone)) main()
{
    if(testExpr() == 11){
        printf("Hello World");
    }
    return 0;
}