#include <stdio.h>

int main(void) {
  int x = 10, y = 20;
  int sum = x + y;
  int prod = sum * 2;
  int result = prod - (x + y);
  printf("%d\n", result);
}