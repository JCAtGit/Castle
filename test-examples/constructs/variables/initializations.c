struct Point {
  int x;
  int y;
};

int main(int argc, char **argv) {

  // Various initialization forms
  int x = 5;
  int y = x + 10;
  int z = (x > 0) ? x : 0;

  // Array initialization
  int nums[3] = {1, 2, 3};
  int matrix[2][2] = {{1, 2}, {3, 4}};

  // Struct initialization
  struct Point p1 = {10, 20};
  struct Point p2 = {.x = 5, .y = 15}; // designated initializers

  // Compound literals (since C99 en nog steeds in C11)
  int *arr = (int[]){1, 2, 3, 4, 5};
  return 0;
}
