# Clang Format
Links: 
- https://clang.llvm.org/docs/ClangFormat.html

## Adds braces to braceless constructs and potential dangling statements.
To another file: 	clang-format --style="{InsertBraces: true, BasedOnStyle: LLVM}" example0.c > example0_formatted.c
Edit in-place: 		clang-format -i --style="{InsertBraces: true, BasedOnStyle: LLVM}" example0.c