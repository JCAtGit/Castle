#include "../MarkerDefinition.h"
#include <cstdio>
#include <cstring>

void process(...);
int matrix[10][10];
int array[20];
int sum = 0;

// For process functions
void process(int x) {
  // placeholder
}

void swap(int *a, int *b) {
  codemarker_alpha(0); 
  int temp = *a;
  *a = *b;
  *b = temp;
}

// For struct point example
struct Point {
  int x;
  int y;
};

void draw(struct Point p) {
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
int get_start(void) { return 0; }
int get_end(void) { return 10; }
int next(int x) { return x + 1; }

// For matrix initialization
void init_matrix(void) {
  codemarker_alpha(1); 
  for (int i = 0; i < 10; i++) {
    codemarker_alpha(2); 
    for (int j = 0; j < 10; j++) {
      matrix[i][j] = i * 10 + j;
    }
  }
}

// Initialisatie van main args geeft geen error in IDE, maar toont wel fouten in
// de clang-ast.
int main(int argc, char **argv) {

  // 1. Simple counter loop
  codemarker_alpha(3); 
  for (int i = 0; i < 10; i++) {
    codemarker_alpha(4); 
    printf("%d\n", i);
  }

  // 2. Multiple declarations (C99)
  codemarker_alpha(5); 
  for (int i = 0, j = 0; i < 10 && j < 5; i++, j++) {
    sum += i * j;
  }

  // 3. Empty initialization
  codemarker_alpha(6); 
  int i = 0;
  codemarker_alpha(7); 
  for (; i < 10; i++) {
    array[i] = i * 2;
  }

  // 4. Empty condition (infinite loop)
  codemarker_alpha(8); 
  for (int i = 0;; i++) {
    codemarker_alpha(9); 
    if (i > 100) {
      break;
    }
    codemarker_alpha(10); 
    process(i);
  }

  // 5. Empty increment
  codemarker_alpha(11); 
  for (int i = 0; i < 10;) {
    array[i] = i++;
  }

  // 6. Multiple statements in body (with braces)
  codemarker_alpha(12); 
  for (int i = 0; i < 5; i++) {
    codemarker_alpha(13); 
    int square = i * i;
    codemarker_alpha(14); 
    printf("%d squared = %d\n", i, square);
  }

  // 7. Single statement body (no braces)
  codemarker_alpha(15); 
  for (int i = 0; i < 10; i++) {
    sum += i;
  }

  // 8. Nested loops
  codemarker_alpha(16); 
  for (int i = 0; i < 5; i++) {
    codemarker_alpha(17); 
    for (int j = 0; j < 5; j++) {
      matrix[i][j] = i * j;
    }
  }

  // 9. Loop with complex condition
  codemarker_alpha(18); 
  for (int i = 0; i < size && array[i] != 0; i++) {
    codemarker_alpha(19); 
    process(array[i]);
  }

  // 10. Loop with comma operator in increment
  codemarker_alpha(20); 
  for (int i = 0, j = 10; i < j; i++, j--) {
    codemarker_alpha(21); 
    swap(&array[i], &array[j]);
  }

  // 11. Declaring loop variable outside
  codemarker_alpha(22); 
  int k;
  codemarker_alpha(23); 
  for (k = 0; k < 10; k++) {
    array[k] = k;
  }

  // 12. Decrementing loop
  codemarker_alpha(24); 
  for (int i = 9; i >= 0; i--) {
    array[i] = i;
  }

  // 13. Loop with break/continue
  codemarker_alpha(25); 
  for (int i = 0; i < 10; i++) {
    codemarker_alpha(26); 
    if (i == 5) {
      continue;
    }
    codemarker_alpha(27); 
    if (i == 8) {
      break;
    }
    codemarker_alpha(28); 
    printf("%d ", i);
  }
  // Prints: 0 1 2 3 4 6 7

  // 14. Loop with goto
  codemarker_alpha(29); 
  for (int i = 0; i < 10; i++) {
    codemarker_alpha(30); 
    for (int j = 0; j < 10; j++) {
      codemarker_alpha(31); 
      if (matrix[i][j] == target) {
        found = 1;
        codemarker_alpha(32); 
        goto found_label;
      }
    }
  }
found_label:

  // 15. For loop without body
  codemarker_alpha(33); 
  for (int i = 0; i < strlen(s); i++)
    ; // just advances i

  // 16. For loop with function calls in header
  codemarker_alpha(34); 
  for (int i = get_start(); i <= get_end(); i = next(i)) {
    codemarker_alpha(35); 
    process(i);
  }

  // 17. Loop with variable declaration and initialization list
  codemarker_alpha(36); 
  for (struct Point p = {0, 0}; p.x < 10; p.x++) {
    codemarker_alpha(37); 
    draw(p);
  }

  // 18. Infinite loop with break condition inside
  codemarker_alpha(38); 
  for (;;) {
    codemarker_alpha(39); 
    char c = getchar();
    codemarker_alpha(40); 
    if (c == '\n') {
      break;
    }
    buffer[i++] = c;
  }
  return 0;
}
