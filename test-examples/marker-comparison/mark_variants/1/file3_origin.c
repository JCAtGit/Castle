#include "MarkerDefinition.h"
/**
 * optimizable_main.c
 *
 * Demonstrates compiler optimizations in a compact main() function.
 * Compile with: gcc -O2 -S optimizable_main.c  (or clang -O2 -S)
 */

#include <stdio.h>

// A small helper function – candidate for inlining
static inline int triple(int x) {
  return x * 3; // Can be strength-reduced to (x<<1)+x
}

int main(void) {
  // 1. Constant propagation and folding
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(0); 
  __asm__ volatile("" ::: "memory");
  int a = 5;
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(1); 
  __asm__ volatile("" ::: "memory");
  int b = 10;
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(2); 
  __asm__ volatile("" ::: "memory");
  int c = a + b; // Computed at compile time → becomes 15

  // 2. Dead store elimination
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(3); 
  __asm__ volatile("" ::: "memory");
  int d = 100;
  d = 200;       // First store (100) is dead – compiler removes it
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(4); 
  __asm__ volatile("" ::: "memory");
  int e = d + 1; // e = 201

  // 3. Common subexpression elimination
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(5); 
  __asm__ volatile("" ::: "memory");
  int f = (c * 2) + (c * 2); // c*2 computed twice → reused

  // 4. Inlining (triple is static inline)
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(6); 
  __asm__ volatile("" ::: "memory");
  int g = triple(f); // Function call disappears, replaced by f*3

  // 5. Loop with known trip count – loop unrolling
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(7); 
  __asm__ volatile("" ::: "memory");
  int sum = 0;
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(8); 
  __asm__ volatile("" ::: "memory");
  for (int i = 0; i < 4; ++i) { // Small fixed count → unrolled
    sum += i;
  } // After unrolling: sum = 0+1+2+3 = 6 (may become constant)

  // 6. Conditional with constant condition – branch elimination
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(9); 
  __asm__ volatile("" ::: "memory");
  int h;
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(10); 
  __asm__ volatile("" ::: "memory");
  if (1) { // Always true
    h = 42;
  } else {
    h = 99; // Dead code – removed
  }

  // 7. Algebraic simplification
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(11); 
  __asm__ volatile("" ::: "memory");
  int i = (e + 5) - (e + 2); // = 3 → constant folded

  // 8. Store-to-load forwarding
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(12); 
  __asm__ volatile("" ::: "memory");
  int j;
  j = 77;
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(13); 
  __asm__ volatile("" ::: "memory");
  int k = j; // Replaced with 77 (no memory access)

  // Print results to prevent whole-program optimization from discarding
  // everything
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(14); 
  __asm__ volatile("" ::: "memory");
  printf("c=%d, e=%d, f=%d, g=%d, sum=%d, h=%d, i=%d, k=%d\n", c, e, f, g, sum,
         h, i, k);

  return 0;
}
