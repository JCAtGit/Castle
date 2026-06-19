#include "vector"
#include <stdexcept>

using namespace std;

template <typename T>
class Matrix{
	private:
		int rowSize;
		int colSize;
		vector<T> matrixData; // Handles all memory perfectly
		
	public:
	Matrix(int rowSize, int colSize) :
	rowSize(rowSize), colSize(colSize), matrixData(rowSize * colSize, T{})
	{
		
	}
	
	T& at(int row, int col) {
		if (row < 0 || row >= rowSize || col < 0 || col >= colSize) throw std::out_of_range("Index out of bounds");
        return matrixData[row * colSize + col]; // O(1) access
    }
	
	// Const access (read-only)
    const T& at(int row, int col) const {
        return matrixData[row * colSize + col];
    }

    // Getters for dimensions
    int getRows() const { return rowSize; }
    int getCols() const { return colSize; }
};