#include <stdio.h>

void func_sideeffect0(int x) {
    printf("sideeffect0: %d\n", x);
}

void func_sideeffect1(int x) {
    printf("sideeffect1: %d\n", x);
}

int main() {
    func_sideeffect0(0);
    int i;                     // Uninitialized, reserved on stack
    func_sideeffect1(1);
    int print_hash_value = 0;  // Initialized here
    return 0;
}