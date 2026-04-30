#include "../MarkerDefinition.h"
// switch_examples.c

#include <stdio.h>

enum Color { RED, GREEN, BLUE };

void simple_switch(int x) {
  codemarker_alpha(0); 
  switch (x) {
  codemarker_alpha(1); 
  case 1:
    printf("one\n");
    break;
  codemarker_alpha(2); 
  case 2:
    printf("two\n");
    break;
  default:
    printf("other\n");
    break;
  }
}

void switch_with_fallthrough(int x) {
  codemarker_alpha(3); 
  switch (x) {
  codemarker_alpha(4); 
  case 1:
    printf("one ");
    // fall through
  codemarker_alpha(5); 
  case 2:
    printf("two\n");
    break;
  codemarker_alpha(6); 
  case 3:
    printf("three\n");
    break;
  }
}

void nested_switch(int a, int b) {
  codemarker_alpha(7); 
  switch (a) {
  codemarker_alpha(8); 
  case 1:
    codemarker_alpha(9); 
    switch (b) {
    codemarker_alpha(10); 
    case 10:
      printf("a=1, b=10\n");
      break;
    default:
      printf("a=1, other b\n");
      break;
    }
    break;
  codemarker_alpha(11); 
  case 2:
    printf("a=2\n");
    break;
  default:
    printf("other a\n");
    break;
  }
}

void switch_with_enum(enum Color c) {
  codemarker_alpha(12); 
  switch (c) {
  codemarker_alpha(13); 
  case RED:
    printf("red\n");
    break;
  codemarker_alpha(14); 
  case GREEN:
    printf("green\n");
    break;
  codemarker_alpha(15); 
  case BLUE:
    printf("blue\n");
    break;
  }
}

void switch_in_loop(int n) {
  codemarker_alpha(16); 
  for (int i = 0; i < n; i++) {
    codemarker_alpha(17); 
    switch (i % 3) {
    codemarker_alpha(18); 
    case 0:
      printf("zero ");
      break;
    codemarker_alpha(19); 
    case 1:
      printf("one ");
      break;
    default:
      printf("other ");
      break;
    }
  }
  codemarker_alpha(20); 
  printf("\n");
}

void switch_with_empty_case(int x) {
  codemarker_alpha(21); 
  switch (x) {
  codemarker_alpha(22); 
  case 1:
  codemarker_alpha(23); 
  case 2:
    printf("one or two\n");
    break;
  codemarker_alpha(24); 
  case 3:
    printf("three\n");
    break;
  default:
    printf("default\n");
    break;
  }
}

void switch_with_compound_statement(int x) {
  codemarker_alpha(25); 
  switch (x) {
  codemarker_alpha(26); 
  case 1: {
    codemarker_alpha(27); 
    int y = x + 10;
    codemarker_alpha(28); 
    printf("case 1: y = %d\n", y);
    break;
  }
  codemarker_alpha(29); 
  case 2:
    printf("case 2\n");
    break;
  default: {
    codemarker_alpha(30); 
    printf("default\n");
    break;
  }
  }
}

int main() {
  codemarker_alpha(31); 
  simple_switch(2);
  codemarker_alpha(32); 
  switch_with_fallthrough(1);
  codemarker_alpha(33); 
  nested_switch(1, 10);
  codemarker_alpha(34); 
  switch_with_enum(GREEN);
  codemarker_alpha(35); 
  switch_in_loop(5);
  codemarker_alpha(36); 
  switch_with_empty_case(2);
  codemarker_alpha(37); 
  switch_with_compound_statement(1);
  return 0;
}
