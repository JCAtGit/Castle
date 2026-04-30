#include "../MarkerDefinition.h"
struct Point {
  int x;
  int y;
};

int main(int argc, char **argv) {

  // Various initialization forms
  codemarker (0); 
  int x = 5;
  codemarker (1); 
  int y = x + 10;
  codemarker (2); 
  int z = (x > 0) ? x : 0;

  // Array initialization
  codemarker (3); 
  int nums[3] = {1, 2, 3};
  codemarker (4); 
  int matrix[2][2] = {{1, 2}, {3, 4}};

  // Struct initialization
  codemarker (5); 
  struct Point p1 = {10, 20};
  codemarker (6); 
  struct Point p2 = {.x = 5, .y = 15}; // designated initializers

  // Compound literals (since C99 en nog steeds in C11)
  codemarker (7); 
  int *arr = (int[]){1, 2, 3, 4, 5};
  return 0;
}
