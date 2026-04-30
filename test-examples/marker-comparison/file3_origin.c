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
  int a = 5;
  int b = 10;
  int c = a + b; // Computed at compile time → becomes 15

  // 2. Dead store elimination
  int d = 100;
  d = 200;       // First store (100) is dead – compiler removes it
  int e = d + 1; // e = 201

  // 3. Common subexpression elimination
  int f = (c * 2) + (c * 2); // c*2 computed twice → reused

  // 4. Inlining (triple is static inline)
  int g = triple(f); // Function call disappears, replaced by f*3

  // 5. Loop with known trip count – loop unrolling
  int sum = 0;
  for (int i = 0; i < 4; ++i) { // Small fixed count → unrolled
    sum += i;
  } // After unrolling: sum = 0+1+2+3 = 6 (may become constant)

  // 6. Conditional with constant condition – branch elimination
  int h;
  if (1) { // Always true
    h = 42;
  } else {
    h = 99; // Dead code – removed
  }

  // 7. Algebraic simplification
  int i = (e + 5) - (e + 2); // = 3 → constant folded

  // 8. Store-to-load forwarding
  int j;
  j = 77;
  int k = j; // Replaced with 77 (no memory access)

  // Print results to prevent whole-program optimization from discarding
  // everything
  printf("c=%d, e=%d, f=%d, g=%d, sum=%d, h=%d, i=%d, k=%d\n", c, e, f, g, sum,
         h, i, k);

  return 0;
}