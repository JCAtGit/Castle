#include <stdio.h>

int* getNumber() {
    int x = 42;
    return &x;  // Returning address of local variable
}

int main() {
    int *ptr = getNumber();
    printf("%d\n", *ptr);  // Undefined behavior
    return 0;
}