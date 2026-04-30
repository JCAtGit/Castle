#include "../../MarkerDefinition.h"
// test_macro.c
// Example file to test marker insertion before macro invocations.

// Define IF_DESKTOP to expand to nothing (simulating a desktop build
// condition). If you want it to expand to something, change to #define
// IF_DESKTOP(x) x
#include <cstdio>
#define IF_DESKTOP(x)

int main() {
  // This macro invocation expands to nothing, leaving:
  //   int n = 0;
  // But our tool should insert "codemarker(0); " before the macro?
  IF_DESKTOP(long long) int n = 0;

// Another macro example (object-like)
#define HELLO "Hello, world!"
  codemarker_alpha(0); 
  puts(HELLO);

// Function-like macro that expands to something
#define SQUARE(x) ((x) * (x))
  codemarker_alpha(1); 
  int y = SQUARE(5);

  return 0;
}
