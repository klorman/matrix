#include <iostream>
#include <vector>
#include <cassert>

template <typename T>
struct matrix
{
    int col_, row_;
    std::vector<std::vector<T>> data_;

    matrix(int columns, int rows) :
        col_(columns), 
        row_(rows), 
        data_(col_, std::vector<T>(row_))
    {}

    std::vector<T>& operator[](int col);
    const matrix<T> operator-(const matrix<T>& m);

    void input();
    void print();
};

template<typename T>
std::vector<T>& matrix<T>::operator[](int col) {
    return data_[col];
}

template<typename T>
const matrix<T> matrix<T>::operator-(const matrix<T>& m) {
    assert((*this).col_ == m.col_ && (*this).row_ == m.row_);

    matrix<T> res(m.col_, m.row_);
    
    for (int i = 0; i < res.col_; i++) {
        for (int j = 0; j < res.row_; j++) {
            res[i][j] = (*this)[i][j] - m.data_[i][j];
        }
    }

    return res;
}

template<typename T>
void matrix<T>::print() {
    for (int i = 0; i < col_; i++) {
        for (int j = 0; j < row_; j++) {
            std::cout << data_[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

template<typename T>
void matrix<T>::input() {
    for (int i = 0; i < row_; i++) {
        for (int j = 0; j < col_; j++) {
            std::cin >> data_[i][j];
        }
    }
}

int main() {
    int col, row;

    std::cin >> col >> row;
    matrix<int> a(col, row);
    matrix<int> b(col, row);

    a.input();
    b.input();

    matrix<int> c = a - b;

    c.print();
}