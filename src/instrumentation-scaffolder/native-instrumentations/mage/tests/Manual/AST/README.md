Test to observe the effects of unparsable code by clang.
Change to dir of the test file within cmd and then:

Results in AST:
clang -w -fno-color-diagnostics -Xclang -ast-dump -Xclang -ast-dump-filter=main -fsyntax-only example0.c > example0_ast.txt

Results in "empty" AST:
clang -w -fno-color-diagnostics -Xclang -ast-dump -fsyntax-only test.py > testpy_ast.txt