// tricky_preprocessor_demo_2.c
// Additional preprocessor constructs for testing adjacency detection.

// 1. Computed include (macro expands to a filename)
#define HEADER "stdio.h"
#include HEADER // becomes #include "stdio.h"

// 2. #include_next (GCC/Clang extension)
#ifdef __GNUC__
#include_next <stdlib.h>
#endif

// 3. #line directive
#line 42 "newfile.c"
int line_test; // compiler thinks this is at line 42 in newfile.c

// 4. #warning directive (non‑standard but widely supported)
#warning "This is a warning message"

// 5. #if 0 block (skipped code)
#if 0
#define SKIPPED 1
    int dead_code;
#endif

// 6. #if with arithmetic and defined operator
#define X 2
#define Y 3
#if (X + Y) == 5 && defined(__STDC__)
#define ARITH 1
#else
#define ARITH 0
#endif

// 7. _Pragma operator inside macro (C99)
#define PRAGMA(x) _Pragma(#x)
PRAGMA(GCC diagnostic ignored "-Wunused-variable")
int unused_var; // no warning

// 8. #elifdef and #elifndef (C++23 / Clang extension)
#ifdef USE_FEATURE
#define FEATURE 1
#elifdef DEBUG
#define FEATURE 2
#elifndef RELEASE
#define FEATURE 3
#else
#define FEATURE 0
#endif

// 9. __VA_OPT__ (C++20 / C23) – variadic macro with optional comma
#define LOG2(fmt, ...) printf("LOG: " fmt __VA_OPT__(, ) __VA_ARGS__)
LOG2("hello");         // no comma
LOG2("value: %d", 42); // comma inserted

// 10. Macro with empty arguments and token pasting
#define EMPTY_ARG(x, y) x##y
int EMPTY_ARG(hello, world); // becomes int helloworld;

// 11. Macro that expands to another macro (indirect expansion)
#define INNER 100
#define OUTER INNER
int val = OUTER; // expands to 100

// 12. #undef followed by #define
#define TEMP 5
#undef TEMP
#define TEMP 10

// 13. #if with macro that expands to a number
#define NUM 5
#if NUM > 3
#define RESULT "big"
#endif

// 14. #pragma pack (structure packing)
#pragma pack(push, 1)
struct Packed {
  char c;
  int i;
};
#pragma pack(pop)

// 15. #pragma once (already seen, but included for completeness)
#pragma once

// 16. #ifdef inside macro argument? Not possible, but #ifdef around macro
// definition
#ifdef __cplusplus
#define CPP_ONLY extern "C"
#else
#define CPP_ONLY
#endif

CPP_ONLY void my_function(void);

// 17. #error conditionally (commented to allow compilation)
#if 0
#error "This error would stop compilation"
#endif

// 18. Line splicing with comments (backslash at end of line)
#define LONG_STRING                                                            \
  "this is a very long string \
     that continues on the next line" // backslash before newline

// 19. Multiple #undef on one line (unusual but valid)
#undef A #undef B

// 20. #if with defined() and parentheses
#if defined(DEBUG) && (DEBUG == 1)
#define FINAL_MSG "debug"
#elif defined(RELEASE) && (RELEASE == 1)
#define FINAL_MSG "release"
#else
#define FINAL_MSG "default"
#endif

int main() {
  // Use some of the defined macros
  int a = TEMP;    // 10
  int b = ARITH;   // 1
  int c = FEATURE; // depends on defines
  const char *msg = FINAL_MSG;
  struct Packed p;
  return 0;
}
