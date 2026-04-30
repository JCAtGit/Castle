#include "../MarkerDefinition.h"
#include <stdarg.h>

int sum(int count, ...) {
  va_list args;
  va_start(args, count);
  codemarker_alpha(0); 
  int total = 0;
  codemarker_alpha(1); 
  for (int i = 0; i < count; i++) {
    total += va_arg(args, int);
  }
  va_end(args);
  return total;
}

int main() {
  codemarker_alpha(2); 
  int result = sum(3, 10, 20, 30);
  return result;
}
