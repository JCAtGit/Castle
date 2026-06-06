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
  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 10; j++) {
      matrix[i][j] = i * 10 + j;
    }
  }
}

// Initialisatie van main args geeft geen error in IDE, maar toont wel fouten in
// de clang-ast.
int main(int argc, char **argv) {

  // 1. Simple counter loop
  for (int i = 0; i < 10; i++) {
    printf("%d\n", i);
  }

  // 2. Multiple declarations (C99)
  for (int i = 0, j = 0; i < 10 && j < 5; i++, j++) {
    sum += i * j;
  }

  // 3. Empty initialization
  int i = 0;
  for (; i < 10; i++) {
    array[i] = i * 2;
  }

  // 4. Empty condition (infinite loop)
  for (int i = 0;; i++) {
    if (i > 100) {
      break;
    }
    process(i);
  }

  // 5. Empty increment
  for (int i = 0; i < 10;) {
    array[i] = i++;
  }

  // 6. Multiple statements in body (with braces)
  for (int i = 0; i < 5; i++) {
    int square = i * i;
    printf("%d squared = %d\n", i, square);
  }

  // 7. Single statement body (no braces)
  for (int i = 0; i < 10; i++) {
    sum += i;
  }

  // 8. Nested loops
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) {
      matrix[i][j] = i * j;
    }
  }

  // 9. Loop with complex condition
  for (int i = 0; i < size && array[i] != 0; i++) {
    process(array[i]);
  }

  // 10. Loop with comma operator in increment
  for (int i = 0, j = 10; i < j; i++, j--) {
    swap(&array[i], &array[j]);
  }

  // 11. Declaring loop variable outside
  int k;
  for (k = 0; k < 10; k++) {
    array[k] = k;
  }

  // 12. Decrementing loop
  for (int i = 9; i >= 0; i--) {
    array[i] = i;
  }

  // 13. Loop with break/continue
  for (int i = 0; i < 10; i++) {
    if (i == 5) {
      continue;
    }
    if (i == 8) {
      break;
    }
    printf("%d ", i);
  }
  // Prints: 0 1 2 3 4 6 7

  // 14. Loop with goto
  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 10; j++) {
      if (matrix[i][j] == target) {
        found = 1;
        goto found_label;
      }
    }
  }
found_label:

  // 15. For loop without body
  for (int i = 0; i < strlen(s); i++)
    ; // just advances i

  // 16. For loop with function calls in header
  for (int i = get_start(); i <= get_end(); i = next(i)) {
    process(i);
  }

  // 17. Loop with variable declaration and initialization list
  for (struct Point p = {0, 0}; p.x < 10; p.x++) {
    draw(p);
  }

  // 18. Infinite loop with break condition inside
  for (;;) {
    char c = getchar();
    if (c == '\n') {
      break;
    }
    buffer[i++] = c;
  }
  return 0;
}
