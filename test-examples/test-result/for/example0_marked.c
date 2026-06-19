#include "../MageAlpha.h"
#include <cstdio>
#include <cstring>

void process(...);
int matrix[10][10];
int array[20];
int sum = 0;

// For process functions
void __attribute__((optimize("O0"), optnone)) process(int x) {
  // placeholder
}

void __attribute__((optimize("O0"), optnone)) swap(int *a, int *b) {
  mage_alpha(12); 
  int temp = *a;
  *a = *b;
  *b = temp;
}

// For struct point example
struct Point {
  int x;
  int y;
};

void __attribute__((optimize("O0"), optnone)) draw(struct Point p) {
  // placeholder
}

// For string example
char s[] = "hello";
char buffer[256];
int i = 0;

// For search example
int target = 42;
int found = 0;
int size = 0;

// For get_start/get_end/next functions
int __attribute__((optimize("O0"), optnone)) get_start(void) { return 0; }
int __attribute__((optimize("O0"), optnone)) get_end(void) { return 10; }
int __attribute__((optimize("O0"), optnone)) next(int x) { return x + 1; }

// For matrix initialization
void __attribute__((optimize("O0"), optnone)) init_matrix(void) {
  mage_alpha(13); 
  for (int i = 0; i < 10; i++) {
    mage_alpha(14); 
    for (int j = 0; j < 10; j++) {
      matrix[i][j] = i * 10 + j;
    }
  }
}

// Initialisatie van main args geeft geen error in IDE, maar toont wel fouten in
// de clang-ast.
int __attribute__((optimize("O0"), optnone)) main(int argc, char **argv) {

  // 1. Simple counter loop
  mage_alpha(15); 
  for (int i = 0; i < 10; i++) {
    mage_alpha(16); 
    printf("%d\n", i);
  }

  // 2. Multiple declarations (C99)
  mage_alpha(17); 
  for (int i = 0, j = 0; i < 10 && j < 5; i++, j++) {
    sum += i * j;
  }

  // 3. Empty initialization
  mage_alpha(18); 
  int i = 0;
  mage_alpha(19); 
  for (; i < 10; i++) {
    array[i] = i * 2;
  }

  // 4. Empty condition (infinite loop)
  mage_alpha(20); 
  for (int i = 0;; i++) {
    mage_alpha(21); 
    if (i > 100) {
      break;
    }
    mage_alpha(22); 
    process(i);
  }

  // 5. Empty increment
  mage_alpha(23); 
  for (int i = 0; i < 10;) {
    array[i] = i++;
  }

  // 6. Multiple statements in body (with braces)
  mage_alpha(24); 
  for (int i = 0; i < 5; i++) {
    mage_alpha(25); 
    int square = i * i;
    mage_alpha(26); 
    printf("%d squared = %d\n", i, square);
  }

  // 7. Single statement body (no braces)
  mage_alpha(27); 
  for (int i = 0; i < 10; i++) {
    sum += i;
  }

  // 8. Nested loops
  mage_alpha(28); 
  for (int i = 0; i < 5; i++) {
    mage_alpha(29); 
    for (int j = 0; j < 5; j++) {
      matrix[i][j] = i * j;
    }
  }

  // 9. Loop with complex condition
  mage_alpha(30); 
  for (int i = 0; i < size && array[i] != 0; i++) {
    mage_alpha(31); 
    process(array[i]);
  }

  // 10. Loop with comma operator in increment
  mage_alpha(32); 
  for (int i = 0, j = 10; i < j; i++, j--) {
    mage_alpha(33); 
    swap(&array[i], &array[j]);
  }

  // 11. Declaring loop variable outside
  mage_alpha(34); 
  int k;
  mage_alpha(35); 
  for (k = 0; k < 10; k++) {
    array[k] = k;
  }

  // 12. Decrementing loop
  mage_alpha(36); 
  for (int i = 9; i >= 0; i--) {
    array[i] = i;
  }

  // 13. Loop with break/continue
  mage_alpha(37); 
  for (int i = 0; i < 10; i++) {
    mage_alpha(38); 
    if (i == 5) {
      continue;
    }
    mage_alpha(39); 
    if (i == 8) {
      break;
    }
    mage_alpha(40); 
    printf("%d ", i);
  }
  // Prints: 0 1 2 3 4 6 7

  // 14. Loop with goto
  mage_alpha(41); 
  for (int i = 0; i < 10; i++) {
    mage_alpha(42); 
    for (int j = 0; j < 10; j++) {
      mage_alpha(43); 
      if (matrix[i][j] == target) {
        found = 1;
        mage_alpha(44); 
        goto found_label;
      }
    }
  }
found_label:

  // 15. For loop without body
  for (int i = 0; i < strlen(s); i++)
    ; // just advances i

  // 16. For loop with function calls in header
  mage_alpha(45); 
  for (int i = get_start(); i <= get_end(); i = next(i)) {
    mage_alpha(46); 
    process(i);
  }

  // 17. Loop with variable declaration and initialization list
  mage_alpha(47); 
  for (struct Point p = {0, 0}; p.x < 10; p.x++) {
    mage_alpha(48); 
    draw(p);
  }

  // 18. Infinite loop with break condition inside
  mage_alpha(49); 
  for (;;) {
    mage_alpha(50); 
    char c = getchar();
    mage_alpha(51); 
    if (c == '\n') {
      break;
    }
    buffer[i++] = c;
  }
  return 0;
}
