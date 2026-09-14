#include <stdio.h>

void func_sideeffect0(int x) {
    printf("sideeffect0: %d\n", x);
}

void func_sideeffect1(int x) {
    printf("sideeffect1: %d\n", x);
}

int main() {
    int i;                     // Declared at top, uninitialized
    int print_hash_value;      // Declared at top, uninitialized
    func_sideeffect0(0);
    func_sideeffect1(1);
    print_hash_value = 0;      // Assigned here
    return 0;
}