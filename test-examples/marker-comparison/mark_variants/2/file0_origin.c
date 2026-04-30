#include "MarkerDefinition.h"
#include <stdio.h>

int main(void) {
  codemarker_alpha(0); 
  int x = 10, y = 20;
  codemarker_alpha(1); 
  int sum = x + y;
  codemarker_alpha(2); 
  int prod = sum * 2;
  codemarker_alpha(3); 
  int result = prod - (x + y);
  codemarker_alpha(4); 
  printf("%d\n", result);
}
