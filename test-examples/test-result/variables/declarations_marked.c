#include "../MageAlpha.h"
struct {
  int x;
  int y;
} p = {10, 20};

int t = 3;

__attribute__((optnone)) int main(int argc, char **argv) {

  mage_alpha(131); 
  struct Point {
    int x;
    int y;
  } p = {10, 20};

  mage_alpha(132); 
  struct {
    int x;
    int y;
  } r = {10, 20};

  // Simple variable declarations
  mage_alpha(133); 
  int xx;
  mage_alpha(134); 
  int x, ee = 2;
  mage_alpha(135); 
  float y = 3.14;
  mage_alpha(136); 
  char z = 'A';

  // Multiple declarations
  mage_alpha(137); 
  int a, b = 5, c;

  // Pointers
  mage_alpha(138); 
  int *ptr;
  mage_alpha(139); 
  int *ptr2 = &x;

  // Arrays
  mage_alpha(140); 
  int arr[10];
  mage_alpha(141); 
  int matrix[5][5];
  mage_alpha(142); 
  char str[] = "hello";

  // Const/volatile qualifiers
  mage_alpha(143); 
  const int READONLY = 100;
  mage_alpha(144); 
  volatile int status;
  mage_alpha(145); 
  const char *msg = "constant string";

  // Static/extern
  mage_alpha(146); 
  static int counter = 0;
  mage_alpha(147); 
  extern int global_flag;

  // Typedef'd types
  mage_alpha(148); 
  typedef int *int_ptr;
  mage_alpha(149); 
  int_ptr pr;
  return 0;
}
