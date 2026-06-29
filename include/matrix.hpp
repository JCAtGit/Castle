#pragma once
#include "vector"
#include <stdexcept>

using namespace std;


/**
 *	Row x column matrix datastructure.
 */
template <typename T>
class Matrix{
	private:
		int rowSize;
		int colSize;
		vector<T> matrixData;
		
	public:
	Matrix(){}
	Matrix(int rowSize, int colSize):rowSize(rowSize), colSize(colSize), matrixData(rowSize * colSize, T{}){}
	
	T& at(int row, int col) {
		if (row < 0 || row >= rowSize || col < 0 || col >= colSize) throw std::out_of_range("Index out of bounds");
        return matrixData[row * colSize + col];
    }
	
	// Const access (read-only)
    const T& at(int row, int col) const {
        return matrixData[row * colSize + col];
    }

    // Getters for dimensions
    int getRows() const { return rowSize; }
    int getCols() const { return colSize; }
};