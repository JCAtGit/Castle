// functions_example.c

#include <stdio.h>

// Simple function with a single return
int add(int a, int b) { return a + b; }

// Void function with no return
void print_hello(void) { printf("Hello\n"); }

// Function with multiple early returns
int max(int a, int b) {
  if (a > b) {
    return a;
  }
  return b;
}

// Function with a return inside a loop
int sum_to_n(int n) {

  int total = 0;
  for (int i = 1; i <= n; i++) {
    total += i;
  }
  return total;
}

// Function with a nested block and a return
int factorial(int n) {
  if (n <= 1) {
    return 1;
  }
  int result = n * factorial(n - 1);
  return result;
}

// Function with a compound statement but single line inside
void do_nothing(void) {
  ; // null statement
}

// Function with a large compound body (simulated)
void print_multi(int times) {
  for (int i = 0; i < times; i++) {
    printf("%d ", i);
  }
  printf("\n");
}

// Static inline function (C99 or GNU)
static inline int square(int x) { return x * x; }

// Function with a return statement inside a switch
char *weekday_name(int d) {
  switch (d) {
  case 1:
    return "Monday";
  case 2:
    return "Tuesday";
  case 3:
    return "Wednesday";
  case 4:
    return "Thursday";
  case 5:
    return "Friday";
  case 6:
    return "Saturday";
  case 7:
    return "Sunday";
  default:
    return "Invalid";
  }
}

// Function returning void with multiple return statements (C allows void
// returns)
void check_positive(int x) {
  if (x > 0) {
    printf("positive\n");
    return;
  }
  printf("not positive\n");
}

// Main function with parameters and return
int main(int argc, char **argv) {
  printf("add(3,4) = %d\n", add(3, 4));
  print_hello();
  printf("max(5,2) = %d\n", max(5, 2));
  printf("sum_to_n(10) = %d\n", sum_to_n(10));
  printf("factorial(5) = %d\n", factorial(5));
  do_nothing();
  print_multi(5);
  printf("square(6) = %d\n", square(6));
  printf("weekday_name(3) = %s\n", weekday_name(3));
  check_positive(-1);
  return 0;
}
