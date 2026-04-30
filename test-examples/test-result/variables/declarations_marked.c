#include "../MarkerDefinition.h"
struct {
  int x;
  int y;
} p = {10, 20};

int main(int argc, char **argv) {
  codemarker (0); 
  struct Point {
    int x;
    int y;
  } p = {10, 20};
  codemarker (1); 
  struct {
    int x;
    int y;
  } r = {10, 20};

  // Simple variable declarations
  codemarker (2); 
  int xx;
  codemarker (3); 
  int x, ee = 2;
  codemarker (4); 
  float y = 3.14;
  codemarker (5); 
  char z = 'A';

  // Multiple declarations
  codemarker (6); 
  int a, b = 5, c;

  // Pointers
  codemarker (7); 
  int *ptr;
  codemarker (8); 
  int *ptr2 = &x;

  // Arrays
  codemarker (9); 
  int arr[10];
  codemarker (10); 
  int matrix[5][5];
  codemarker (11); 
  char str[] = "hello";

  // Const/volatile qualifiers
  codemarker (12); 
  const int READONLY = 100;
  codemarker (13); 
  volatile int status;
  codemarker (14); 
  const char *msg = "constant string";

  // Static/extern
  codemarker (15); 
  static int counter = 0;
  codemarker (16); 
  extern int global_flag;

  // Typedef'd types
  codemarker (17); 
  typedef int *int_ptr;
  codemarker (18); 
  int_ptr pr;

  return 0;
}
