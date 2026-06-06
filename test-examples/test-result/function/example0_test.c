__attribute__((noinline, used, noipa, optnone, optimize("-O0"))) void  mage_alpha53(int id){volatile int dummy = id; __asm__ volatile(""::"r"(id):"memory");};
__attribute__((noinline, used, noipa, optnone, optimize("-O0"))) void  mage_alpha54(int id){volatile int dummy = id; __asm__ volatile(""::"r"(id):"memory");};
__attribute__((noinline, used, noipa, optnone, optimize("-O0"))) void  mage_alpha55(int id){volatile int dummy = id; __asm__ volatile(""::"r"(id):"memory");};
__attribute__((noinline, used, noipa, optnone, optimize("-O0"))) void  mage_alpha56(int id){volatile int dummy = id; __asm__ volatile(""::"r"(id):"memory");};
__attribute__((noinline, used, noipa, optnone, optimize("-O0"))) void  mage_alpha57(int id){volatile int dummy = id; __asm__ volatile(""::"r"(id):"memory");};
__attribute__((noinline, used, noipa, optnone, optimize("-O0"))) void  mage_alpha58(int id){volatile int dummy = id; __asm__ volatile(""::"r"(id):"memory");};
__attribute__((noinline, used, noipa, optnone, optimize("-O0"))) void  mage_alpha59(int id){volatile int dummy = id; __asm__ volatile(""::"r"(id):"memory");};
__attribute__((noinline, used, noipa, optnone, optimize("-O0"))) void  mage_alpha60(int id){volatile int dummy = id; __asm__ volatile(""::"r"(id):"memory");};
__attribute__((noinline, used, noipa, optnone, optimize("-O0"))) void  mage_alpha61(int id){volatile int dummy = id; __asm__ volatile(""::"r"(id):"memory");};
__attribute__((noinline, used, noipa, optnone, optimize("-O0"))) void  mage_alpha62(int id){volatile int dummy = id; __asm__ volatile(""::"r"(id):"memory");};
__attribute__((noinline, used, noipa, optnone, optimize("-O0"))) void  mage_alpha63(int id){volatile int dummy = id; __asm__ volatile(""::"r"(id):"memory");};
__attribute__((noinline, used, noipa, optnone, optimize("-O0"))) void  mage_alpha64(int id){volatile int dummy = id; __asm__ volatile(""::"r"(id):"memory");};
__attribute__((noinline, used, noipa, optnone, optimize("-O0"))) void  mage_alpha65(int id){volatile int dummy = id; __asm__ volatile(""::"r"(id):"memory");};
__attribute__((noinline, used, noipa, optnone, optimize("-O0"))) void  mage_alpha66(int id){volatile int dummy = id; __asm__ volatile(""::"r"(id):"memory");};
__attribute__((noinline, used, noipa, optnone, optimize("-O0"))) void  mage_alpha67(int id){volatile int dummy = id; __asm__ volatile(""::"r"(id):"memory");};
__attribute__((noinline, used, noipa, optnone, optimize("-O0"))) void  mage_alpha68(int id){volatile int dummy = id; __asm__ volatile(""::"r"(id):"memory");};
__attribute__((noinline, used, noipa, optnone, optimize("-O0"))) void  mage_alpha69(int id){volatile int dummy = id; __asm__ volatile(""::"r"(id):"memory");};
__attribute__((noinline, used, noipa, optnone, optimize("-O0"))) void  mage_alpha70(int id){volatile int dummy = id; __asm__ volatile(""::"r"(id):"memory");};
__attribute__((noinline, used, noipa, optnone, optimize("-O0"))) void  mage_alpha71(int id){volatile int dummy = id; __asm__ volatile(""::"r"(id):"memory");};
__attribute__((noinline, used, noipa, optnone, optimize("-O0"))) void  mage_alpha72(int id){volatile int dummy = id; __asm__ volatile(""::"r"(id):"memory");};
__attribute__((noinline, used, noipa, optnone, optimize("-O0"))) void  mage_alpha73(int id){volatile int dummy = id; __asm__ volatile(""::"r"(id):"memory");};
__attribute__((noinline, used, noipa, optnone, optimize("-O0"))) void  mage_alpha74(int id){volatile int dummy = id; __asm__ volatile(""::"r"(id):"memory");};
__attribute__((noinline, used, noipa, optnone, optimize("-O0"))) void  mage_alpha75(int id){volatile int dummy = id; __asm__ volatile(""::"r"(id):"memory");};
__attribute__((noinline, used, noipa, optnone, optimize("-O0"))) void  mage_alpha76(int id){volatile int dummy = id; __asm__ volatile(""::"r"(id):"memory");};
__attribute__((noinline, used, noipa, optnone, optimize("-O0"))) void  mage_alpha77(int id){volatile int dummy = id; __asm__ volatile(""::"r"(id):"memory");};
__attribute__((noinline, used, noipa, optnone, optimize("-O0"))) void  mage_alpha78(int id){volatile int dummy = id; __asm__ volatile(""::"r"(id):"memory");};
__attribute__((noinline, used, noipa, optnone, optimize("-O0"))) void  mage_alpha79(int id){volatile int dummy = id; __asm__ volatile(""::"r"(id):"memory");};
__attribute__((noinline, used, noipa, optnone, optimize("-O0"))) void  mage_alpha80(int id){volatile int dummy = id; __asm__ volatile(""::"r"(id):"memory");};
__attribute__((noinline, used, noipa, optnone, optimize("-O0"))) void  mage_alpha81(int id){volatile int dummy = id; __asm__ volatile(""::"r"(id):"memory");};
__attribute__((noinline, used, noipa, optnone, optimize("-O0"))) void  mage_alpha82(int id){volatile int dummy = id; __asm__ volatile(""::"r"(id):"memory");};
#include "../MageAlpha.h"
// functions_example.c

#include <stdio.h>

// Simple function with a single return
__attribute__((optnone)) int add(int a, int b) { return a + b; }

// Void function with no return
__attribute__((optnone)) void print_hello(void) { mage_alpha53(53); 
printf("Hello\n"); }

// Function with multiple early returns
__attribute__((optnone)) int max(int a, int b) {
  mage_alpha54(54); 
  if (a > b) {
    return a;
  }
  return b;
}

// Function with a return inside a loop
__attribute__((optnone)) int sum_to_n(int n) {

  mage_alpha55(55); 
  int total = 0;
  mage_alpha56(56); 
  for (int i = 1; i <= n; i++) {
    total += i;
  }
  return total;
}

// Function with a nested block and a return
__attribute__((optnone)) int factorial(int n) {
  mage_alpha57(57); 
  if (n <= 1) {
    return 1;
  }
  mage_alpha58(58); 
  int result = n * factorial(n - 1);
  return result;
}

// Function with a compound statement but single line inside
__attribute__((optnone)) void do_nothing(void) {
  ; // null statement
}

// Function with a large compound body (simulated)
__attribute__((optnone)) void print_multi(int times) {
  mage_alpha59(59); 
  for (int i = 0; i < times; i++) {
    mage_alpha60(60); 
    printf("%d ", i);
  }
  mage_alpha61(61); 
  printf("\n");
}

// Static inline function (C99 or GNU)
__attribute__((optnone)) static inline int square(int x) { return x * x; }

// Function with a return statement inside a switch
__attribute__((optnone)) char *weekday_name(int d) {
  mage_alpha62(62); 
  switch (d) {
  mage_alpha63(63); 
  case 1:
    return "Monday";
  mage_alpha64(64); 
  case 2:
    return "Tuesday";
  mage_alpha65(65); 
  case 3:
    return "Wednesday";
  mage_alpha66(66); 
  case 4:
    return "Thursday";
  mage_alpha67(67); 
  case 5:
    return "Friday";
  mage_alpha68(68); 
  case 6:
    return "Saturday";
  mage_alpha69(69); 
  case 7:
    return "Sunday";
  default:
    return "Invalid";
  }
}

// Function returning void with multiple return statements (C allows void
// returns)
__attribute__((optnone)) void check_positive(int x) {
  mage_alpha70(70); 
  if (x > 0) {
    mage_alpha71(71); 
    printf("positive\n");
    return;
  }
  mage_alpha72(72); 
  printf("not positive\n");
}

// Main function with parameters and return
__attribute__((optnone)) int main(int argc, char **argv) {
  mage_alpha73(73); 
  printf("add(3,4) = %d\n", add(3, 4));
  mage_alpha74(74); 
  print_hello();
  mage_alpha75(75); 
  printf("max(5,2) = %d\n", max(5, 2));
  mage_alpha76(76); 
  printf("sum_to_n(10) = %d\n", sum_to_n(10));
  mage_alpha77(77); 
  printf("factorial(5) = %d\n", factorial(5));
  mage_alpha78(78); 
  do_nothing();
  mage_alpha79(79); 
  print_multi(5);
  mage_alpha80(80); 
  printf("square(6) = %d\n", square(6));
  mage_alpha81(81); 
  printf("weekday_name(3) = %s\n", weekday_name(3));
  mage_alpha82(82); 
  check_positive(-1);
  return 0;
}
