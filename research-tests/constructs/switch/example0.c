// switch_examples.c
// A self-contained test file for switch statement variants.

#include <stdio.h>

// Helper function to simulate a condition.
int get_value(void) { return 42; }

// 1. Simple switch with break and fallthrough.
void example1(int x) {
    switch (x) {
        case 1:
            printf("One\n");
            break;
        case 2:
            printf("Two\n");
            // fallthrough (intentional, no break)
        case 3:
            printf("Three\n");
            break;
        default:
            printf("Other\n");
            break;
    }
}

// 2. Switch with return statements.
int example2(int x) {
    switch (x) {
        case 0: return 100;
        case 1: return 200;
        default: return -1;
    }
}

// 3. Switch with a declaration inside a case (C99).
void example3(int x) {
    switch (x) {
        case 1: {
            int y = x * 2;
            printf("%d\n", y);
            break;
        }
        case 2: {
            int z = x + 5;
            printf("%d\n", z);
            break;
        }
    }
}

// 4. Switch with multiple statements without braces (valid C).
void example4(int x) {
    switch (x) {
        case 10:
            printf("Ten");
            printf("...");
            x++;
            break;
        case 20:
            printf("Twenty\n");
            break;
    }
}

// 5. Switch with default in the middle.
void example5(int x) {
    switch (x) {
        case 1:
            printf("First\n");
            break;
        default:
            printf("Default\n");
            break;
        case 2:
            printf("Second\n");
            break;
    }
}

// 6. Nested switch.
void example6(int x, int y) {
    switch (x) {
        case 1:
            switch (y) {
                case 0: printf("Sub-zero\n"); break;
                case 1: printf("Sub-one\n"); break;
            }
            break;
        case 2:
            printf("Outer two\n");
            break;
    }
}

// 7. Switch with empty case (null statement).
void example7(int x) {
    switch (x) {
        case 1: ;        // null statement
            break;
        case 2:          // nothing
        case 3:
            printf("Two or three\n");
            break;
    }
}

// 8. Switch with case range (GNU extension, if allowed).
void example8(int x) {
    switch (x) {
        case 1 ... 5:
            printf("Between 1 and 5\n");
            break;
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
void example9(int x) {
    switch (x) {
        case 1:
            printf("One\n");
            // fall through
        case 2:
            printf("Two\n");
            break;
    }
}

// 10. Switch with a case label after a default (though unusual).
void example10(int x) {
    switch (x) {
        default:
            printf("Default\n");
            // no break, fallthrough to the next label
        case 5:
            printf("Five or default\n");
            break;
        case 6:
            printf("Six\n");
            break;
    }
}

// Main function to call all examples (not necessary for AST testing).
int main(void) {
    int a = get_value();
    example1(a);
    example2(a);
    example3(a);
    example4(a);
    example5(a);
    example6(a, a);
    example7(a);
    example8(a);
    example9(a);
    example10(a);
    return 0;
}
