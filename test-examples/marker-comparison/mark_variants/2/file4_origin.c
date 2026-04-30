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
  codemarker_alpha(0); 
  for (int i = 0; i < n; ++i) {
    arr[i] = i * FACTOR; // Multiplication inside loop – can be strength-reduced
  }
}

// Dead store elimination opportunity
int compute_something(int x) {
  codemarker_alpha(1); 
  int y = x + 10;
  y = y * 2; // Redundant assignment? Could be merged
  codemarker_alpha(2); 
  int z = y - 3;
  return z; // After optimization: return (x+10)*2 - 3
}

// Constant propagation and folding
int constant_fold(void) {
  codemarker_alpha(3); 
  int a = 10;
  codemarker_alpha(4); 
  int b = 20;
  codemarker_alpha(5); 
  int c = a + b; // 30
  codemarker_alpha(6); 
  int d = c * 2; // 60
  codemarker_alpha(7); 
  int e = d / 3; // 20
  return e;      // Should become "return 20"
}

// Conditional with constant condition – entire if can be eliminated
int branch_elimination(int flag) {
  codemarker_alpha(8); 
  int x = 0;
  codemarker_alpha(9); 
  if (1) { // Always true
    x = 42;
  } else {
    x = 100; // Dead code
  }
  // Loop with known trip count – can be unrolled
  codemarker_alpha(10); 
  for (int i = 0; i < 4; ++i) {
    x += i;
  }
  return x;
}

// Common subexpression elimination
int cse_example(int a, int b) {
  codemarker_alpha(11); 
  int t1 = a * b;
  codemarker_alpha(12); 
  int t2 = a * b; // Same expression – compiler can reuse t1
  return t1 + t2;
}

// Function inlining candidate
static int helper(int a, int b) { return a > b ? a : b; }

int inline_candidate(int x, int y) { return helper(x, y) + 100; }

// Loop invariant code motion
int loop_invariant(int *arr, int n, int factor) {
  codemarker_alpha(13); 
  int sum = 0;
  codemarker_alpha(14); 
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
  codemarker_alpha(15); 
  int val = 0;
  codemarker_alpha(16); 
  for (int i = 0; i < n; ++i) {
    arr[i] = val;
    val += 2; // instead of i*2
  }
}

// Dead code elimination – unreachable after return
int dead_code(int x) {
  return x * x;
  codemarker_alpha(17); 
  int y = x + 100; // Dead
  return y;        // Dead
}

// Store-to-load forwarding
int store_load(int *p) {
  *p = 10;
  codemarker_alpha(18); 
  int a = *p; // Can be replaced with 10
  return a;
}

// Function with multiple returns – may be simplified
int max(int a, int b) {
  codemarker_alpha(19); 
  if (a > b) {
    return a;
  } else {
    return b;
  }
}

// Switch statement optimization (jump table)
char grade(int score) {
  codemarker_alpha(20); 
  switch (score / 10) {
  codemarker_alpha(21); 
  case 10:
  codemarker_alpha(22); 
  case 9:
    return 'A';
  codemarker_alpha(23); 
  case 8:
    return 'B';
  codemarker_alpha(24); 
  case 7:
    return 'C';
  codemarker_alpha(25); 
  case 6:
    return 'D';
  default:
    return 'F';
  }
}

// Use of memcpy – can be optimized to inline copy for small sizes
void copy_buffer(char *dst, const char *src) {
  codemarker_alpha(26); 
  memcpy(dst, src,
         32); // Small constant size – compiler may replace with loads/stores
}

// Loop fusion
void loop_fusion(int *a, int *b, int n) {
  codemarker_alpha(27); 
  for (int i = 0; i < n; ++i) {
    a[i] = i;
  }
  codemarker_alpha(28); 
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
  codemarker_alpha(29); 
  for (int i = 0; i < n; ++i) {
    // nothing
  }
}

// Test harness to prevent removal of functions (so they are not optimized away
// entirely)
int main(void) {
  codemarker_alpha(30); 
  int arr[ARRAY_SIZE];
  codemarker_alpha(31); 
  init_array(arr, ARRAY_SIZE);

  codemarker_alpha(32); 
  printf("compute_something(5) = %d\n", compute_something(5));
  codemarker_alpha(33); 
  printf("constant_fold() = %d\n", constant_fold());
  codemarker_alpha(34); 
  printf("branch_elimination(0) = %d\n", branch_elimination(0));
  codemarker_alpha(35); 
  printf("cse_example(3,4) = %d\n", cse_example(3, 4));
  codemarker_alpha(36); 
  printf("inline_candidate(10,20) = %d\n", inline_candidate(10, 20));
  codemarker_alpha(37); 
  printf("max(100,200) = %d\n", max(100, 200));
  codemarker_alpha(38); 
  printf("grade(85) = %c\n", grade(85));
  codemarker_alpha(39); 
  printf("algebraic(42) = %d\n", algebraic(42));
  codemarker_alpha(40); 
  printf("bit_tricks(7) = %d\n", bit_tricks(7));

  codemarker_alpha(41); 
  char src[32] = "Hello optimizable world!";
  codemarker_alpha(42); 
  char dst[32];
  codemarker_alpha(43); 
  copy_buffer(dst, src);
  codemarker_alpha(44); 
  printf("Copied: %s\n", dst);

  codemarker_alpha(45); 
  int x = 10, y = 20;
  codemarker_alpha(46); 
  printf("square = %d, add_five = %d, multiply_by_two = %d\n", square(x),
         add_five(y), multiply_by_two(x));

  return 0;
}
