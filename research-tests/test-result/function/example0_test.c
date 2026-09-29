#include <stdio.h>
#include "../../../research-tests/test-result/MageAlpha.h"

// Simple function with a single return
__attribute__((optnone)) int add(int a, int b) { return a + b; }

// Void function with no return
__attribute__((optnone)) void print_hello(void) { mage_alpha(62); 
printf("Hello\n"); }

// Function with multiple early returns
__attribute__((optnone)) int max(int a, int b) {
  mage_alpha(63); 
  if (a > b) {
    return a;
  }
  return b;
}

// Function with a return inside a loop
__attribute__((optnone)) int sum_to_n(int n) {

  mage_alpha(64); 
  int total = 0;
  mage_alpha(65); 
  for (int i = 1; i <= n; i++) {
    total += i;
  }
  return total;
}

// Function with a nested block and a return
__attribute__((optnone)) int factorial(int n) {
  mage_alpha(66); 
  if (n <= 1) {
    return 1;
  }
  mage_alpha(67); 
  int result = n * factorial(n - 1);
  return result;
}

// Function with a compound statement but single line inside
__attribute__((optnone)) void do_nothing(void) {
  ; // null statement
}

// Function with a large compound body (simulated)
__attribute__((optnone)) void print_multi(int times) {
  mage_alpha(68); 
  for (int i = 0; i < times; i++) {
    mage_alpha(69); 
    printf("%d ", i);
  }
  mage_alpha(70); 
  printf("\n");
}

// Static inline function (C99 or GNU)
__attribute__((optnone)) static inline int square(int x) { return x * x; }

// Function with a return statement inside a switch
__attribute__((optnone)) char *weekday_name(int d) {
  mage_alpha(71); 
  switch (d) {
  mage_alpha(72); 
  case 1:
    return "Monday";
  mage_alpha(73); 
  case 2:
    return "Tuesday";
  mage_alpha(74); 
  case 3:
    return "Wednesday";
  mage_alpha(75); 
  case 4:
    return "Thursday";
  mage_alpha(76); 
  case 5:
    return "Friday";
  mage_alpha(77); 
  case 6:
    return "Saturday";
  mage_alpha(78); 
  case 7:
    return "Sunday";
  default:
    return "Invalid";
  }
}

// Function returning void with multiple return statements (C allows void
// returns)
__attribute__((optnone)) void check_positive(int x) {
  mage_alpha(79); 
  if (x > 0) {
    mage_alpha(80); 
    printf("positive\n");
    return;
  }
  mage_alpha(81); 
  printf("not positive\n");
}

// Main function with parameters and return
__attribute__((optnone)) int main(int argc, char **argv) {
  mage_alpha(82); 
  printf("add(3,4) = %d\n", add(3, 4));
  mage_alpha(83); 
  print_hello();
  mage_alpha(84); 
  printf("max(5,2) = %d\n", max(5, 2));
  mage_alpha(85); 
  printf("sum_to_n(10) = %d\n", sum_to_n(10));
  mage_alpha(86); 
  printf("factorial(5) = %d\n", factorial(5));
  mage_alpha(87); 
  do_nothing();
  mage_alpha(88); 
  print_multi(5);
  mage_alpha(89); 
  printf("square(6) = %d\n", square(6));
  mage_alpha(90); 
  printf("weekday_name(3) = %s\n", weekday_name(3));
  mage_alpha(91); 
  check_positive(-1);
  return 0;
}
