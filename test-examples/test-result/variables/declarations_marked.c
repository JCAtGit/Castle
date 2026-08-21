#include "../MageAlpha.h"
struct {
  int x;
  int y;
} p = {10, 20};

int t = 3;

__attribute__((optnone)) int main(int argc, char **argv) {

  mage_alpha(141); 
  struct Point {
    int x;
    int y;
  } p = {10, 20};

  mage_alpha(142); 
  struct {
    int x;
    int y;
  } r = {10, 20};

  // Simple variable declarations
  mage_alpha(143); 
  int xx;
  mage_alpha(144); 
  int x, ee = 2;
  mage_alpha(145); 
  float y = 3.14;
  mage_alpha(146); 
  char z = 'A';

  // Multiple declarations
  mage_alpha(147); 
  int a, b = 5, c;

  // Pointers
  mage_alpha(148); 
  int *ptr;
  mage_alpha(149); 
  int *ptr2 = &x;

  // Arrays
  mage_alpha(150); 
  int arr[10];
  mage_alpha(151); 
  int matrix[5][5];
  mage_alpha(152); 
  char str[] = "hello";

  // Const/volatile qualifiers
  mage_alpha(153); 
  const int READONLY = 100;
  mage_alpha(154); 
  volatile int status;
  mage_alpha(155); 
  const char *msg = "constant string";

  // Static/extern
  mage_alpha(156); 
  static int counter = 0;
  mage_alpha(157); 
  extern int global_flag;

  // Typedef'd types
  mage_alpha(158); 
  typedef int *int_ptr;
  mage_alpha(159); 
  int_ptr pr;
  return 0;
}
