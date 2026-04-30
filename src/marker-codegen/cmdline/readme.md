# De cmd folder handelt de program aanroep af samen met de meegeleverde argumenten en flags.

## Link mechanism
// Everything here is clang specific.
// For reference, this is the kind of command sequence that build_executable does:
// clang .\source_1.c -c -emit-llvm
// clang .\source_2.c -c -emit-llvm
// llvm-link .\source_1.bc .\source_2.bc -o merged.bc
// llvm-dis merged.bc -o human_readable.ll
// clang merged.bc -o exe.exe

individual files -[clang -c for each file]-> individual .bc bitcode-files -[]>


## How to Link a Large Number of Files
To bypass this limitation, use a response file. This technique allows you to pass a long list of files via a file, not the command line.

Create a text file (e.g., files.txt) listing the bitcode files, one per line.
Pass the file to llvm-link using the @ syntax:
bash
llvm-link @files.txt -o output.bc