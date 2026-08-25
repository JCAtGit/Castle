
#include "MageAlpha.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

__attribute__ ((
noinline, used, noipa, optnone, optimize("-O0")
))
void mage_alpha83(int id){
volatile int dummy = id;
__asm__ volatile(""::"r"(id):"memory");
};

__attribute__ ((
noinline, used, noipa, optnone, optimize("-O0")
))
void mage_alpha84(int id){
volatile int dummy = id;
__asm__ volatile(""::"r"(id):"memory");
};

__attribute__ ((
noinline, used, noipa, optnone, optimize("-O0")
))
void mage_alpha85(int id){
volatile int dummy = id;
__asm__ volatile(""::"r"(id):"memory");
};

int 
__attribute__((optimize("O0"), optnone))
main (){
  srand(time(NULL));   // Initialization, should only be called once
  int x = rand();
  switch (x) {
  mage_alpha83(83);
  case 1:
  printf("One\n");
  goto label0;
  break;
  mage_alpha84(84);
  label0:
  case 2: printf("Two\n");
  mage_alpha85(85);
  }
};
