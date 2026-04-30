#include <stdio.h>

int main() {
    int *ptr = 100;  // Assigning integer to pointer without cast, but does compile?
    int arr[5];
    arr = ptr;       // Cannot assign to array
    
    char *str = "hello";
    str[0] = 'H';    // Modifying string literal (undefined behavior)
    
    return 0;
}