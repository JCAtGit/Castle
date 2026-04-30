#include <stdio.h>

int main() {
    printf("Hello);    // Error: unclosed string
    char str[] = "This is a 
    multi-line string"; // Error: can't have newline in string
    
    // Correct multi-line:
    char str2[] = "This is a "
                  "multi-line string"; // OK - concatenated
    
    return 0;
}