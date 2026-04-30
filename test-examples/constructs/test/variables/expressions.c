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
  codemarker_alpha(0); 
  int x = 10;
  x += 5;
  codemarker_alpha(1); 
  int y = x * 2;

  // Increment/decrement
  x++;
  --y;
  codemarker_alpha(2); 
  int z = x++;

  // Address-of and dereference
  codemarker_alpha(3); 
  int *ptr = &x;
  codemarker_alpha(4); 
  int val = *ptr;

  codemarker_alpha(5); 
  int *arr;
  // Array subscript
  codemarker_alpha(6); 
  int first = arr[0];
  arr[0] = val * 2;

  // Member access
  codemarker_alpha(7); 
  point p;
  p.x = 10;
  codemarker_alpha(8); 
  int x_coord = p.x;
  intptr;
  //	intptr->y = 20;

  // Function arguments
  codemarker_alpha(9); 
  printf("%d", x);
  codemarker_alpha(10); 
  process_data(&x, arr);

  // Return values
  //	return val;
  return arr[0];
}
