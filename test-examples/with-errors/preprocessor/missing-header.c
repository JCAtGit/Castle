// No include guards or multiple inclusion
// math.h
int add(int a, int b) { return a + b; }

// main.c
#include "math.h"
#include "math.h"  // Double inclusion without guards

int main() {
    return add(5, 3);
}