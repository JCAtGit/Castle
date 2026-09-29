#include <stdio.h>
#include "../../../research-tests/test-result/MageAlpha.h"

int __attribute__((optimize("O0"), optnone)) main(void) {
  // Example 1: Input validation – repeat until the user enters a valid number.
  // The do-while is essential because we must prompt and read at least once.
  mage_alpha(0); 
  int number;
  do {
    mage_alpha(1); 
    printf("Enter a number between 1 and 10: ");
    mage_alpha(2); 
    scanf("%d", &number);
    mage_alpha(3); 
    if (number < 1 || number > 10) {
      mage_alpha(4); 
      printf("Invalid input. Try again.\n");
    }
  } while (number < 1 || number > 10);
  mage_alpha(5); 
  printf("You entered %d. Good!\n\n", number);

  // Example 2: Accumulate positive numbers until zero or negative is entered.
  // The loop must execute at least once because we need the first number.
  mage_alpha(6); 
  int sum = 0;
  mage_alpha(7); 
  int value;
  do {
    mage_alpha(8); 
    printf("Enter a positive number (0 or negative to stop): ");
    mage_alpha(9); 
    scanf("%d", &value);
    mage_alpha(10); 
    if (value > 0) {
      sum += value;
    }
  } while (value > 0); // stop when value <= 0
  mage_alpha(11); 
  printf("Sum of positive numbers entered: %d\n", sum);

  return 0;
}
