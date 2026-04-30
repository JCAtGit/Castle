// 1. Signed integer overflow
int overflow() {
    int x = 2147483647;
    return x + 1;  // Compiles with NO warning! Results in -2147483648
}

// 2. Uninitialized variable
int uninit() {
    int x;  // Compiles with NO warning in many compilers
    return x;  // Returns garbage
}

// 3. Buffer overflow with no warning
void buffer_overflow() {
    char buffer[10];
    strcpy(buffer, "This string is way too long!");  // No warning!
}

// 4. Memory leak
void leak() {
    malloc(1000);  // No warning! Memory leaked
    // No free
}

// 5. Shift beyond bit width
int shift() {
    int x = 1;
    return x << 35;  // Undefined behavior, but compiles cleanly
}