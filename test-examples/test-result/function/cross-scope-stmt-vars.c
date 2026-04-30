#include "../MarkerDefinition.h"
int process(int input) {
  codemarker_alpha(0); 
  int multiplier = 2;
  codemarker_alpha(1); 
  int threshold = 10;
  codemarker_alpha(2); 
  int result = 0;

  codemarker_alpha(3); 
  for (int i = 0; i < input; i++) {
    codemarker_alpha(4); 
    int temp = i * multiplier; // uses outer multiplier
    codemarker_alpha(5); 
    if (temp > threshold) {
      // nested scope inside if
      codemarker_alpha(6); 
      int extra = temp + 5;
      codemarker_alpha(7); 
      if (extra > 20) {
        return extra; // nested return inside two levels
      }
      result += extra;
    }
    result += temp;
  }

  return result;
}

int main() {
  codemarker_alpha(8); 
  int result = process(3);
  return result;
}
