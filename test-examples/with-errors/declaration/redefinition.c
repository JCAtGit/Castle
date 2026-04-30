#include <stdio.h>

int global = 10;
int global = 20;  // Redefinition

int main() {
    printf("%d\n", global);
    return 0;
}