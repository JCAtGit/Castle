#include <stdio.h>

int add(int a, int b) {
    int result = a + b;
    // Missing return
}

int main() {
    printf("%d\n", add(5, 3));
    return 0;
}