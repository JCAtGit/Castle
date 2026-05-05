#include "MarkerDefinition.h"

void exampleFn(){}

int main(int argc, const char **argv) {
	__asm__ volatile("" ::: "memory");
	codemarker_alpha(0);
	__asm__ volatile("" ::: "memory");
	int i=0;
	__asm__ volatile("" ::: "memory");
	codemarker_alpha(1);
	__asm__ volatile("" ::: "memory");
	exampleFn();
	return 0;
}
