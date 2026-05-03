# Documentation
https://github.com/dtolnay/clang-ast/blob/master/README.md
https://clang.llvm.org/doxygen/annotated.html

## Note
-cc1, 				frontend-only 
-Xclang, 			driver
-fno-macro-expand,	leaves macros in ast-tree

## Generate AST and compile from c-file, examples
clang-check -ast-dump ..somepath/filename.c
clang-check -ast-dump ..somepath/filename.c > ..somepath/filename_ast.c

## Generate AST, syntax and ast generation only, no compilation.
clang -Xclang -ast-dump file_name.c
clang -Xclang -ast-dump -fsyntax-only file.cpp 
clang -Xclang -ast-dump -fsyntax-only example0.c -I /usr/include/c++/15/ 

**These commands dump to terminal, no colours: ** <br/>
clang -Xclang -ast-dump -fsyntax-only -fno-color-diagnostics testfile0.c > testfile0_ast.txt
clang -Xclang -ast-dump -fsyntax-only -fno-color-diagnostics -Xclang -ast-dump file.cpp
clang -Xclang -ast-dump -fsyntax-only -fno-color-diagnostics -Xclang -ast-dump-filter=main file.cpp

**These commands dump ast to files: ** <br/> 	<-- use this
clang -Xclang -ast-dump -fsyntax-only -fno-color-diagnostics -Xclang -ast-dump-filter=main testfile0.c > testfile0_ast.txt
clang -Xclang -ast-dump -fsyntax-only -fno-color-diagnostics -Xclang -ast-dump testfile0.c > testfile0_ast.txt

clang -Xclang -ast-dump -fsyntax-only -fno-color-diagnostics -Wdisabled-macro-expansion -Xclang -ast-dump testfile0.c > testfile0_ast.txt

## Generate visual CFG from c-file, examples
clang --analyze -Xclang -analyzer-checker=debug.ViewCFG yourfile.c
clang --analyze -Xclang -analyzer-checker=debug.DumpCFG -o /dev/null yourfile.c > cfg_output.txt
clang --analyze -Xclang -analyzer-checker=debug.DumpCFG variadic_example.c > output.txt 2>&1

## Clang AST for pseudo C
clang -Xclang -ast-dump -fsyntax-only -fno-color-diagnostics -Xclang -frecovery-ast your_source.cpp > testfile_ast.txt

