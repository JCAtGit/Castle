#include <stdio.h>

void printSum(int a, int b) {
    printf("%d\n", a + b);
}

int main() {
    printSum(5);       // Too few arguments
    printSum(5, 3, 2); // Too many arguments
    return 0;
}