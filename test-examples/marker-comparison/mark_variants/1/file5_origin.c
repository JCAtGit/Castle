#include "MarkerDefinition.h"

void fnC(){
	__asm__ volatile("" ::: "memory");
	codemarker_alpha(0); 
	__asm__ volatile("" ::: "memory");
	int C = 0;
}


void fnB(){
	__asm__ volatile("" ::: "memory");
	codemarker_alpha(1); 
	__asm__ volatile("" ::: "memory");
	int B = 0;
	__asm__ volatile("" ::: "memory");
	codemarker_alpha(2); 
	__asm__ volatile("" ::: "memory");
	fnC();
}

void fnA(){
	__asm__ volatile("" ::: "memory");
	codemarker_alpha(3); 
	__asm__ volatile("" ::: "memory");
	int A = 0;
	__asm__ volatile("" ::: "memory");
	codemarker_alpha(4); 
	__asm__ volatile("" ::: "memory");
	fnB();
}


int main(int argc, char **argv) {
	__asm__ volatile("" ::: "memory");
	codemarker_alpha(5); 
	__asm__ volatile("" ::: "memory");
	fnA();
}

