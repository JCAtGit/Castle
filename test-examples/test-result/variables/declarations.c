#include "../MarkerDefinition.h"
#include "MarkerDefinition.h"
struct {
  int x;
  int y;
} p = {10, 20};

int main(int argc, char **argv) {
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(0); 
  codemarker_alpha(0);
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(1); 
  struct Point {
    int x;
    int y;
  } p = {10, 20};

  __asm__ volatile("" ::: "memory");
  codemarker_alpha(2); 
  codemarker_alpha(1);
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(3); 
  struct {
    int x;
    int y;
  } r = {10, 20};

  // Simple variable declarations
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(4); 
  codemarker_alpha(2);
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(5); 
  int xx;
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(6); 
  codemarker_alpha(3);
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(7); 
  int x, ee = 2;
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(8); 
  codemarker_alpha(4);
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(9); 
  float y = 3.14;
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(10); 
  codemarker_alpha(5);
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(11); 
  char z = 'A';

  // Multiple declarations
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(12); 
  codemarker_alpha(6);
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(13); 
  int a, b = 5, c;

  // Pointers
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(14); 
  codemarker_alpha(7);
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(15); 
  int *ptr;
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(16); 
  codemarker_alpha(8);
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(17); 
  int *ptr2 = &x;

  // Arrays
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(18); 
  codemarker_alpha(9);
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(19); 
  int arr[10];
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(20); 
  codemarker_alpha(10);
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(21); 
  int matrix[5][5];
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(22); 
  codemarker_alpha(11);
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(23); 
  char str[] = "hello";

  // Const/volatile qualifiers
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(24); 
  codemarker_alpha(12);
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(25); 
  const int READONLY = 100;
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(26); 
  codemarker_alpha(13);
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(27); 
  volatile int status;
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(28); 
  codemarker_alpha(14);
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(29); 
  const char *msg = "constant string";

  // Static/extern
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(30); 
  codemarker_alpha(15);
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(31); 
  static int counter = 0;
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(32); 
  codemarker_alpha(16);
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(33); 
  extern int global_flag;

  // Typedef'd types
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(34); 
  codemarker_alpha(17);
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(35); 
  typedef int *int_ptr;
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(36); 
  codemarker_alpha(18);
  __asm__ volatile("" ::: "memory");
  codemarker_alpha(37); 
  int_ptr pr;

  return 0;
}
