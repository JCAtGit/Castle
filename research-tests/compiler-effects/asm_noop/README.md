We check if asm statements acting as memory clobbers act as noops when compiled.

Chandler has a video where he demonstrates the use of a clobber as barrier, but his usecase is for c++ performance benchmarking/tuning:

``` 
https://www.youtube.com/watch?v=nXaxk27zwlk 
```

The principle remains the same however for such barriers statements.


# checking for ops in the llvm-ir, here we can see a mention of the asm statement.

Info on asm statements: 
-	https://gcc.gnu.org/onlinedocs/gcc/Extended-Asm.html
-	https://gcc.gnu.org/onlinedocs/gcc/Extended-Asm.html#Clobbers-and-Scratch-Registers
-	https://gcc.gnu.org/onlinedocs/gcc/Extended-Asm.html#InputOperands, we hebben r gebruikt maar g is ook een optie. Eindeffect is hetzelfde.

```
clang -S -emit-llvm -O0 noop.c
clang -O0 -save-temps noop.c -o noop_out.bin

clang -S -emit-llvm -O0 baseline.c
clang -O0 -save-temps baseline.c -o baseline_out.bin
```
