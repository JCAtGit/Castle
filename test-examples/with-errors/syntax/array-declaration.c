int main() {
    int arr[];         // Error: array size missing
    int matrix[][];    // Error: only first dimension can be empty
    int values[5] = {1, 2, 3, 4, 5, 6};  // Error: too many initializers
    
    // Correct:
    int arr[5];
    int matrix[3][4];
    int values[5] = {1, 2, 3, 4, 5};
    
    return 0;
}