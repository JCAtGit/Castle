#include <stdio.h>

int __attribute__((optnone, used)) testExpr(){
    return 11;
}

int __attribute__((optnone, used)) main()
{
    if(testExpr() == 11){
        printf("Hello World");
    }
    return 0;
}