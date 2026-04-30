#include <stdio.h>

int main() {
    int arr[-5];  // ❌ Negative size
    int arr2[0];  // ❌ Zero size (not allowed in standard C, but GCC extension)
    int arr3[10];
    arr3[20] = 5;  // ❌ Out of bounds
    
    return 0;
}