# Goal
Some manual tests are performed here with to goal to investigate the influence of compiler algorithms on decompiler results.

## Findings
The attributes seem to largely preserve the statements when inspecting the llvm-ir, however there seem to be some exceptions.
Despite the documentation stating optnone to block all optimizations we find some still happen regardless (https://clang.llvm.org/docs/AttributeReference.html#optnone).
We consider such anomalies to be a pipeline artifacts.
Such anomalies are detected by mage instrumentations and can be used to provide feedback on compilers as well. 
However pure decompiler benchmarking does suffer due to compilers making such choices.

