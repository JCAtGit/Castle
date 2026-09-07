Change to dir of the test file within cmd and then:

Results in AST:
clang -w -fno-color-diagnostics -Xclang -ast-dump -fsyntax-only test.c > testc_ast.txt

Results in "empty" AST:
clang -w -fno-color-diagnostics -Xclang -ast-dump -fsyntax-only test.py > testpy_ast.txt