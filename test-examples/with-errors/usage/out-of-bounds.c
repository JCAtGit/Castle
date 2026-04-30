int main() {
    int arr[10];
    arr[20] = 5;  // ⚠️ Compiles with warning
    printf("%d\n", arr[20]);  // Might work... until it doesn't
    return 0;
}