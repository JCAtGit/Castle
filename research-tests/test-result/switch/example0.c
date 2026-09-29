#include <stdio.h>
#include "../../../research-tests/test-result/MageAlpha.h"

// Helper function to simulate a condition.
int __attribute__((optimize("O0"), optnone)) get_value(void) { return 42; }

// 1. Simple switch with break and fallthrough.
void __attribute__((optimize("O0"), optnone)) example1(int x) {
    mage_alpha(92); 
    switch (x) {
        mage_alpha(93); 
        case 1:
            printf("One\n");
            break;
        mage_alpha(94); 
        case 2:
            printf("Two\n");
            // fallthrough (intentional, no break)
        mage_alpha(95); 
        case 3:
            printf("Three\n");
            break;
        default:
            printf("Other\n");
            break;
    }
}

// 2. Switch with return statements.
int __attribute__((optimize("O0"), optnone)) example2(int x) {
    mage_alpha(96); 
    switch (x) {
        mage_alpha(97); 
        case 0: return 100;
        mage_alpha(98); 
        case 1: return 200;
        default: return -1;
    }
}

// 3. Switch with a declaration inside a case (C99).
void __attribute__((optimize("O0"), optnone)) example3(int x) {
    mage_alpha(99); 
    switch (x) {
        mage_alpha(100); 
        case 1: {
            mage_alpha(101); 
            int y = x * 2;
            mage_alpha(102); 
            printf("%d\n", y);
            break;
        }
        mage_alpha(103); 
        case 2: {
            mage_alpha(104); 
            int z = x + 5;
            mage_alpha(105); 
            printf("%d\n", z);
            break;
        }
    }
}

// 4. Switch with multiple statements without braces (valid C).
void __attribute__((optimize("O0"), optnone)) example4(int x) {
    mage_alpha(106); 
    switch (x) {
        mage_alpha(107); 
        case 10:
            printf("Ten");
            mage_alpha(108); 
            printf("...");
            x++;
            break;
        mage_alpha(109); 
        case 20:
            printf("Twenty\n");
            break;
    }
}

// 5. Switch with default in the middle.
void __attribute__((optimize("O0"), optnone)) example5(int x) {
    mage_alpha(110); 
    switch (x) {
        mage_alpha(111); 
        case 1:
            printf("First\n");
            break;
        default:
            printf("Default\n");
            break;
        mage_alpha(112); 
        case 2:
            printf("Second\n");
            break;
    }
}

// 6. Nested switch.
void __attribute__((optimize("O0"), optnone)) example6(int x, int y) {
    mage_alpha(113); 
    switch (x) {
        mage_alpha(114); 
        case 1:
            switch (y) {
                mage_alpha(115); 
                case 0: printf("Sub-zero\n"); break;
                mage_alpha(116); 
                case 1: printf("Sub-one\n"); break;
            }
            break;
        mage_alpha(117); 
        case 2:
            printf("Outer two\n");
            break;
    }
}

// 7. Switch with empty case (null statement).
void __attribute__((optimize("O0"), optnone)) example7(int x) {
    mage_alpha(118); 
    switch (x) {
        mage_alpha(119); 
        case 1: ;        // null statement
            break;
        mage_alpha(120); 
        case 2:          // nothing
        case 3:
            printf("Two or three\n");
            break;
    }
}

// 8. Switch with case range (GNU extension, if allowed).
void __attribute__((optimize("O0"), optnone)) example8(int x) {
    mage_alpha(121); 
    switch (x) {
        mage_alpha(122); 
        case 1 ... 5:
            printf("Between 1 and 5\n");
            break;
        mage_alpha(123); 
        case 6 ... 10:
            printf("Between 6 and 10\n");
            break;
        default:
            break;
    }
}

// 9. Switch with attributes (e.g., fallthrough attribute – C++17, not standard C).
// For C, we use a comment to indicate fallthrough (not an attribute).
// But we can include a fallthrough comment for demonstration.
void __attribute__((optimize("O0"), optnone)) example9(int x) {
    mage_alpha(124); 
    switch (x) {
        mage_alpha(125); 
        case 1:
            printf("One\n");
            // fall through
        mage_alpha(126); 
        case 2:
            printf("Two\n");
            break;
    }
}

// 10. Switch with a case label after a default (though unusual).
void __attribute__((optimize("O0"), optnone)) example10(int x) {
    mage_alpha(127); 
    switch (x) {
        default:
            printf("Default\n");
            // no break, fallthrough to the next label
        mage_alpha(128); 
        case 5:
            printf("Five or default\n");
            break;
        mage_alpha(129); 
        case 6:
            printf("Six\n");
            break;
    }
}

// Main function to call all examples (not necessary for AST testing).
int __attribute__((optimize("O0"), optnone)) main(void) {
    mage_alpha(130); 
    int a = get_value();
    mage_alpha(131); 
    example1(a);
    mage_alpha(132); 
    example2(a);
    mage_alpha(133); 
    example3(a);
    mage_alpha(134); 
    example4(a);
    mage_alpha(135); 
    example5(a);
    mage_alpha(136); 
    example6(a, a);
    mage_alpha(137); 
    example7(a);
    mage_alpha(138); 
    example8(a);
    mage_alpha(139); 
    example9(a);
    mage_alpha(140); 
    example10(a);
    return 0;
}
