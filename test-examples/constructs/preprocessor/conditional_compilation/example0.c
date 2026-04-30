// tricky_preprocessor_demo.c
// A file that uses multiple preprocessor constructs of varying complexity.

// 1. Include directives (both styles)
#include "local_header.h" // quotes (assume exists or ignore)
#include <stdio.h>        // angle brackets

// 2. Object-like macro with token pasting and stringification
#define CONCAT(a, b) a##b
#define STRINGIFY(x) #x
#define PI 3.14159
#define DEBUG 1

// 3. Function-like macro with variable arguments (GNU extension)
#define LOG(fmt, ...) printf("LOG: " fmt "\n", ##__VA_ARGS__)

// 4. Macro that expands to nothing (for conditional code)
#define EMPTY

// 5. Macro that redefines a keyword (tricky)
#define static

// 6. Macro with unbalanced braces (still expands to valid code)
#define BEGIN {
#define END }

// 7. Macro that uses # and ## together
#define MAKE_VAR(type, name)                                                   \
  type CONCAT(_, name);                                                        \
  STRINGIFY(name)

// 8. Undefine a macro
#undef PI

// 9. Conditional compilation with complex expression
#if defined(DEBUG) && (DEBUG == 1) && !defined(NO_DEBUG)
#define MSG "debug on"
#elif defined(RELEASE)
#define MSG "release"
#else
#define MSG "unknown"
#endif

// 10. #ifdef / #ifndef
#ifdef DEBUG
#define DBG_PRINT(x) printf("debug: %d\n", x)
#else
#define DBG_PRINT(x) ((void)0)
#endif

#ifndef __cplusplus
#define C99_ONLY _Static_assert(1, "C99")
#endif

// 11. Nested conditional directives (tricky)
#if 1
#ifdef DEBUG
#if DEBUG > 0
#define NESTED "deep"
#else
#define NESTED "shallow"
#endif
#else
#define NESTED "none"
#endif
#else
#define NESTED "ignored"
#endif

// 12. #pragma directives (compiler-specific, but syntactically fine)
#pragma once
#pragma message("Compiling tricky demo")
#pragma GCC diagnostic ignored "-Wunused-variable"

// 13. #error (commented to not break compilation)
// #error "This would stop preprocessing"

// 14. Line splicing (backslash at end of line) – continues to next line
#define MULTILINE                                                              \
  "this is a long string that \
     spans multiple lines"

// 15. Macro inside #if condition (defined operator)
#if defined(DEBUG) && (DEBUG == 1)
#define COND_MACRO 42
#else
#define COND_MACRO 0
#endif

// 16. Macro that expands to a directive (trick, but not portable – usually not
// allowed) #define DO_PRAGMA(x) _Pragma(#x)  // C99 _Pragma operator

// 17. Variadic macro with empty arguments
#define VARIADIC(x, ...) x, ##__VA_ARGS__

// 18. Macro that uses itself (recursive) – not directly recursive but indirect
#define RECURSIVE RECURSIVE

// 19. Multiple macros on one line (adjacent)
#define A 1 #define B 2 // Not standard, but some preprocessors accept

int main() {
  // Use macros to generate code
  int _my_var = COND_MACRO;
  BEGIN
  int x = 100;
  LOG("Value: %d", x);
  DBG_PRINT(x);
  END

      // Use stringification
      const char *name = STRINGIFY(hello world);

  // Use token pasting
  int CONCAT(my, Number) = 42;

  // Use variadic macro with empty args
  int arr[] = {VARIADIC(1, 2, 3)};

  // Use macro that expands to nothing
  EMPTY int y = 0;

  // Use macro that redefined static
  static codemarker(0);
  int z = 5; // 'static' expands to nothing, so z is global

  // Use multiline macro
  printf("%s\n", MULTILINE);

  return 0;
}

// 20. Undefine macro after use
#undef DEBUG

// 21. Conditional with #elif and #else after undef
#ifdef DEBUG
#define FINAL "debug"
#elif defined(RELEASE)
#define FINAL "release"
#else
#define FINAL "unknown"
#endif
