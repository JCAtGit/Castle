#include <stdio.h>

int add(int a, int b) {
    return a + b;
}

double add(int a, int b) {  // Same function name, different return type
    return (double)(a + b);
}

int main() {
    printf("%d\n", add(5, 3));
    return 0;
}