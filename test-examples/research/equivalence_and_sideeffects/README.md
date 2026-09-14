
# LLVM-IR file test
clang -S -emit-llvm -O0 case1.c -o case1_O0.ll
clang -S -emit-llvm -O0 case2.c -o case2_O0.ll
clang -S -emit-llvm -O0 case3.c -o case3_O0.ll
clang -S -emit-llvm -O0 case4.c -o case4_O0.ll

# AST inspection
clang -w -fno-color-diagnostics -Xclang -ast-dump -Xclang -ast-dump-filter=main -fsyntax-only case1.c > case1_ast.txt
clang -w -fno-color-diagnostics -Xclang -ast-dump -Xclang -ast-dump-filter=main -fsyntax-only case2.c > case2_ast.txt
clang -w -fno-color-diagnostics -Xclang -ast-dump -Xclang -ast-dump-filter=main -fsyntax-only case3.c > case3_ast.txt
clang -w -fno-color-diagnostics -Xclang -ast-dump -Xclang -ast-dump-filter=main -fsyntax-only case4.c > case4_ast.txt

# DAGS inspection
clang -mllvm -view-dag-combine1-dags -O0 -c case1.c
