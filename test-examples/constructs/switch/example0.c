// switch_examples.c

#include <stdio.h>

enum Color { RED, GREEN, BLUE };

void simple_switch(int x) {
  switch (x) {
  case 1:
    printf("one\n");
    break;
  case 2:
    printf("two\n");
    break;
  default:
    printf("other\n");
    break;
  }
}

void switch_with_fallthrough(int x) {
  switch (x) {
  case 1:
    printf("one ");
    // fall through
  case 2:
    printf("two\n");
    break;
  case 3:
    printf("three\n");
    break;
  }
}

void nested_switch(int a, int b) {
  switch (a) {
  case 1:
    switch (b) {
    case 10:
      printf("a=1, b=10\n");
      break;
    default:
      printf("a=1, other b\n");
      break;
    }
    break;
  case 2:
    printf("a=2\n");
    break;
  default:
    printf("other a\n");
    break;
  }
}

void switch_with_enum(enum Color c) {
  switch (c) {
  case RED:
    printf("red\n");
    break;
  case GREEN:
    printf("green\n");
    break;
  case BLUE:
    printf("blue\n");
    break;
  }
}

void switch_in_loop(int n) {
  for (int i = 0; i < n; i++) {
    switch (i % 3) {
    case 0:
      printf("zero ");
      break;
    case 1:
      printf("one ");
      break;
    default:
      printf("other ");
      break;
    }
  }
  printf("\n");
}

void switch_with_empty_case(int x) {
  switch (x) {
  case 1:
  case 2:
    printf("one or two\n");
    break;
  case 3:
    printf("three\n");
    break;
  default:
    printf("default\n");
    break;
  }
}

void switch_with_compound_statement(int x) {
  switch (x) {
  case 1: {
    int y = x + 10;
    printf("case 1: y = %d\n", y);
    break;
  }
  case 2:
    printf("case 2\n");
    break;
  default: {
    printf("default\n");
    break;
  }
  }
}

int main() {
  simple_switch(2);
  switch_with_fallthrough(1);
  nested_switch(1, 10);
  switch_with_enum(GREEN);
  switch_in_loop(5);
  switch_with_empty_case(2);
  switch_with_compound_statement(1);
  return 0;
}
