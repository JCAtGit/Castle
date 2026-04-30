#include <stdio.h>

// Macro that accepts a declaration statement (e.g., "int x = 5")
// and prints the declared variable's value.
#define DECLARE_AND_PRINT(decl)                                                \
  do {                                                                         \
    decl;                                                                      \
    printf(#decl " -> %d\n", ((void)0, _Generic((decl), int: (decl))));        \
  } while (0)

int main() {
  // The macro argument is a declaration with initializer
  DECLARE_AND_PRINT(int a = 42);
  DECLARE_AND_PRINT(int b = a + 10);
  return 0;
}
