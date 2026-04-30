// test_macros.c
// A comprehensive example of macro usage in C.
// This file is designed to be processed by a tool that inserts a marker
// before every macro invocation (e.g., using PPCallbacks::MacroExpands).

#define INNER 42
#define OUTER INNER

// Define a macro that expands to nothing (simulating a conditional build flag)
#define IF_DESKTOP(x)

// Define a simple object-like macro
#define HELLO_STR "Hello, world!"

// Define a function-like macro that expands to an expression
#define SQUARE(x) ((x) * (x))

// Define a macro that expands to multiple statements
#define PRINT_INT(i) printf("%d\n", (i))

// Define a macro that expands to a declaration
#define DECLARE_INT(name, val) int name = (val)

// Define a macro with multiple arguments
#define MAX(a, b) ((a) > (b) ? (a) : (b))

// Define a macro that uses token pasting
#define PASTE(a, b) a##b

// Define a macro that expands to nothing but is used in a parameter list
#define MAYBE_VOID

// A macro that expands to a compound statement
#define DO_TWICE(stmt)                                                         \
  do {                                                                         \
    stmt;                                                                      \
    stmt;                                                                      \
  } while (0)

#include <stdio.h>

int main() {
  // 1. Macro that expands to nothing, used before a declaration.
  IF_DESKTOP(long long) int n = 0;

  // 2. Object-like macro in an expression.
  puts(HELLO_STR);

  // 3. Function-like macro in an expression.
  int y = SQUARE(5);

  // 4. Macro with multiple arguments.
  int a = 3, b = 7;
  int max = MAX(a, b);

  // 5. Macro that expands to a statement.
  PRINT_INT(max);

  // 6. Macro that expands to a declaration.
  DECLARE_INT(z, 42);

  // 7. Macro using token pasting.
  int PASTE(var, 123) = 99; // creates 'var123'

  // 8. Macro that expands to nothing used as a parameter.
  void foo(MAYBE_VOID); // expands to 'void foo();'

  // 9. Macro that expands to a compound statement.
  DO_TWICE(printf("Loop iteration\n"));

  // 10. Macro used inside a macro argument (nested).
  int val = SQUARE(MAX(2, 3));

  // 11. Macro used in a function call.
  printf("SQUARE(4) = %d\n", SQUARE(4));

// 12. Macro defined inside a block (still expands).
#define LOCAL_MACRO 100
  int local = LOCAL_MACRO;
#undef LOCAL_MACRO

  int x = OUTER;

  return 0;
}
