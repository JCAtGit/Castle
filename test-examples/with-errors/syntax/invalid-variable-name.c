int main() {
    int 123abc;       // Error: starts with digit
    int my-var;       // Error: hyphen not allowed
    int float;        // Error: using keyword
    int _valid;        // Valid (but reserved prefixes exist)
    return 0;
}