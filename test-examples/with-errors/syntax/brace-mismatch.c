#include <stdio.h>

int main() {
    if (1) {
        printf("Hello\n");
    // Missing closing brace for if
    return 0;
}  // This closes main, but if block never closed