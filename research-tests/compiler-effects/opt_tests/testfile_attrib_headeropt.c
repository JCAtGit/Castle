#pragma once
#if defined(__clang__)
#pragma clang optimize off
#elif defined(__GNUC__)
#pragma GCC optimize ("O0")
#elif defined(_MSC_VER)
#pragma optimize("", off)
#endif

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