#define MAX(a, b) a > b ? a : b  // Missing parentheses

int main() {
    int x = 5, y = 10;
    int z = MAX(x, y) * 2;  // Expands to: x > y ? x : y * 2
    return 0;
}