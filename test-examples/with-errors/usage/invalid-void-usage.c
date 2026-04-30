#include <stdio.h>

int main() {
    void v;           // Cannot declare void variable
    void *ptr = &v;   // Cannot take address of void
    
    void func();      // OK - declares function returning void
    void *vptr;       // OK - void pointer is allowed
    *vptr = 5;        // Cannot dereference void pointer
    
    return 0;
}