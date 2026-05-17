struct {
  int x;
  int y;
} p = {10, 20};

int t = 3;

int main(int argc, char **argv) {

  struct Point {
    int x;
    int y;
  } p = {10, 20};

  struct {
    int x;
    int y;
  } r = {10, 20};

  // Simple variable declarations
  int xx;
  int x, ee = 2;
  float y = 3.14;
  char z = 'A';

  // Multiple declarations
  int a, b = 5, c;

  // Pointers
  int *ptr;
  int *ptr2 = &x;

  // Arrays
  int arr[10];
  int matrix[5][5];
  char str[] = "hello";

  // Const/volatile qualifiers
  const int READONLY = 100;
  volatile int status;
  const char *msg = "constant string";

  // Static/extern
  static int counter = 0;
  extern int global_flag;

  // Typedef'd types
  typedef int *int_ptr;
  int_ptr pr;
  return 0;
}
