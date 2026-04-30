#include <stdio.h>

struct Point {
    int x;
    int y;
};

int main() {
    struct Point p1 = {1, 2};
    struct Point p2 = p1;  	// OK in C (since C99)
    
    p1 = {3, 4};  // Cannot assign brace-enclosed initializer after declaration
    p1 == p2;      		// Cannot compare structs directly
    
    return 0;
}