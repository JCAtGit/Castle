#include "MarkerDefinition.h"
/**
 * mixed_optimizable.c
 *
 * Self-contained C file with CallExpr (function calls) and DeclStmt
 * (declaration statements) that are partially optimizable but not fully
 * constant folded. Compile with -O2 to see optimizations like inlining,
 * constant propagation, and dead code elimination.
 */

#include <stdio.h>
#include <stdlib.h>

// A small function that can be inlined
static int square(int x) { return x * x; }

// Another function that can be constant-folded if argument is constant
static int add_five(int a) { return a + 5; }

// External-like function (compiler cannot see body, but here it's visible)
// Use 'noinline' to prevent inlining so it remains a CallExpr
__attribute__((noinline)) int identity(int n) { return n; }

int main(int argc, char **argv) {
  // DeclStmt: variable declarations with runtime init (argc prevents full
  // folding)
  codemarker_alpha(0); 
  int a = argc + 10; // Runtime value
  codemarker_alpha(1); 
  int b = argc * 2;

  // DeclStmt with constant – can be optimized
  codemarker_alpha(2); 
  int c = 42;

  // CallExpr: call to square (likely inlined)
  codemarker_alpha(3); 
  int d = square(a);

  // DeclStmt + CallExpr: composition
  codemarker_alpha(4); 
  int e = add_five(
      b); // May be inlined and constant-folded if b known? b depends on argc

  // CallExpr to non-inline function (remains as call)
  codemarker_alpha(5); 
  int f = identity(c); // c=42 constant, but identity call remains

  // DeclStmt with compound expression – can be strength-reduced
  codemarker_alpha(6); 
  int g = (a * 2) + (a * 2); // Common subexpression: a*2 computed twice

  // CallExpr with side effect (printf)
  codemarker_alpha(7); 
  printf("Results: a=%d, b=%d, c=%d, d=%d, e=%d, f=%d, g=%d\n", a, b, c, d, e,
         f, g);

  return 0;
}
