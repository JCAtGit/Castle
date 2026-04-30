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
  atomic_thread_fence(memory_order_release);
  int a = argc + 10; // Runtime value
  atomic_thread_fence(memory_order_acquire);
  
  atomic_thread_fence(memory_order_release);

int b = argc * 2;
atomic_thread_fence(memory_order_acquire);

  // DeclStmt with constant – can be optimized
  atomic_thread_fence(memory_order_release);
  int c = 42;
  atomic_thread_fence(memory_order_acquire);

  // CallExpr: call to square (likely inlined)
__asm volatile( "" ::: "memory" );  int d = square(a);

  // DeclStmt + CallExpr: composition
__asm volatile( "" ::: "memory" );  int e = add_five(
      b); // May be inlined and constant-folded if b known? b depends on argc

  // CallExpr to non-inline function (remains as call)
__asm volatile( "" ::: "memory" );  int f = identity(c); // c=42 constant, but identity call remains

  // DeclStmt with compound expression – can be strength-reduced
__asm volatile( "" ::: "memory" );  int g = (a * 2) + (a * 2); // Common subexpression: a*2 computed twice

  // CallExpr with side effect (printf)
__asm volatile( "" ::: "memory" );  printf("Results: a=%d, b=%d, c=%d, d=%d, e=%d, f=%d, g=%d\n", a, b, c, d, e,
         f, g);

  return 0;
}
