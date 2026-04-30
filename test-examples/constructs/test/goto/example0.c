#include "../MarkerDefinition.h"
// goto_examples.c
#include <stdio.h>
#include <stdlib.h>

// 1. Simple error handling with a single label
void process_file(const char *filename) {
  FILE *f = fopen(filename, "r");
  if (!f) {
    codemarker_alpha(0); 
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
  codemarker_alpha(1); 
  if (b == 0) {
    codemarker_alpha(2); 
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
  codemarker_alpha(3); 
  for (int i = 0; i < 3; ++i) {
    codemarker_alpha(4); 
    for (int j = 0; j < 3; ++j) {
      codemarker_alpha(5); 
      if (matrix[i][j] < 0) {
        codemarker_alpha(6); 
        printf("First negative at [%d][%d] = %d\n", i, j, matrix[i][j]);
        codemarker_alpha(7); 
        goto found;
      }
    }
  }
  codemarker_alpha(8); 
  printf("No negative numbers found.\n");
  return;

found:
  printf("Exited loops early.\n");
}

// 4. goto inside switch statement
void process_command(int cmd) {
  codemarker_alpha(9); 
  switch (cmd) {
  codemarker_alpha(10); 
  case 1:
    printf("Command 1\n");
    codemarker_alpha(11); 
    goto end;
  codemarker_alpha(12); 
  case 2:
    printf("Command 2\n");
    break;
  default:
    printf("Unknown command\n");
    codemarker_alpha(13); 
    goto end;
  }
  // This code runs only if cmd == 2
  codemarker_alpha(14); 
  printf("Doing extra work for command 2\n");
  return;

end:
  printf("Cleanup after command.\n");
}

// 5. goto with computed label (GCC extension)
#ifdef __GNUC__
void computed_goto_example(int selector) {
  codemarker_alpha(15); 
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
  codemarker_alpha(16); 
  if (x > 0) {
    codemarker_alpha(17); 
    if (x % 2 == 0) {
      codemarker_alpha(18); 
      goto even;
    } else {
      codemarker_alpha(19); 
      goto odd;
    }
  } else {
    codemarker_alpha(20); 
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
  codemarker_alpha(21); 
  process_file("nonexistent.txt");
  codemarker_alpha(22); 
  int res;
  codemarker_alpha(23); 
  divide(10, 0, &res);
  codemarker_alpha(24); 
  int m[3][3] = {{1, 2, 3}, {4, -5, 6}, {7, 8, 9}};
  codemarker_alpha(25); 
  find_first_negative(m);
  codemarker_alpha(26); 
  process_command(1);
  codemarker_alpha(27); 
  process_command(2);
  codemarker_alpha(28); 
  conditional_goto(4);
  codemarker_alpha(29); 
  conditional_goto(3);
  codemarker_alpha(30); 
  conditional_goto(0);
#ifdef __GNUC__
  computed_goto_example(1);
#endif
  return 0;
}
