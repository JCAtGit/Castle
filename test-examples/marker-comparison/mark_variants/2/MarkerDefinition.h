#ifndef CODEMARKER_ALPHA
#define CODEMARKER_ALPHA
__attribute__((noinline, used, nothrow, norecurse, noclone, nofree, noipa))
void codemarker_alpha(int markerID) {
	__asm__ volatile("" : : "r"(markerID) : "memory");
}
#endif
