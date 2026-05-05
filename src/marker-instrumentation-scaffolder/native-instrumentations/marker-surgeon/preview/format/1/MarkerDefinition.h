#ifndef CODEMARKER_ALPHA
#define CODEMARKER_ALPHA
__attribute__((noinline))
void codemarker_alpha(int markerID) {
	volatile int a = markerID;
}
#endif
