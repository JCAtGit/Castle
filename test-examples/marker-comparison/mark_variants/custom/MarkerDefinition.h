#include <stdatomic.h>

#ifndef CODEMARKER_ALPHA
#define CODEMARKER_ALPHA

__attribute__((noinline))
void codemarker_alpha(int markerID) {
	atomic_thread_fence(memory_order_seq_cst);
}
#endif