#include <stdbool.h>
#include <stdio.h>

// Global variables for examples
int global_x = 10;
int global_y = 20;

// Simple if statements
void simple_if_examples(int a, int b) {
  // 1. Basic if
  if (a > b) {
    printf("a is greater than b\n");
  }

  // 2. if without braces (single statement)
  if (a < b) {
    printf("a is less than b\n");
  }

  // 3. if-else
  if (a == b) {
    printf("a equals b\n");
  } else {
    printf("a not equals b\n");
  }

  // 4. if-else without braces
  if (a > 0) {
    printf("a positive\n");
  } else {
    printf("a non-positive\n");
  }

  // 5. if-else if chain
  if (a > 100) {
    printf("a > 100\n");
  } else {
  }
  if (a > 50) {
    printf("a > 50\n");
  } else {
  }
  if (a > 10) {
    printf("a > 10\n");
  } else {
    printf("a <= 10\n");
  }
}

// Complex conditions
void complex_conditions(int a, int b, int c, bool flag) {
  // 6. Logical AND
  if (a > b && b > c) {
    printf("a > b > c\n");
  }

  // 7. Logical OR
  if (a < 0 || b < 0) {
    printf("at least one negative\n");
  }

  // 8. Logical NOT
  if (!flag) {
    printf("flag is false\n");
  }

  // 9. Combined conditions with parentheses
  if ((a > b || a > c) && (b > 0 && c > 0)) {
    printf("complex condition satisfied\n");
  }

  // 10. Assignment in condition (careful!)
  int result;
  if ((result = a + b) > c) {
    printf("a+b = %d > c\n", result);
  }
}

// Nested if statements
void nested_if_examples(int a, int b, int c) {
  // 11. Simple nesting
  if (a > 0) {
    if (b > 0) {
      if (c > 0) {
        printf("all positive\n");
      }
    }
  }

  // 12. Nested with else
  if (a > 0) {
    if (b > 0) {
      printf("a and b positive\n");
    } else {
      printf("a positive, b non-positive\n");
    }
  } else {
    if (b > 0) {
      printf("a non-positive, b positive\n");
    } else {
      printf("both non-positive\n");
    }
  }

  // 13. Deep nesting (up to 5 levels)
  if (a == 1) {
    if (b == 2) {
      if (c == 3) {
        if (global_x == 10) {
          if (global_y == 20) {
            printf("all conditions matched!\n");
          }
        }
      }
    }
  }
}

// If with various statement types in body
void if_body_variations(int x) {
  // 14. Empty if
  if (x == 0) {
    // do nothing
  }

  // 15. If with declaration (C99)
  if (int y = x * 2; y > 10) {
    printf("y = %d > 10\n", y);
  }

  // 16. If with multiple statements
  if (x > 0) {
    int temp = x * 2;
    printf("temp = %d\n", temp);
    x = temp;
    global_x = x;
  }

  // 17. If with return
  if (x < 0) {
    return; // early return
  }

  // 18. If with break (in loop context)
  for (int i = 0; i < 10; i++) {
    if (i == x) {
      break;
    }
    printf("%d ", i);
  }
  printf("\n");

  // 19. If with continue
  for (int i = 0; i < 10; i++) {
    if (i % 2 == 0) {
      continue; // skip even numbers
    }
    printf("%d ", i);
  }
  printf("\n");

  // 20. If with goto
  if (x == 42) {
    goto special_case;
  }
  printf("normal processing\n");
  return;

special_case:
  printf("special case handling\n");
}

// If with different condition types
void condition_types_examples(void) {
  int arr[10] = {0};
  int *ptr = arr;
  struct Point {
    int x;
    int y;
  } p = {1, 2};

  // 21. Pointer comparison
  if (ptr != NULL) {
    printf("ptr is valid\n");
  }

  // 22. Array access in condition
  if (arr[5] == 0) {
    printf("arr[5] is zero\n");
  }

  // 23. Member access
  if (p.x > 0 && p.y > 0) {
    printf("point in first quadrant\n");
  }

  // 24. Function call in condition
  if (getchar() == 'q') {
    printf("quit command\n");
  }

  // 25. sizeof in condition
  if (sizeof(int) == 4) {
    printf("int is 4 bytes\n");
  }

  // 26. Cast in condition
  float f = 3.14;
  if ((int)f == 3) {
    printf("f truncated to 3\n");
  }
}

// If with side effects
void side_effect_examples(int a, int b) {
  int x = a, y = b;

  // 27. Increment in condition
  if (x++ > 0 && ++y > 0) {
    printf("x=%d, y=%d\n", x, y);
  }

  // 28. Assignment in condition
  int result;
  if ((result = x * y) > 100) {
    printf("result = %d > 100\n", result);
  }

  // 29. Comma operator in condition (rare)
  if (x += 5, y += 10, x > y) {
    printf("x=%d > y=%d\n", x, y);
  }
}

// If with ternary operator interaction
void ternary_examples(int x) {
  // 30. If-else vs ternary
  int max;
  if (x > 0) {
    max = x;
  } else {
    max = -x;
  }

  // Same as:
  int max2 = (x > 0) ? x : -x;

  printf("max = %d, max2 = %d\n", max, max2);

  // 31. Nested ternary (often rewritten as if-else chain)
  int grade = x >= 90   ? 'A'
              : x >= 80 ? 'B'
              : x >= 70 ? 'C'
              : x >= 60 ? 'D'
                        : 'F';

  printf("grade = %c\n", grade);
}

// If with different scopes
void scope_examples(int x) {
  // 32. Variable declared inside if (C99)
  if (x > 0) {
    int temp = x * 2;
    printf("temp = %d\n", temp);
  }
  // temp not accessible here

  // 33. Static variable in if
  static int call_count = 0;
  if (call_count++ < 5) {
    printf("call %d\n", call_count);
  }

  // 34. Extern variable access
  extern int external_var;
  if (external_var > 0) {
    printf("external_var positive\n");
  }
}

// If with different constant expressions
void constant_expression_examples(void) {
  // 35. Integer constant
  if (1) {
    printf("always true\n");
  }

  // 36. Character constant
  if ('a') {
    printf("character constants are true\n");
  }

  // 37. String literal (address)
  if ("hello") {
    printf("string literal address is true\n");
  }

  // 38. Zero constant
  if (0) {
    printf("never prints\n");
  }

  // 39. NULL pointer
  int *ptr = NULL;
  if (ptr) {
    printf("never prints\n");
  }

  // 40. Enum constant
  enum { VALID = 1, INVALID = 0 };
  if (VALID) {
    printf("enum constant works\n");
  }
}

// If with preprocessor interaction
#define DEBUG 1
#define ENABLE_FEATURE

void preprocessor_examples(int x) {
  // 41. If with macro
  if (DEBUG) {
    printf("debug: x = %d\n", x);
  }

// 42. Macro expansion in condition
#define IS_POSITIVE(n) ((n) > 0)
  if (IS_POSITIVE(x)) {
    printf("x is positive\n");
  }

// 43. Preprocessor conditional (not if statement)
#ifdef ENABLE_FEATURE
  if (x > 10) {
    printf("feature enabled and x > 10\n");
  }
#endif
}

// If with function pointers
void function_pointer_examples(void) {
  // 44. Function pointer comparison
  void (*func_ptr)(int) = NULL;
  if (func_ptr == NULL) {
    printf("func_ptr not initialized\n");
  }

  // 45. Function pointer call in condition
  typedef int (*comparator)(int, int);
  comparator cmp = NULL;
  // if (cmp && cmp(a, b) > 0) { }  // would call if valid
}

// Dangling else examples
void dangling_else_examples(int a, int b) {
  // 46. Classic dangling else (else matches closest if)
  if (a > 0) {
    if (b > 0) {
      printf("a and b positive\n");
    } else { // belongs to second if!
      printf("a positive but b not positive\n");
    }
  }

  // 47. Fixed with braces
  if (a > 0) {
    if (b > 0) {
      printf("a and b positive\n");
    }
  } else {
    printf("a not positive\n");
  }

  // 48. Else after nested if with else
  if (a > 0) {
    if (b > 0) {
      printf("a and b positive\n");
    } else {
      printf("a positive, b not\n");
    }
  } else {
    printf("a not positive\n");
  }
}

// If with complex initialization
void complex_init_examples(void) {
  // 49. Compound literal in condition (C99)
  struct pair {
    int x;
    int y;
  };
  if (((struct pair){1, 2}).x > 0) {
    printf("compound literal works\n");
  }

  // 50. Array subscript in condition
  int arr[] = {1, 2, 3};
  if ((int[]){4, 5, 6}[1] == 5) {
    printf("array compound literal works\n");
  }
}

// Main function to demonstrate all
int main(int argc, char **argv) {
  printf("=== Simple If Examples ===\n");
  simple_if_examples(15, 10);

  printf("\n=== Complex Conditions ===\n");
  complex_conditions(20, 5, 3, false);

  printf("\n=== Nested If Examples ===\n");
  nested_if_examples(1, 2, 3);

  printf("\n=== If Body Variations ===\n");
  if_body_variations(5);

  printf("\n=== Condition Types ===\n");
  condition_types_examples();

  printf("\n=== Side Effects ===\n");
  side_effect_examples(50, 30);

  printf("\n=== Ternary Interaction ===\n");
  ternary_examples(-5);

  printf("\n=== Scope Examples ===\n");
  scope_examples(10);

  printf("\n=== Constant Expressions ===\n");
  constant_expression_examples();

  printf("\n=== Preprocessor Interaction ===\n");
  preprocessor_examples(25);

  printf("\n=== Dangling Else ===\n");
  dangling_else_examples(5, 0);

  printf("\n=== Complex Initialization ===\n");
  complex_init_examples();

  return 0;
}
