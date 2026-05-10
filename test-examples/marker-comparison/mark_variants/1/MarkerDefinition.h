#ifndef CODEMARKER_ALPHA
#define CODEMARKER_ALPHA
__attribute__((noinline, used, nothrow, nofree, noipa, optnone))
static void codemarker_alpha(int markerID) {
    volatile int dummy = markerID;
	__asm__ volatile("" :: "r"(markerID) : "memory");

}
#endif