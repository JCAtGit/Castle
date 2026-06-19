#include <memory>
#include <utility>
#include <stdexcept>

template <typename T>
class Matrix {
public:
    
    Matrix() : rows_(0), cols_(0), data_(nullptr) {}

    // Main constructor: allocates a fixed-size `rows x cols` matrix
    Matrix(size_t rows, size_t cols, const T& initial_value = T{})
        : rows_(rows), cols_(cols), data_(std::make_unique<T[]>(rows * cols)) {
        // Optional: Fill with the initial value
        std::fill(data_.get(), data_.get() + rows * cols, initial_value);
    }

    // ---------- Copy Semantics (Deep Copy) ----------
    // Because we use unique_ptr, we MUST define these manually to deep copy.
    
    Matrix(const Matrix& other)
        : rows_(other.rows_), cols_(other.cols_), 
          data_(std::make_unique<T[]>(other.rows_ * other.cols_)) {
        std::copy(other.data_.get(), other.data_.get() + rows_ * cols_, data_.get());
    }

    Matrix& operator=(const Matrix& other) {
        if (this != &other) {
            // We need to reallocate if the size is different.
            // To keep it "rigid", we could throw an error if sizes mismatch.
            // But for flexibility, we resize to match the other.
            if (rows_ * cols_ != other.rows_ * other.cols_) {
                data_ = std::make_unique<T[]>(other.rows_ * other.cols_);
            }
            rows_ = other.rows_;
            cols_ = other.cols_;
            std::copy(other.data_.get(), other.data_.get() + rows_ * cols_, data_.get());
        }
        return *this;
    }

    // ---------- Move Semantics (Efficient) ----------
    Matrix(Matrix&& other) noexcept = default;
    Matrix& operator=(Matrix&& other) noexcept = default;

    // ---------- Element Access ----------
    
    // Non-const 2D access (read/write)
    T& operator()(size_t row, size_t col) {
        // Optional bounds checking (remove for release builds if you want max speed)
        // if (row >= rows_ || col >= cols_) throw std::out_of_range("Matrix index out of bounds");
        return data_[row * cols_ + col];
    }

    // Const 2D access (read-only)
    const T& operator()(size_t row, size_t col) const {
        // if (row >= rows_ || col >= cols_) throw std::out_of_range("Matrix index out of bounds");
        return data_[row * cols_ + col];
    }

    // Raw pointer access (for integration with unordered_map or C-style APIs)
    T* data() { return data_.get(); }
    const T* data() const { return data_.get(); }

    // ---------- Getters / Utilities ----------
    size_t rows() const { return rows_; }
    size_t cols() const { return cols_; }
    size_t size() const { return rows_ * cols_; } // Total number of elements

    // Fill entire matrix with a value
    void fill(const T& value) {
        std::fill(data_.get(), data_.get() + rows_ * cols_, value);
    }

    // Swap two matrices
    void swap(Matrix& other) noexcept {
        std::swap(rows_, other.rows_);
        std::swap(cols_, other.cols_);
        data_.swap(other.data_); // unique_ptr has a built-in swap
    }

private:
    size_t rows_;
    size_t cols_;
    std::unique_ptr<T[]> data_;
};

// ---------- Equality Operators (Optional but useful) ----------
template <typename T>
bool operator==(const Matrix<T>& lhs, const Matrix<T>& rhs) {
    if (lhs.rows() != rhs.rows() || lhs.cols() != rhs.cols()) return false;
    for (size_t i = 0; i < lhs.size(); ++i) {
        if (lhs.data()[i] != rhs.data()[i]) return false;
    }
    return true;
}