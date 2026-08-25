# Manual test to check for case scope markers.
cd [this dir]
clang test.c <br>
objdump -d a.out | grep mage_alpha <br> 

[comment]: # (We'll see that the compiler omits the case scope marker calls.)
