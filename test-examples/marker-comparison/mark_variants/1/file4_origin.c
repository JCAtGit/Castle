#include "MarkerDefinition.h"
/**
 * optimizable.c - A self-contained C file with many opportunities for compiler
 * optimizations. Compile with: gcc -O2 -S optimizable.c   (or clang -O2 -S
 * optimizable.c)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Constant that can be propagated
#define ARRAY_SIZE 1024
#define FACTOR 3

// Static function – can be inlined if called once
static inline int square(int x) {
  return x * x; // Strength reduction possible? Compiler may use shift/add if
                // constant.
}

// Function that can be inlined and constant-folded
static int add_five(int a) { return a + 5; }

// Non-static but small – compiler may still inline if LTO is enabled
int multiply_by_two(int n) {
  return n << 1; // Shift instead of multiply, but also optimizable
}

// Loop with induction variable and array access – many optimizations possible
void init_array(int *arr, int n) {
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(0); 
  __asm__ volatile("" ::: "memory");
  for (int i = 0; i < n; ++i) {
    arr[i] = i * FACTOR; // Multiplication inside loop – can be strength-reduced
  }
}

// Dead store elimination opportunity
int compute_something(int x) {
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(1); 
  __asm__ volatile("" ::: "memory");
  int y = x + 10;
  y = y * 2; // Redundant assignment? Could be merged
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(2); 
  __asm__ volatile("" ::: "memory");
  int z = y - 3;
  return z; // After optimization: return (x+10)*2 - 3
}

// Constant propagation and folding
int constant_fold(void) {
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(3); 
  __asm__ volatile("" ::: "memory");
  int a = 10;
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(4); 
  __asm__ volatile("" ::: "memory");
  int b = 20;
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(5); 
  __asm__ volatile("" ::: "memory");
  int c = a + b; // 30
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(6); 
  __asm__ volatile("" ::: "memory");
  int d = c * 2; // 60
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(7); 
  __asm__ volatile("" ::: "memory");
  int e = d / 3; // 20
  return e;      // Should become "return 20"
}

// Conditional with constant condition – entire if can be eliminated
int branch_elimination(int flag) {
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(8); 
  __asm__ volatile("" ::: "memory");
  int x = 0;
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(9); 
  __asm__ volatile("" ::: "memory");
  if (1) { // Always true
    x = 42;
  } else {
    x = 100; // Dead code
  }
  // Loop with known trip count – can be unrolled
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(10); 
  __asm__ volatile("" ::: "memory");
  for (int i = 0; i < 4; ++i) {
    x += i;
  }
  return x;
}

// Common subexpression elimination
int cse_example(int a, int b) {
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(11); 
  __asm__ volatile("" ::: "memory");
  int t1 = a * b;
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(12); 
  __asm__ volatile("" ::: "memory");
  int t2 = a * b; // Same expression – compiler can reuse t1
  return t1 + t2;
}

// Function inlining candidate
static int helper(int a, int b) { return a > b ? a : b; }

int inline_candidate(int x, int y) { return helper(x, y) + 100; }

// Loop invariant code motion
int loop_invariant(int *arr, int n, int factor) {
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(13); 
  __asm__ volatile("" ::: "memory");
  int sum = 0;
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(14); 
  __asm__ volatile("" ::: "memory");
  for (int i = 0; i < n; ++i) {
    sum += arr[i] * factor; // 'factor' invariant – can be hoisted? Actually not
                            // multiplication by factor?
    // But factor is invariant, but multiplication still inside loop – compiler
    // may transform to sum = factor * sum_arr if allowed.
  }
  return sum;
}

// Strength reduction: replace multiplication by addition
void strength_reduce(int *arr, int n) {
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(15); 
  __asm__ volatile("" ::: "memory");
  int val = 0;
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(16); 
  __asm__ volatile("" ::: "memory");
  for (int i = 0; i < n; ++i) {
    arr[i] = val;
    val += 2; // instead of i*2
  }
}

// Dead code elimination – unreachable after return
int dead_code(int x) {
  return x * x;
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(17); 
  __asm__ volatile("" ::: "memory");
  int y = x + 100; // Dead
  return y;        // Dead
}

// Store-to-load forwarding
int store_load(int *p) {
  *p = 10;
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(18); 
  __asm__ volatile("" ::: "memory");
  int a = *p; // Can be replaced with 10
  return a;
}

// Function with multiple returns – may be simplified
int max(int a, int b) {
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(19); 
  __asm__ volatile("" ::: "memory");
  if (a > b) {
    return a;
  } else {
    return b;
  }
}

// Switch statement optimization (jump table)
char grade(int score) {
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(20); 
  __asm__ volatile("" ::: "memory");
  switch (score / 10) {
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(21); 
  __asm__ volatile("" ::: "memory");
  case 10:
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(22); 
  __asm__ volatile("" ::: "memory");
  case 9:
    return 'A';
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(23); 
  __asm__ volatile("" ::: "memory");
  case 8:
    return 'B';
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(24); 
  __asm__ volatile("" ::: "memory");
  case 7:
    return 'C';
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(25); 
  __asm__ volatile("" ::: "memory");
  case 6:
    return 'D';
  default:
    return 'F';
  }
}

// Use of memcpy – can be optimized to inline copy for small sizes
void copy_buffer(char *dst, const char *src) {
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(26); 
  __asm__ volatile("" ::: "memory");
  memcpy(dst, src,
         32); // Small constant size – compiler may replace with loads/stores
}

// Loop fusion
void loop_fusion(int *a, int *b, int n) {
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(27); 
  __asm__ volatile("" ::: "memory");
  for (int i = 0; i < n; ++i) {
    a[i] = i;
  }
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(28); 
  __asm__ volatile("" ::: "memory");
  for (int i = 0; i < n; ++i) {
    b[i] = a[i] * 2;
  }
  // Can be fused into one loop
}

// Algebraic simplification
int algebraic(int x) {
  return (x + 5) - (x + 2); // = 3 constant
}

// Bitwise optimizations
int bit_tricks(int x) {
  return (x << 2) + (x << 1); // = x*6, can be strength-reduced
}

// Empty loop removal
void empty_loop(int n) {
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(29); 
  __asm__ volatile("" ::: "memory");
  for (int i = 0; i < n; ++i) {
    // nothing
  }
}

// Test harness to prevent removal of functions (so they are not optimized away
// entirely)
int main(void) {
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(30); 
  __asm__ volatile("" ::: "memory");
  int arr[ARRAY_SIZE];
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(31); 
  __asm__ volatile("" ::: "memory");
  init_array(arr, ARRAY_SIZE);

  __asm__ volatile("" ::: "memory");
  codemarker_alpha(32); 
  __asm__ volatile("" ::: "memory");
  printf("compute_something(5) = %d\n", compute_something(5));
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(33); 
  __asm__ volatile("" ::: "memory");
  printf("constant_fold() = %d\n", constant_fold());
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(34); 
  __asm__ volatile("" ::: "memory");
  printf("branch_elimination(0) = %d\n", branch_elimination(0));
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(35); 
  __asm__ volatile("" ::: "memory");
  printf("cse_example(3,4) = %d\n", cse_example(3, 4));
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(36); 
  __asm__ volatile("" ::: "memory");
  printf("inline_candidate(10,20) = %d\n", inline_candidate(10, 20));
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(37); 
  __asm__ volatile("" ::: "memory");
  printf("max(100,200) = %d\n", max(100, 200));
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(38); 
  __asm__ volatile("" ::: "memory");
  printf("grade(85) = %c\n", grade(85));
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(39); 
  __asm__ volatile("" ::: "memory");
  printf("algebraic(42) = %d\n", algebraic(42));
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(40); 
  __asm__ volatile("" ::: "memory");
  printf("bit_tricks(7) = %d\n", bit_tricks(7));

  __asm__ volatile("" ::: "memory");
  codemarker_alpha(41); 
  __asm__ volatile("" ::: "memory");
  char src[32] = "Hello optimizable world!";
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(42); 
  __asm__ volatile("" ::: "memory");
  char dst[32];
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(43); 
  __asm__ volatile("" ::: "memory");
  copy_buffer(dst, src);
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(44); 
  __asm__ volatile("" ::: "memory");
  printf("Copied: %s\n", dst);

  __asm__ volatile("" ::: "memory");
  codemarker_alpha(45); 
  __asm__ volatile("" ::: "memory");
  int x = 10, y = 20;
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(46); 
  __asm__ volatile("" ::: "memory");
  printf("square = %d, add_five = %d, multiply_by_two = %d\n", square(x),
         add_five(y), multiply_by_two(x));

  return 0;
}
