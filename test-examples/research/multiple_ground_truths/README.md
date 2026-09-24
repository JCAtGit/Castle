# example code; source, llvm, asm and binary

clang -O0 -save-temps add.c -o out.bin
clang -S -emit-llvm -O0 add.c
 
xxd -b -p out.bin | tr -d '\n ' > output.txt

## ghidra decompile script
/home/user/Desktop/CCastle/CCastleData/DeComScaffolder/scripts/ghidra_decompile.sh