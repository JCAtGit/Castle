TODO: 
	Consider Escaped Addresses to prevent constant-folding, example:
	
	int main() {
	    int x = 10;          // Without escape, compiler folds x to 10.
	    __asm__ volatile("" : : "r"(&x) : "memory");
	    int y = x + 5;       // Now x must be loaded from memory (runtime value)
	    return y;
	}
	
	