#ifndef CUBE_MATRIX_H
#define CUBE_MATRIX_H

#include <vector>
#include <ostream>
#include <iomanip>

template<typename T>
class Matrix{
private:
    std::vector<std::vector<T>> data;
    int num_lines;
    int num_columns;

public:
    Matrix(int n, int m) : num_lines(n), num_columns(m) {
        data = std::vector(num_lines, std::vector(num_columns, T()));
    }

    explicit Matrix(const std::vector<std::vector<T>> &data) : data(data) {
        num_lines = data.size();
        num_columns = data[0].size();
    }

    void set_cell(size_t i, size_t j, T x){
        data[i][j] = x;
    }

    const std::vector<std::vector<T>> &getData() const {
        return data;
    }

    [[nodiscard]] int get_num_lines() const {
        return num_lines;
    }

    [[nodiscard]] int get_num_columns() const {
        return num_columns;
    }

    bool operator==(const Matrix &rhs) const {
        if (this->num_lines != rhs.num_lines or this->num_columns != rhs.num_columns){
            return false;
        }
        else{
            for (int i = 0; i < num_lines; ++i) {
                for (int j = 0; j < num_columns; ++j) {
                    if (this->data[i][j] != rhs.data[i][j])
                        return false;
                }
            }
            return true;
        }
    }

    bool operator!=(const Matrix &rhs) const {
        return not (rhs == *this);
    }

    std::vector<T> operator[] (size_t i) const{
        return this->data[i];
    }

    template<typename  U>
    /**
     *
     * @tparam U we suppose that the type T can be convert in type U
     * @param other
     * @return the matrix this + other
     */
    Matrix<U> operator+(Matrix<U> const& other){
        Matrix<U> res(num_lines, num_columns);
        for (int i = 0; i < num_lines; ++i) {
            for (int j = 0; j < num_columns; ++j) {
                res.set_cell(i, j, (*this)[i][j] + other[i][j]);
            }
        }
        return res;
    }

    template<typename U>
    /**
     *
     * @tparam U we suppose that the type T can be convert in type U
     * @param x a scalar number
     * @return the matrix this * x
     */
    Matrix<U> operator*(U x){
        Matrix<U> res(num_lines, num_columns);
        for (int i = 0; i < num_lines; ++i) {
            for (int j = 0; j < num_columns; ++j) {
                res.set_cell(i, j, (*this)[i][j] * x);
            }
        }
        return res;
    }

    friend std::ostream &operator<<(std::ostream &os, const Matrix &matrix) {
        std::ios state(nullptr);
        for (int i = 0; i < matrix.num_lines; ++i) {
            for (int j = 0; j < matrix.num_columns; ++j) {
                if (j != 0)
                    os << " ";
                state.copyfmt(os);
                os << std::setw(5) << std::setfill(' ') << matrix[i][j];
                os.copyfmt(state);
            }
            os << std::endl;
        }
        return os;
    }
};

#endif //CUBE_MATRIX_H
