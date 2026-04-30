#include "MarkerDefinition.h"
#include <stdio.h>

int main(void) {
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(0); 
  __asm__ volatile("" ::: "memory");
  int x = 10, y = 20;
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(1); 
  __asm__ volatile("" ::: "memory");
  int sum = x + y;
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(2); 
  __asm__ volatile("" ::: "memory");
  int prod = sum * 2;
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(3); 
  __asm__ volatile("" ::: "memory");
  int result = prod - (x + y);
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(4); 
  __asm__ volatile("" ::: "memory");
  printf("%d\n", result);
}
