#include <stdio.h>

int main(void) {
  // Example 1: Input validation – repeat until the user enters a valid number.
  // The do-while is essential because we must prompt and read at least once.
  int number;
  do {
    printf("Enter a number between 1 and 10: ");
    scanf("%d", &number);
    if (number < 1 || number > 10) {
      printf("Invalid input. Try again.\n");
    }
  } while (number < 1 || number > 10);
  printf("You entered %d. Good!\n\n", number);

  // Example 2: Accumulate positive numbers until zero or negative is entered.
  // The loop must execute at least once because we need the first number.
  int sum = 0;
  int value;
  do {
    printf("Enter a positive number (0 or negative to stop): ");
    scanf("%d", &value);
    if (value > 0) {
      sum += value;
    }
  } while (value > 0); // stop when value <= 0
  printf("Sum of positive numbers entered: %d\n", sum);

  return 0;
}