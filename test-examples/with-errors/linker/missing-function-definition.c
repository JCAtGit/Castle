// file1.c
extern void missingFunction(void);  // Declaration but no definition

int main() {
    missingFunction();  // Linker can't find definition
    return 0;
}