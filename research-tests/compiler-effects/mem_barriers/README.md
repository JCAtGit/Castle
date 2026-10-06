# CMD Reminder

```
clang -S -emit-llvm -O0 noop.c
clang -O0 -save-temps noop.c -o noop_out.bin

clang -S -emit-llvm -O0 baseline.c
clang -O0 -save-temps baseline.c -o baseline_out.bin
```