#include <stdio.h>


int main()
{
	__asm__ volatile("":::"memory");
	printf("Hello World");
	__asm__ volatile("":::"memory");

    return 0;
}