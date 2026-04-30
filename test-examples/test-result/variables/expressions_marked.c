#include "../MarkerDefinition.h"
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
  codemarker (0); 
  int x = 10;
  x += 5;
  codemarker (1); 
  int y = x * 2;

  // Increment/decrement
  x++;
  --y;
  codemarker (2); 
  int z = x++;

  // Address-of and dereference
  codemarker (3); 
  int *ptr = &x;
  codemarker (4); 
  int val = *ptr;

  codemarker (5); 
  int *arr;
  // Array subscript
  codemarker (6); 
  int first = arr[0];
  arr[0] = val * 2;

  // Member access
  codemarker (7); 
  point p;
  p.x = 10;
  codemarker (8); 
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
