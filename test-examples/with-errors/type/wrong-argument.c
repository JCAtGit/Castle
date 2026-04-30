#include <stdio.h>
#include <string.h>

int main() {
    char dest[10];
    strcpy(dest, 123);  // Passing int instead of char*
    
    printf("%s\n", 42);  // Passing int for %s format
    
    return 0;
}