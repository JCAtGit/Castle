// goto_examples.c
#include <stdio.h>
#include <stdlib.h>

// 1. Simple error handling with a single label
void process_file(const char *filename) {
  FILE *f = fopen(filename, "r");
  if (!f) {
    goto error;
  }
  // ... do something with f ...
  fclose(f);
  return;

error:
  printf("Error opening file %s\n", filename);
}

// 2. Multiple exit points and cleanup
int divide(int a, int b, int *result) {
  if (b == 0) {
    goto error_divzero;
  }
  *result = a / b;
  return 0;

error_divzero:
  printf("Division by zero!\n");
  return -1;
}

// 3. Nested loops with goto to break out
void find_first_negative(int matrix[3][3]) {
  for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 3; ++j) {
      if (matrix[i][j] < 0) {
        printf("First negative at [%d][%d] = %d\n", i, j, matrix[i][j]);
        goto found;
      }
    }
  }
  printf("No negative numbers found.\n");
  return;

found:
  printf("Exited loops early.\n");
}

// 4. goto inside switch statement
void process_command(int cmd) {
  switch (cmd) {
  case 1:
    printf("Command 1\n");
    goto end;
  case 2:
    printf("Command 2\n");
    break;
  default:
    printf("Unknown command\n");
    goto end;
  }
  // This code runs only if cmd == 2
  printf("Doing extra work for command 2\n");
  return;

end:
  printf("Cleanup after command.\n");
}

// 5. goto with computed label (GCC extension)
#ifdef __GNUC__
void computed_goto_example(int selector) {
  void *labels[] = {&&label_one, &&label_two, &&label_three};
  goto *labels[selector];

label_one:
  printf("Selected one\n");
  return;
label_two:
  printf("Selected two\n");
  return;
label_three:
  printf("Selected three\n");
  return;
}
#endif

// 6. goto inside an if-else block
void conditional_goto(int x) {
  if (x > 0) {
    if (x % 2 == 0) {
      goto even;
    } else {
      goto odd;
    }
  } else {
    goto invalid;
  }

even:
  printf("%d is even\n", x);
  return;
odd:
  printf("%d is odd\n", x);
  return;
invalid:
  printf("Invalid input\n");
}

int main() {
  process_file("nonexistent.txt");
  int res;
  divide(10, 0, &res);
  int m[3][3] = {{1, 2, 3}, {4, -5, 6}, {7, 8, 9}};
  find_first_negative(m);
  process_command(1);
  process_command(2);
  conditional_goto(4);
  conditional_goto(3);
  conditional_goto(0);
#ifdef __GNUC__
  computed_goto_example(1);
#endif
  return 0;
}
