Sources:

	Eigenschappen die toe te kennen zijn aan constructen, sommige kenmerken zijn specifiek voor bepaalde constructen:
	https://llvm.org/docs/LangRef.html#attribute-groups
	
	Named metadata knopen, wordt gebruikt bij optimization and code generation:
	https://llvm.org/docs/LangRef.html#named-metadata

	LTO:
	https://llvm.org/docs/LinkTimeOptimization.html
	
	ASM Instructions, optimization barriers and mem clobber workings:
	https://gcc.gnu.org/onlinedocs/gcc/Extended-Asm.html
	https://github.com/wookayin/pintos/blob/master/src/threads/synch.h
		
	Checking the state of code using bc/ll before codegen into machine:
	https://github.com/llvm/llvm-project/commit/8e2b3309a975748649a504620a9600da9fe3c837


__attribute__((noinline, optnone, sysv_abi, used,nothrow))

Findings:
	- marked/0/ :
		When using different optimizations settings, the results given by dogbolt differ.
		Dogbolt gives 4 outputs from the following decompilers:
		- angr 9.2.211
		- BinaryNinja 5.3.9434 (f7cf98e7)
		- Ghidra 12.0.1 (0a2b6329)
		- Hex-Rays 9.3.0.260327

Prelink optimization results:
The results when using default setting in the compilation scripts, with optimization -O0, bear the most resemblance from angr. The rest of the decompilers optimize the intermediate statements between the markers. Thus given these observations, the effectivity of markers is somewhat reduced. The flag -O2 during the prelinking phase, seems to optimize the results away, regardless of separate linking procedure.

Postlink optimization results:
TODO: