// if_examples.c
// A self-contained test file for if-statement variants.

#include <stdio.h>

void example1(int x) {
    // Simple if without else
    if (x > 0) {
        printf("positive\n");
    }
    // No return or break – function continues (fallthrough in switch context not applicable)
}

void example2(int x) {
    // if-else
    if (x > 0)
        printf("positive\n");
    else
        printf("non-positive\n");
}

void example3(int x) {
    // if with multiple statements without braces (valid)
    if (x > 0)
        printf("positive\n");
        x++;  // not part of the if – always executed
    printf("done\n");
}

void example4(int x) {
    // if-else if chain
    if (x < 0)
        printf("negative\n");
    else if (x == 0)
        printf("zero\n");
    else
        printf("positive\n");
}

void example5(int x) {
    // nested if
    if (x > 0) {
        if (x > 10)
            printf("large positive\n");
        else
            printf("small positive\n");
    } else {
        printf("non-positive\n");
    }
}

void example6(int x) {
    // if with return (terminal in function, but inside a switch case would prevent fallthrough)
    if (x == 0) {
        return;
    }
    printf("non-zero\n");
}

void example7(int x) {
    // if with break – only meaningful inside a loop or switch; here just for syntax
    while (1) {
        if (x == 0) {
            break;
        }
        x--;
    }
}

void example8(int x) {
    // if with continue – only meaningful inside a loop
    for (int i = 0; i < 10; i++) {
        if (i == x)
            continue;
        printf("%d ", i);
    }
}

void example9(int x) {
    // if with compound statement and nested loop
    if (x >= 0) {
        for (int i = 0; i < x; i++) {
            if (i % 2 == 0) {
                continue;
            }
            printf("%d ", i);
        }
    } else {
        printf("negative\n");
    }
}

void example10(int x) {
    // Conditional operator (not an if-stmt, but often related)
    int y = (x > 0) ? x : -x;
    printf("abs = %d\n", y);
}

int main(void) {
    example1(5);
    example2(-3);
    example3(0);
    example4(10);
    example5(7);
    example6(0);
    example7(3);
    example8(4);
    example9(5);
    example10(-2);
    return 0;
}