
void fnC(){
	int C = 0;
}


void fnB(){
	int B = 0;
	fnC();
}

void fnA(){
	int A = 0;
	fnB();
}


int main(int argc, char **argv) {
	fnA();
}