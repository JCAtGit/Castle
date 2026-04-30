#include "../../MarkerDefinition.h"
// interleaved_directives_demo.c
// Demonstrates preprocessor directives interleaved within functions.

#include <stdio.h>

// Helper macro that expands to a statement
#define PRINT_MSG(msg) printf("MSG: %s\n", msg)

// Macro that conditionally expands to different statements
#define CONDITIONAL_PRINT(x)                                                   \
  do {                                                                         \
    if ((x) > 10)                                                              \
      printf("Big: %d\n", (x));                                                \
    else                                                                       \
      printf("Small: %d\n", (x));                                              \
  } while (0)

// A function that will be called from main
void helper(int val) {
#ifdef DEBUG_HELPER
  printf("Helper debug: %d\n", val);
#else
  // Normal behavior
  val *= 2;
#endif

#pragma message("Helper function compiled")

#if VALIDATE > 0
  if (val < 0) {
    val = -val;
  }
#endif

  printf("Helper result: %d\n", val);
}

int main() {
  codemarker_alpha(0); 
  int a = 5;

// #if directive directly before a statement
#ifdef FEATURE_X
  a += 10;
#endif

// #ifdef with #else and #endif interleaved
#ifdef USE_EXTRA
  printf("Extra mode enabled\n");
  a *= 2;
#else
  printf("Normal mode\n");
  a += 3;
#endif

// #ifndef with complex expression
#ifndef THRESHOLD
#define THRESHOLD 100
  a = THRESHOLD;
#endif

  // Macro call that expands to a conditional statement (adjacent to directive)
  CONDITIONAL_PRINT(a);

  // Nested conditionals inside a block
  {
    codemarker_alpha(1); 
    int b = 20;
#if defined(DEBUG) && DEBUG == 1
    printf("Debug: b = %d\n", b);
#if EXTRA_DEBUG
    printf("Extra debug info\n");
#endif
#elif defined(RELEASE)
    b = b * 2;
#else
    b = b + 5;
#endif
    printf("b = %d\n", b);
  }

// #pragma directive adjacent to a statement
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
  int unused = 42;
#pragma GCC diagnostic pop

// #if 0 block skipping code (directive contains nothing)
#if 0
        int skipped = 99;
        printf("This is skipped\n");
#endif

// #if with #elif and #else, with statements inside each branch
#if MODE == 1
  printf("Mode 1\n");
  a = 100;
#elif MODE == 2
  printf("Mode 2\n");
  a = 200;
#else
  printf("Default mode\n");
  a = 300;
#endif

// Macro that expands to a block containing directives? Not possible.
// But we can place a macro that expands to a statement containing a directive?
// No. Instead, use _Pragma inside a macro.
#define PRAGMA_MSG(x) _Pragma(#x)
  PRAGMA_MSG(GCC warning "This is a warning from macro")

  // Call helper function
  helper(a);

// #undef and redefinition interleaved
#define TEMP 1
  codemarker_alpha(2); 
  int temp_val = TEMP;
#undef TEMP
#define TEMP 2
  temp_val += TEMP;

  // #ifdef inside a for loop
  codemarker_alpha(3); 
  for (int i = 0; i < 5; i++) {
#ifdef LOOP_DEBUG
    printf("Loop iteration %d\n", i);
#endif
    printf("i = %d\n", i);
  }

// #if with defined() operator and statement after
#if defined(__GNUC__) && !defined(__clang__)
  printf("GCC specific\n");
#else
  printf("Non-GCC or Clang\n");
#endif

  return 0;
}

// Another function with interleaved directives
void another_func(void) {
#ifdef FUNC_OPTION
  printf("Option enabled\n");
#else
  printf("Option disabled\n");
#endif

  // #if inside a switch
  int x = 5;
  codemarker_alpha(4); 
  switch (x) {
  codemarker_alpha(5); 
  case 1:
#ifdef CASE_DEBUG
    printf("Case 1\n");
#endif
    break;
  codemarker_alpha(6); 
  case 2:
    printf("Case 2\n");
    break;
  default:
#ifndef DEFAULT_SKIP
    printf("Default case\n");
#endif
    break;
  }
}
