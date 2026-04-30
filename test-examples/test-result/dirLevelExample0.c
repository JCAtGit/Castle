#include "MarkerDefinition.h"
// functions_example.c

#include <stdio.h>

// Simple function with a single return
int add(int a, int b) { return a + b; }

// Void function with no return
void print_hello(void) { codemarker_alpha(0); 
printf("Hello\n"); }

// Function with multiple early returns
int max(int a, int b) {
  codemarker_alpha(1); 
  if (a > b) {
    return a;
  }
  return b;
}

// Function with a return inside a loop
int sum_to_n(int n) {

  codemarker_alpha(2); 
  int total = 0;
  codemarker_alpha(3); 
  for (int i = 1; i <= n; i++) {
    total += i;
  }
  return total;
}

// Function with a nested block and a return
int factorial(int n) {
  codemarker_alpha(4); 
  if (n <= 1) {
    return 1;
  }
  codemarker_alpha(5); 
  int result = n * factorial(n - 1);
  return result;
}

// Function with a compound statement but single line inside
void do_nothing(void) {
  ; // null statement
}

// Function with a large compound body (simulated)
void print_multi(int times) {
  codemarker_alpha(6); 
  for (int i = 0; i < times; i++) {
    codemarker_alpha(7); 
    printf("%d ", i);
  }
  codemarker_alpha(8); 
  printf("\n");
}

// Static inline function (C99 or GNU)
static inline int square(int x) { return x * x; }

// Function with a return statement inside a switch
char *weekday_name(int d) {
  codemarker_alpha(9); 
  switch (d) {
  codemarker_alpha(10); 
  case 1:
    return "Monday";
  codemarker_alpha(11); 
  case 2:
    return "Tuesday";
  codemarker_alpha(12); 
  case 3:
    return "Wednesday";
  codemarker_alpha(13); 
  case 4:
    return "Thursday";
  codemarker_alpha(14); 
  case 5:
    return "Friday";
  codemarker_alpha(15); 
  case 6:
    return "Saturday";
  codemarker_alpha(16); 
  case 7:
    return "Sunday";
  default:
    return "Invalid";
  }
}

// Function returning void with multiple return statements (C allows void
// returns)
void check_positive(int x) {
  codemarker_alpha(17); 
  if (x > 0) {
    codemarker_alpha(18); 
    printf("positive\n");
    return;
  }
  codemarker_alpha(19); 
  printf("not positive\n");
}

// Main function with parameters and return
int main(int argc, char **argv) {
  codemarker_alpha(20); 
  printf("add(3,4) = %d\n", add(3, 4));
  codemarker_alpha(21); 
  print_hello();
  codemarker_alpha(22); 
  printf("max(5,2) = %d\n", max(5, 2));
  codemarker_alpha(23); 
  printf("sum_to_n(10) = %d\n", sum_to_n(10));
  codemarker_alpha(24); 
  printf("factorial(5) = %d\n", factorial(5));
  codemarker_alpha(25); 
  do_nothing();
  codemarker_alpha(26); 
  print_multi(5);
  codemarker_alpha(27); 
  printf("square(6) = %d\n", square(6));
  codemarker_alpha(28); 
  printf("weekday_name(3) = %s\n", weekday_name(3));
  codemarker_alpha(29); 
  check_positive(-1);
  return 0;
}
