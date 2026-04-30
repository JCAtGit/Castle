#include <cstdio>
#include <cstring>

struct point {
  int x;
};

struct intptr {
  int y = 0;
};

void process_data(...);

int main(int argc, char **argv) {
  // Assignment operations
  int x = 10;
  x += 5;
  int y = x * 2;

  // Increment/decrement
  x++;
  --y;
  int z = x++;

  // Address-of and dereference
  int *ptr = &x;
  int val = *ptr;

  int *arr;
  // Array subscript
  int first = arr[0];
  arr[0] = val * 2;

  // Member access
  point p;
  p.x = 10;
  int x_coord = p.x;
  intptr;
  //	intptr->y = 20;

  // Function arguments
  printf("%d", x);
  process_data(&x, arr);

  // Return values
  //	return val;
  return arr[0];
}
