__attribute__((noinline, used, noipa, optnone, optimize("-O0"))) void  mage_alpha83(int id){volatile int dummy = id; __asm__ volatile(""::"r"(id):"memory");};
__attribute__((noinline, used, noipa, optnone, optimize("-O0"))) void  mage_alpha84(int id){volatile int dummy = id; __asm__ volatile(""::"r"(id):"memory");};
__attribute__((noinline, used, noipa, optnone, optimize("-O0"))) void  mage_alpha85(int id){volatile int dummy = id; __asm__ volatile(""::"r"(id):"memory");};
__attribute__((noinline, used, noipa, optnone, optimize("-O0"))) void  mage_alpha86(int id){volatile int dummy = id; __asm__ volatile(""::"r"(id):"memory");};
__attribute__((noinline, used, noipa, optnone, optimize("-O0"))) void  mage_alpha87(int id){volatile int dummy = id; __asm__ volatile(""::"r"(id):"memory");};
__attribute__((noinline, used, noipa, optnone, optimize("-O0"))) void  mage_alpha88(int id){volatile int dummy = id; __asm__ volatile(""::"r"(id):"memory");};
__attribute__((noinline, used, noipa, optnone, optimize("-O0"))) void  mage_alpha89(int id){volatile int dummy = id; __asm__ volatile(""::"r"(id):"memory");};
__attribute__((noinline, used, noipa, optnone, optimize("-O0"))) void  mage_alpha90(int id){volatile int dummy = id; __asm__ volatile(""::"r"(id):"memory");};
__attribute__((noinline, used, noipa, optnone, optimize("-O0"))) void  mage_alpha91(int id){volatile int dummy = id; __asm__ volatile(""::"r"(id):"memory");};
__attribute__((noinline, used, noipa, optnone, optimize("-O0"))) void  mage_alpha92(int id){volatile int dummy = id; __asm__ volatile(""::"r"(id):"memory");};
__attribute__((noinline, used, noipa, optnone, optimize("-O0"))) void  mage_alpha93(int id){volatile int dummy = id; __asm__ volatile(""::"r"(id):"memory");};
__attribute__((noinline, used, noipa, optnone, optimize("-O0"))) void  mage_alpha94(int id){volatile int dummy = id; __asm__ volatile(""::"r"(id):"memory");};
__attribute__((noinline, used, noipa, optnone, optimize("-O0"))) void  mage_alpha95(int id){volatile int dummy = id; __asm__ volatile(""::"r"(id):"memory");};
__attribute__((noinline, used, noipa, optnone, optimize("-O0"))) void  mage_alpha96(int id){volatile int dummy = id; __asm__ volatile(""::"r"(id):"memory");};
__attribute__((noinline, used, noipa, optnone, optimize("-O0"))) void  mage_alpha97(int id){volatile int dummy = id; __asm__ volatile(""::"r"(id):"memory");};
__attribute__((noinline, used, noipa, optnone, optimize("-O0"))) void  mage_alpha98(int id){volatile int dummy = id; __asm__ volatile(""::"r"(id):"memory");};
__attribute__((noinline, used, noipa, optnone, optimize("-O0"))) void  mage_alpha99(int id){volatile int dummy = id; __asm__ volatile(""::"r"(id):"memory");};
__attribute__((noinline, used, noipa, optnone, optimize("-O0"))) void  mage_alpha100(int id){volatile int dummy = id; __asm__ volatile(""::"r"(id):"memory");};
__attribute__((noinline, used, noipa, optnone, optimize("-O0"))) void  mage_alpha101(int id){volatile int dummy = id; __asm__ volatile(""::"r"(id):"memory");};
#include "../MageAlpha.h"
struct {
  int x;
  int y;
} p = {10, 20};

int t = 3;

__attribute__((optnone)) int main(int argc, char **argv) {

  mage_alpha83(83); 
  struct Point {
    int x;
    int y;
  } p = {10, 20};

  mage_alpha84(84); 
  struct {
    int x;
    int y;
  } r = {10, 20};

  // Simple variable declarations
  mage_alpha85(85); 
  int xx;
  mage_alpha86(86); 
  int x, ee = 2;
  mage_alpha87(87); 
  float y = 3.14;
  mage_alpha88(88); 
  char z = 'A';

  // Multiple declarations
  mage_alpha89(89); 
  int a, b = 5, c;

  // Pointers
  mage_alpha90(90); 
  int *ptr;
  mage_alpha91(91); 
  int *ptr2 = &x;

  // Arrays
  mage_alpha92(92); 
  int arr[10];
  mage_alpha93(93); 
  int matrix[5][5];
  mage_alpha94(94); 
  char str[] = "hello";

  // Const/volatile qualifiers
  mage_alpha95(95); 
  const int READONLY = 100;
  mage_alpha96(96); 
  volatile int status;
  mage_alpha97(97); 
  const char *msg = "constant string";

  // Static/extern
  mage_alpha98(98); 
  static int counter = 0;
  mage_alpha99(99); 
  extern int global_flag;

  // Typedef'd types
  mage_alpha100(100); 
  typedef int *int_ptr;
  mage_alpha101(101); 
  int_ptr pr;
  return 0;
}
