int main() {
    int x = 5;
    int y = 10;
    int z = x  y;      // Error: missing operator between x and y
    int w = x +;       // Error: missing operand after +
    
    if (x = 5) {       // Warning: assignment instead of comparison
        // This is syntactically valid but often logical error
    }
    
    return 0;
}