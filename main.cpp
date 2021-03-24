#include <iostream>
#include <vector>
#include <cassert>
#include <cmath>

struct RECT {
    int col1, row1, col2, row2;
};

template <typename T>
struct matrix {
    int col_, row_;
    std::vector<std::vector<T>> data_;

    matrix(int columns, int rows) :
        col_(columns), 
        row_(rows), 
        data_(col_, std::vector<T>(row_))
    {}

    matrix(const matrix<T>& m) :
        col_ (m.col_),
        row_ (m.row_),
        data_(m.data_)
    {}

    matrix(const matrix<T>& m, const RECT& rect) :
        col_ (rect.col2 - rect.col1 + 1),
        row_ (rect.row2 - rect.row1 + 1),
        data_(col_, std::vector<T>(row_))
    {
        assert(col_ > m.col_ && row_ > m.row_);

        for (int i = rect.row1; i <= rect.row2; i++) {
            for (int j = rect.col1; j <= rect.col2; j++) {
                data_[i - rect.row1][j - rect.col1] = m.data_[i][j];
            }
        }
    }

    matrix<T>& operator=(const matrix<T>& m);
    std::vector<T>& operator[](int col);
    matrix<T> operator-(const matrix<T>& m) const;
    matrix<T> operator+(const matrix<T>& m) const;
    matrix<T> operator*(const matrix<T>& m) const;

    void input();
    void print();
};

template<typename T, int size>
matrix<T> Strassen_multiplication(const matrix<T>& a, const matrix<T>& b);

template<typename T>
matrix<T>& matrix<T>::operator=(const matrix<T>& m) {
    if (this == &m) {
		return *this;
	}

    if (col_ < m.col_ || row_ < m.row_) {
        col_ = m.col_;
        row_ = m.row_;
        data_ = m.data_;

        return *this;
    }

    for(int i = 0; i < m.col_; i++) {
        for(int j = 0; j < m.row_; j++) {
            data_[i][j] = m.data_[i][j];
        }
    }

    return *this;
}

template<typename T>
std::vector<T>& matrix<T>::operator[](int col) {
    return data_[col];
}

template<typename T>
matrix<T> matrix<T>::operator-(const matrix<T>& m) const {
    assert((*this).col_ == m.col_ && (*this).row_ == m.row_);

    matrix<T> res(m.col_, m.row_);

    for (int i = 0; i < res.col_; i++) {
        for (int j = 0; j < res.row_; j++) {
            res[i][j] = (*this).data_[i][j] - m.data_[i][j];
        }
    }

    return res;
}

template<typename T>
matrix<T> matrix<T>::operator+(const matrix<T>& m) const {
    assert((*this).col_ == m.col_ && (*this).row_ == m.row_);

    matrix<T> res(m.col_, m.row_);

    for (int i = 0; i < res.col_; i++) {
        for (int j = 0; j < res.row_; j++) {
            res[i][j] = (*this).data_[i][j] + m.data_[i][j];
        }
    }

    return res;
}

template<typename T>
matrix<T> matrix<T>::operator*(const matrix<T>& m) const {
    assert((*this).col_ == m.row_);

    int n = get_new_dimension(*this, m);

    matrix<T> A(n, n), B(n, n), C(n, n);
    A = *this;
    B = m;
    C = Strassen_multiplication<T, n>(A, B);
    return C;
}

template<typename T>
void matrix<T>::print() {
    std::cout << std::endl;
    for (int i = 0; i < col_; i++) {
        for (int j = 0; j < row_; j++) {
            std::cout << data_[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

template<typename T>
void matrix<T>::input() {
    for (int i = 0; i < row_; i++) {
        for (int j = 0; j < col_; j++) {
            std::cin >> data_[i][j];
        }
    }
}

template<typename T>
int get_new_dimension(const matrix<T>& a, const matrix<T>& b) {
    return 1 << int(ceil(log2f(std::max(std::max(a.col_, a.row_), std::max(b.col_, b.row_)))));
}

template<typename T, int size>
matrix<T> Strassen_multiplication(const matrix<T>& a, const matrix<T>& b) {
    return a;
}

int main() {
    int col, row;

    std::cin >> col >> row;
    matrix<int> a(col, row);
    matrix<int> b(col, row);

    a.input();
    b.input();

    matrix<int> c = a * b;

    c.print();

    //std::vector<int> d = {1, 2, 3};

    //std::cout << *d;
}