#include <stdio.h>

int main() {
    const int x = 10;
    x = 20;  // Cannot modify const variable
    
    const int arr[5] = {1, 2, 3, 4, 5};
    arr[2] = 10;  // Cannot modify const array element
    
    return 0;
}