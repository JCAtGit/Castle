# Manual test to check for case scope markers.
cd [this dir]
clang test.c <br>
objdump -d a.out | grep mage_alpha <br> 

[comment]: # (We'll see that the compiler omits the case scope marker calls.)

# LLVM-IR file test
clang -S -emit-llvm -O0 test.c -o test_O0.ll

# Clang AST inspection
clang -w -fno-color-diagnostics -Xclang -ast-dump -Xclang -ast-dump-filter=main -fsyntax-only test.c > test_ast.txt


# Clang DAGS inspection
clang -mllvm -view-dag-combine1-dags -O0 -c test.c
