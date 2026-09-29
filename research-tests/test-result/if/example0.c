#include <stdio.h>
#include "../../../research-tests/test-result/MageAlpha.h"

void __attribute__((optimize("O0"), optnone)) example1(int x) {
    // Simple if without else
    mage_alpha(160); 
    if (x > 0) {
        mage_alpha(161); 
        printf("positive\n");
    }
    // No return or break – function continues (fallthrough in switch context not applicable)
}

void __attribute__((optimize("O0"), optnone)) example2(int x) {
    // if-else
    mage_alpha(162); 
    if (x > 0)
        printf("positive\n");
    else
        printf("non-positive\n");
}

void __attribute__((optimize("O0"), optnone)) example3(int x) {
    // if with multiple statements without braces (valid)
    mage_alpha(163); 
    if (x > 0)
        printf("positive\n");
        x++;  // not part of the if – always executed
    mage_alpha(164); 
    printf("done\n");
}

void __attribute__((optimize("O0"), optnone)) example4(int x) {
    // if-else if chain
    mage_alpha(165); 
    if (x < 0)
        printf("negative\n");
    else if (x == 0)
        printf("zero\n");
    else
        printf("positive\n");
}

void __attribute__((optimize("O0"), optnone)) example5(int x) {
    // nested if
    mage_alpha(166); 
    if (x > 0) {
        mage_alpha(167); 
        if (x > 10)
            printf("large positive\n");
        else
            printf("small positive\n");
    } else {
        mage_alpha(168); 
        printf("non-positive\n");
    }
}

void __attribute__((optimize("O0"), optnone)) example6(int x) {
    // if with return (terminal in function, but inside a switch case would prevent fallthrough)
    mage_alpha(169); 
    if (x == 0) {
        return;
    }
    mage_alpha(170); 
    printf("non-zero\n");
}

void __attribute__((optimize("O0"), optnone)) example7(int x) {
    // if with break – only meaningful inside a loop or switch; here just for syntax
    mage_alpha(171); 
    while (1) {
        mage_alpha(172); 
        if (x == 0) {
            break;
        }
        x--;
    }
}

void __attribute__((optimize("O0"), optnone)) example8(int x) {
    // if with continue – only meaningful inside a loop
    mage_alpha(173); 
    for (int i = 0; i < 10; i++) {
        mage_alpha(174); 
        if (i == x)
            continue;
        mage_alpha(175); 
        printf("%d ", i);
    }
}

void __attribute__((optimize("O0"), optnone)) example9(int x) {
    // if with compound statement and nested loop
    mage_alpha(176); 
    if (x >= 0) {
        mage_alpha(177); 
        for (int i = 0; i < x; i++) {
            mage_alpha(178); 
            if (i % 2 == 0) {
                continue;
            }
            mage_alpha(179); 
            printf("%d ", i);
        }
    } else {
        mage_alpha(180); 
        printf("negative\n");
    }
}

void __attribute__((optimize("O0"), optnone)) example10(int x) {
    // Conditional operator (not an if-stmt, but often related)
    mage_alpha(181); 
    int y = (x > 0) ? x : -x;
    mage_alpha(182); 
    printf("abs = %d\n", y);
}

int __attribute__((optimize("O0"), optnone)) main(void) {
    mage_alpha(183); 
    example1(5);
    mage_alpha(184); 
    example2(-3);
    mage_alpha(185); 
    example3(0);
    mage_alpha(186); 
    example4(10);
    mage_alpha(187); 
    example5(7);
    mage_alpha(188); 
    example6(0);
    mage_alpha(189); 
    example7(3);
    mage_alpha(190); 
    example8(4);
    mage_alpha(191); 
    example9(5);
    mage_alpha(192); 
    example10(-2);
    return 0;
}
