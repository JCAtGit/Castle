__attribute__((noinline, used, noipa, optnone, optimize("-O0"))) void  mage_alpha41(int id){volatile int dummy = id; __asm__ volatile(""::"r"(id):"memory");};
__attribute__((noinline, used, noipa, optnone, optimize("-O0"))) void  mage_alpha42(int id){volatile int dummy = id; __asm__ volatile(""::"r"(id):"memory");};
__attribute__((noinline, used, noipa, optnone, optimize("-O0"))) void  mage_alpha43(int id){volatile int dummy = id; __asm__ volatile(""::"r"(id):"memory");};
__attribute__((noinline, used, noipa, optnone, optimize("-O0"))) void  mage_alpha44(int id){volatile int dummy = id; __asm__ volatile(""::"r"(id):"memory");};
__attribute__((noinline, used, noipa, optnone, optimize("-O0"))) void  mage_alpha45(int id){volatile int dummy = id; __asm__ volatile(""::"r"(id):"memory");};
__attribute__((noinline, used, noipa, optnone, optimize("-O0"))) void  mage_alpha46(int id){volatile int dummy = id; __asm__ volatile(""::"r"(id):"memory");};
__attribute__((noinline, used, noipa, optnone, optimize("-O0"))) void  mage_alpha47(int id){volatile int dummy = id; __asm__ volatile(""::"r"(id):"memory");};
__attribute__((noinline, used, noipa, optnone, optimize("-O0"))) void  mage_alpha48(int id){volatile int dummy = id; __asm__ volatile(""::"r"(id):"memory");};
__attribute__((noinline, used, noipa, optnone, optimize("-O0"))) void  mage_alpha49(int id){volatile int dummy = id; __asm__ volatile(""::"r"(id):"memory");};
__attribute__((noinline, used, noipa, optnone, optimize("-O0"))) void  mage_alpha50(int id){volatile int dummy = id; __asm__ volatile(""::"r"(id):"memory");};
__attribute__((noinline, used, noipa, optnone, optimize("-O0"))) void  mage_alpha51(int id){volatile int dummy = id; __asm__ volatile(""::"r"(id):"memory");};
__attribute__((noinline, used, noipa, optnone, optimize("-O0"))) void  mage_alpha52(int id){volatile int dummy = id; __asm__ volatile(""::"r"(id):"memory");};
#include "../MageAlpha.h"
#include <stdio.h>

int __attribute__((optimize("O0"), optnone)) main(void) {
  // Example 1: Input validation – repeat until the user enters a valid number.
  // The do-while is essential because we must prompt and read at least once.
  mage_alpha41(41); 
  int number;
  do {
    mage_alpha42(42); 
    printf("Enter a number between 1 and 10: ");
    mage_alpha43(43); 
    scanf("%d", &number);
    mage_alpha44(44); 
    if (number < 1 || number > 10) {
      mage_alpha45(45); 
      printf("Invalid input. Try again.\n");
    }
  } while (number < 1 || number > 10);
  mage_alpha46(46); 
  printf("You entered %d. Good!\n\n", number);

  // Example 2: Accumulate positive numbers until zero or negative is entered.
  // The loop must execute at least once because we need the first number.
  mage_alpha47(47); 
  int sum = 0;
  mage_alpha48(48); 
  int value;
  do {
    mage_alpha49(49); 
    printf("Enter a positive number (0 or negative to stop): ");
    mage_alpha50(50); 
    scanf("%d", &value);
    mage_alpha51(51); 
    if (value > 0) {
      sum += value;
    }
  } while (value > 0); // stop when value <= 0
  mage_alpha52(52); 
  printf("Sum of positive numbers entered: %d\n", sum);

  return 0;
}
