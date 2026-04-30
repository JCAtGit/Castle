extern void codemarker_alpha(int markerID);

void fnC(){
	codemarker_alpha(0); 
	int C = 0;
}


void fnB(){
	codemarker_alpha(1); 
	int B = 0;
	codemarker_alpha(2); 
	fnC();
}

void fnA(){
	codemarker_alpha(3); 
	int A = 0;
	codemarker_alpha(4); 
	fnB();
}


int main(int argc, char **argv) {
	codemarker_alpha(5); 
	fnA();
}