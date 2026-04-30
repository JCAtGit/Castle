int process(int input) {
  int multiplier = 2;
  int threshold = 10;
  int result = 0;

  for (int i = 0; i < input; i++) {
    int temp = i * multiplier; // uses outer multiplier
    if (temp > threshold) {
      // nested scope inside if
      int extra = temp + 5;
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
  int result = process(3);
  return result;
}
