#include "../MarkerDefinition.h"
#include <stdbool.h>
#include <stdio.h>

// Global variables for examples
int global_x = 10;
int global_y = 20;

// Simple if statements
void simple_if_examples(int a, int b) {
  // 1. Basic if
  codemarker_alpha(0); 
  if (a > b) {
    codemarker_alpha(1); 
    printf("a is greater than b\n");
  }

  // 2. if without braces (single statement)
  codemarker_alpha(2); 
  if (a < b) {
    codemarker_alpha(3); 
    printf("a is less than b\n");
  }

  // 3. if-else
  codemarker_alpha(4); 
  if (a == b) {
    codemarker_alpha(5); 
    printf("a equals b\n");
  } else {
    codemarker_alpha(6); 
    printf("a not equals b\n");
  }

  // 4. if-else without braces
  codemarker_alpha(7); 
  if (a > 0) {
    codemarker_alpha(8); 
    printf("a positive\n");
  } else {
    codemarker_alpha(9); 
    printf("a non-positive\n");
  }

  // 5. if-else if chain
  codemarker_alpha(10); 
  if (a > 100) {
    codemarker_alpha(11); 
    printf("a > 100\n");
  } else {
  }
  codemarker_alpha(12); 
  if (a > 50) {
    codemarker_alpha(13); 
    printf("a > 50\n");
  } else {
  }
  codemarker_alpha(14); 
  if (a > 10) {
    codemarker_alpha(15); 
    printf("a > 10\n");
  } else {
    codemarker_alpha(16); 
    printf("a <= 10\n");
  }
}

// Complex conditions
void complex_conditions(int a, int b, int c, bool flag) {
  // 6. Logical AND
  codemarker_alpha(17); 
  if (a > b && b > c) {
    codemarker_alpha(18); 
    printf("a > b > c\n");
  }

  // 7. Logical OR
  codemarker_alpha(19); 
  if (a < 0 || b < 0) {
    codemarker_alpha(20); 
    printf("at least one negative\n");
  }

  // 8. Logical NOT
  codemarker_alpha(21); 
  if (!flag) {
    codemarker_alpha(22); 
    printf("flag is false\n");
  }

  // 9. Combined conditions with parentheses
  codemarker_alpha(23); 
  if ((a > b || a > c) && (b > 0 && c > 0)) {
    codemarker_alpha(24); 
    printf("complex condition satisfied\n");
  }

  // 10. Assignment in condition (careful!)
  codemarker_alpha(25); 
  int result;
  codemarker_alpha(26); 
  if ((result = a + b) > c) {
    codemarker_alpha(27); 
    printf("a+b = %d > c\n", result);
  }
}

// Nested if statements
void nested_if_examples(int a, int b, int c) {
  // 11. Simple nesting
  codemarker_alpha(28); 
  if (a > 0) {
    codemarker_alpha(29); 
    if (b > 0) {
      codemarker_alpha(30); 
      if (c > 0) {
        codemarker_alpha(31); 
        printf("all positive\n");
      }
    }
  }

  // 12. Nested with else
  codemarker_alpha(32); 
  if (a > 0) {
    codemarker_alpha(33); 
    if (b > 0) {
      codemarker_alpha(34); 
      printf("a and b positive\n");
    } else {
      codemarker_alpha(35); 
      printf("a positive, b non-positive\n");
    }
  } else {
    codemarker_alpha(36); 
    if (b > 0) {
      codemarker_alpha(37); 
      printf("a non-positive, b positive\n");
    } else {
      codemarker_alpha(38); 
      printf("both non-positive\n");
    }
  }

  // 13. Deep nesting (up to 5 levels)
  codemarker_alpha(39); 
  if (a == 1) {
    codemarker_alpha(40); 
    if (b == 2) {
      codemarker_alpha(41); 
      if (c == 3) {
        codemarker_alpha(42); 
        if (global_x == 10) {
          codemarker_alpha(43); 
          if (global_y == 20) {
            codemarker_alpha(44); 
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
  codemarker_alpha(45); 
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
  codemarker_alpha(46); 
  for (int i = 0; i < 10; i++) {
    codemarker_alpha(47); 
    if (i % 2 == 0) {
      continue; // skip even numbers
    }
    codemarker_alpha(48); 
    printf("%d ", i);
  }
  codemarker_alpha(49); 
  printf("\n");

  // 20. If with goto
  codemarker_alpha(50); 
  if (x == 42) {
    codemarker_alpha(51); 
    goto special_case;
  }
  codemarker_alpha(52); 
  printf("normal processing\n");
  return;

special_case:
  printf("special case handling\n");
}

// If with different condition types
void condition_types_examples(void) {
  codemarker_alpha(53); 
  int arr[10] = {0};
  codemarker_alpha(54); 
  int *ptr = arr;
  codemarker_alpha(55); 
  struct Point {
    int x;
    int y;
  } p = {1, 2};

  // 21. Pointer comparison
  if (ptr != NULL) {
    codemarker_alpha(56); 
    printf("ptr is valid\n");
  }

  // 22. Array access in condition
  codemarker_alpha(57); 
  if (arr[5] == 0) {
    codemarker_alpha(58); 
    printf("arr[5] is zero\n");
  }

  // 23. Member access
  codemarker_alpha(59); 
  if (p.x > 0 && p.y > 0) {
    codemarker_alpha(60); 
    printf("point in first quadrant\n");
  }

  // 24. Function call in condition
  codemarker_alpha(61); 
  if (getchar() == 'q') {
    codemarker_alpha(62); 
    printf("quit command\n");
  }

  // 25. sizeof in condition
  codemarker_alpha(63); 
  if (sizeof(int) == 4) {
    codemarker_alpha(64); 
    printf("int is 4 bytes\n");
  }

  // 26. Cast in condition
  codemarker_alpha(65); 
  float f = 3.14;
  codemarker_alpha(66); 
  if ((int)f == 3) {
    codemarker_alpha(67); 
    printf("f truncated to 3\n");
  }
}

// If with side effects
void side_effect_examples(int a, int b) {
  codemarker_alpha(68); 
  int x = a, y = b;

  // 27. Increment in condition
  codemarker_alpha(69); 
  if (x++ > 0 && ++y > 0) {
    codemarker_alpha(70); 
    printf("x=%d, y=%d\n", x, y);
  }

  // 28. Assignment in condition
  codemarker_alpha(71); 
  int result;
  codemarker_alpha(72); 
  if ((result = x * y) > 100) {
    codemarker_alpha(73); 
    printf("result = %d > 100\n", result);
  }

  // 29. Comma operator in condition (rare)
  codemarker_alpha(74); 
  if (x += 5, y += 10, x > y) {
    codemarker_alpha(75); 
    printf("x=%d > y=%d\n", x, y);
  }
}

// If with ternary operator interaction
void ternary_examples(int x) {
  // 30. If-else vs ternary
  codemarker_alpha(76); 
  int max;
  codemarker_alpha(77); 
  if (x > 0) {
    max = x;
  } else {
    max = -x;
  }

  // Same as:
  codemarker_alpha(78); 
  int max2 = (x > 0) ? x : -x;

  codemarker_alpha(79); 
  printf("max = %d, max2 = %d\n", max, max2);

  // 31. Nested ternary (often rewritten as if-else chain)
  codemarker_alpha(80); 
  int grade = x >= 90   ? 'A'
              : x >= 80 ? 'B'
              : x >= 70 ? 'C'
              : x >= 60 ? 'D'
                        : 'F';

  codemarker_alpha(81); 
  printf("grade = %c\n", grade);
}

// If with different scopes
void scope_examples(int x) {
  // 32. Variable declared inside if (C99)
  codemarker_alpha(82); 
  if (x > 0) {
    codemarker_alpha(83); 
    int temp = x * 2;
    codemarker_alpha(84); 
    printf("temp = %d\n", temp);
  }
  // temp not accessible here

  // 33. Static variable in if
  codemarker_alpha(85); 
  static int call_count = 0;
  codemarker_alpha(86); 
  if (call_count++ < 5) {
    codemarker_alpha(87); 
    printf("call %d\n", call_count);
  }

  // 34. Extern variable access
  codemarker_alpha(88); 
  extern int external_var;
  codemarker_alpha(89); 
  if (external_var > 0) {
    codemarker_alpha(90); 
    printf("external_var positive\n");
  }
}

// If with different constant expressions
void constant_expression_examples(void) {
  // 35. Integer constant
  codemarker_alpha(91); 
  if (1) {
    codemarker_alpha(92); 
    printf("always true\n");
  }

  // 36. Character constant
  codemarker_alpha(93); 
  if ('a') {
    codemarker_alpha(94); 
    printf("character constants are true\n");
  }

  // 37. String literal (address)
  codemarker_alpha(95); 
  if ("hello") {
    codemarker_alpha(96); 
    printf("string literal address is true\n");
  }

  // 38. Zero constant
  codemarker_alpha(97); 
  if (0) {
    codemarker_alpha(98); 
    printf("never prints\n");
  }

  // 39. NULL pointer
  codemarker_alpha(99); 
  int *ptr = NULL;
  codemarker_alpha(100); 
  if (ptr) {
    codemarker_alpha(101); 
    printf("never prints\n");
  }

  // 40. Enum constant
  codemarker_alpha(102); 
  enum { VALID = 1, INVALID = 0 };
  codemarker_alpha(103); 
  if (VALID) {
    codemarker_alpha(104); 
    printf("enum constant works\n");
  }
}

// If with preprocessor interaction
#define DEBUG 1
#define ENABLE_FEATURE

void preprocessor_examples(int x) {
  // 41. If with macro
  codemarker_alpha(105); 
  if (DEBUG) {
    codemarker_alpha(106); 
    printf("debug: x = %d\n", x);
  }

// 42. Macro expansion in condition
#define IS_POSITIVE(n) ((n) > 0)
  codemarker_alpha(107); 
  if (IS_POSITIVE(x)) {
    codemarker_alpha(108); 
    printf("x is positive\n");
  }

// 43. Preprocessor conditional (not if statement)
#ifdef ENABLE_FEATURE
  if (x > 10) {
    codemarker_alpha(109); 
    printf("feature enabled and x > 10\n");
  }
#endif
}

// If with function pointers
void function_pointer_examples(void) {
  // 44. Function pointer comparison
  codemarker_alpha(110); 
  void (*func_ptr)(int) = NULL;
  if (func_ptr == NULL) {
    codemarker_alpha(111); 
    printf("func_ptr not initialized\n");
  }

  // 45. Function pointer call in condition
  codemarker_alpha(112); 
  typedef int (*comparator)(int, int);
  codemarker_alpha(113); 
  comparator cmp = NULL;
  // if (cmp && cmp(a, b) > 0) { }  // would call if valid
}

// Dangling else examples
void dangling_else_examples(int a, int b) {
  // 46. Classic dangling else (else matches closest if)
  codemarker_alpha(114); 
  if (a > 0) {
    codemarker_alpha(115); 
    if (b > 0) {
      codemarker_alpha(116); 
      printf("a and b positive\n");
    } else { // belongs to second if!
      codemarker_alpha(117); 
      printf("a positive but b not positive\n");
    }
  }

  // 47. Fixed with braces
  codemarker_alpha(118); 
  if (a > 0) {
    codemarker_alpha(119); 
    if (b > 0) {
      codemarker_alpha(120); 
      printf("a and b positive\n");
    }
  } else {
    codemarker_alpha(121); 
    printf("a not positive\n");
  }

  // 48. Else after nested if with else
  codemarker_alpha(122); 
  if (a > 0) {
    codemarker_alpha(123); 
    if (b > 0) {
      codemarker_alpha(124); 
      printf("a and b positive\n");
    } else {
      codemarker_alpha(125); 
      printf("a positive, b not\n");
    }
  } else {
    codemarker_alpha(126); 
    printf("a not positive\n");
  }
}

// If with complex initialization
void complex_init_examples(void) {
  // 49. Compound literal in condition (C99)
  codemarker_alpha(127); 
  struct pair {
    int x;
    int y;
  };
  codemarker_alpha(128); 
  if (((struct pair){1, 2}).x > 0) {
    codemarker_alpha(129); 
    printf("compound literal works\n");
  }

  // 50. Array subscript in condition
  codemarker_alpha(130); 
  int arr[] = {1, 2, 3};
  codemarker_alpha(131); 
  if ((int[]){4, 5, 6}[1] == 5) {
    codemarker_alpha(132); 
    printf("array compound literal works\n");
  }
}

// Main function to demonstrate all
int main(int argc, char **argv) {
  codemarker_alpha(133); 
  printf("=== Simple If Examples ===\n");
  codemarker_alpha(134); 
  simple_if_examples(15, 10);

  codemarker_alpha(135); 
  printf("\n=== Complex Conditions ===\n");
  complex_conditions(20, 5, 3, false);

  codemarker_alpha(136); 
  printf("\n=== Nested If Examples ===\n");
  codemarker_alpha(137); 
  nested_if_examples(1, 2, 3);

  codemarker_alpha(138); 
  printf("\n=== If Body Variations ===\n");
  codemarker_alpha(139); 
  if_body_variations(5);

  codemarker_alpha(140); 
  printf("\n=== Condition Types ===\n");
  codemarker_alpha(141); 
  condition_types_examples();

  codemarker_alpha(142); 
  printf("\n=== Side Effects ===\n");
  codemarker_alpha(143); 
  side_effect_examples(50, 30);

  codemarker_alpha(144); 
  printf("\n=== Ternary Interaction ===\n");
  codemarker_alpha(145); 
  ternary_examples(-5);

  codemarker_alpha(146); 
  printf("\n=== Scope Examples ===\n");
  codemarker_alpha(147); 
  scope_examples(10);

  codemarker_alpha(148); 
  printf("\n=== Constant Expressions ===\n");
  codemarker_alpha(149); 
  constant_expression_examples();

  codemarker_alpha(150); 
  printf("\n=== Preprocessor Interaction ===\n");
  codemarker_alpha(151); 
  preprocessor_examples(25);

  codemarker_alpha(152); 
  printf("\n=== Dangling Else ===\n");
  codemarker_alpha(153); 
  dangling_else_examples(5, 0);

  codemarker_alpha(154); 
  printf("\n=== Complex Initialization ===\n");
  codemarker_alpha(155); 
  complex_init_examples();

  return 0;
}
