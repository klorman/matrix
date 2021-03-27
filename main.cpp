#include <iostream>
#include <vector>
#include <cassert>
#include <cmath>
#include <ctime>

struct RECT {
    int row1, col1, row2, col2;
};

template <typename T>
struct matrix {
    int col_, row_;
    T** data_;
    //std::vector<std::vector<T>> data_;

    matrix(int rows, int columns) :
        col_(columns), 
        row_(rows)
        //data_(row_, std::vector<T>(col_))
    {
        data_ = new T * [row_];
        for (int i = 0; i < row_; i++) {
            data_[i] = new T [col_];
        }
    }

    matrix(const matrix<T>& m) :
        col_ (m.col_),
        row_ (m.row_),
        data_(m.data_)
    {}

    matrix(const matrix<T>& m, const RECT& rect) :
        col_ (rect.col2 - rect.col1 + 1),
        row_ (rect.row2 - rect.row1 + 1)
        //data_(row_, std::vector<T>(col_))
    {
        //assert(col_ < m.col_ && row_ < m.row_);
        data_ = new T * [row_];
        for (int i = 0; i < row_; i++) {
            data_[i] = new T [col_];
        }

        for (int i = rect.row1; i <= rect.row2; i++) {
            for (int j = rect.col1; j <= rect.col2; j++) {
                data_[i - rect.row1][j - rect.col1] = m.data_[i][j];
            }
        }
    }

    ~matrix() {
        for (int i = 0; i < row_; i++) {
            delete[] data_[i];
        }
        delete[] data_;
    }

    matrix<T>& operator=(const matrix<T>& m);
    T* operator[](int row);
    matrix<T> operator-(const matrix<T>& m) const;
    matrix<T> operator+(const matrix<T>& m) const;
    matrix<T> operator*(const matrix<T>& m) const;

    void input();
    void random();
    void print();
};

template<typename T>
matrix<T> merge_square_matrices(matrix<T>* matrices[4]) {
    matrix<T> res(matrices[0]->col_ * 2, matrices[0]->row_ * 2);

    for (int matr = 0; matr < 4; matr++) {
        for (int i = 0; i < matrices[0]->row_; i++) {
            for (int j = 0; j < matrices[0]->col_; j++) {
                res.data_[int (matr / 2) * matrices[0]->row_ + i][(matr % 2) * matrices[0]->col_ + j] = matrices[matr]->data_[i][j];
            }
        }
    }

    return res;
}

template<typename T>
matrix<T> multiplication(const matrix<T>& a, const matrix<T>& b) {
    matrix<T> res(a.row_, b.col_);
    
    for (int i = 0; i < a.row_; i++) {
        for (int j = 0; j < b.col_; j++) {
            for (int elem = 0; elem < a.col_; elem++) {
                res.data_[i][j] += a.data_[i][elem] * b.data_[elem][j];
            }
        }
    }

    return res;
}

template<typename T>
matrix<T> Strassen_multiplication(const matrix<T>& a, const matrix<T>& b) {
    if (a.col_ < 164) {
        return multiplication(a, b);
    }
    
    int half_size = a.col_ / 2;

    matrix<T>   A11(a, {0,         0,         half_size-1, half_size-1}),
                A12(a, {0,         half_size, half_size-1, a.col_-1   }),
                A21(a, {half_size, 0,         a.row_-1,    half_size-1}),
                A22(a, {half_size, half_size, a.row_-1,    a.col_-1   }),
                B11(b, {0,         0,         half_size-1, half_size-1}),
                B12(b, {0,         half_size, half_size-1, b.col_-1   }),
                B21(b, {half_size, 0,         b.row_-1,    half_size-1}),
                B22(b, {half_size, half_size, b.row_-1,    b.col_-1   }),

                P1 = Strassen_multiplication(A11 + A22, B11 + B22),
                P2 = Strassen_multiplication(A21 + A22, B11      ),
                P3 = Strassen_multiplication(A11,       B12 - B22),
                P4 = Strassen_multiplication(A22,       B21 - B11),
                P5 = Strassen_multiplication(A11 + A12, B22      ),
                P6 = Strassen_multiplication(A21 - A11, B11 + B12),
                P7 = Strassen_multiplication(A12 - A22, B21 + B22),

                C11 = P1 + P4 - P5 + P7,
                C12 = P3 + P5,
                C21 = P2 + P4, 
                C22 = P1 - P2 + P3 + P6;

    matrix<T>* arr[] = {&C11, &C12, &C21, &C22};

    return merge_square_matrices(arr);
}

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

    for(int i = 0; i < m.row_; i++) {
        for(int j = 0; j < m.col_; j++) {
            data_[i][j] = m.data_[i][j];
        }
    }

    return *this;
}

template<typename T>
T* matrix<T>::operator[](int row) {
    return data_[row];
}

template<typename T>
matrix<T> matrix<T>::operator-(const matrix<T>& m) const {
    assert((*this).col_ == m.col_ && (*this).row_ == m.row_);

    matrix<T> res(m.col_, m.row_);

    for (int i = 0; i < res.row_; i++) {
        for (int j = 0; j < res.col_; j++) {
            res[i][j] = (*this).data_[i][j] - m.data_[i][j];
        }
    }

    return res;
}

template<typename T>
matrix<T> matrix<T>::operator+(const matrix<T>& m) const {
    assert((*this).col_ == m.col_ && (*this).row_ == m.row_);

    matrix<T> res(m.col_, m.row_);

    for (int i = 0; i < res.row_; i++) {
        for (int j = 0; j < res.col_; j++) {
            res[i][j] = (*this).data_[i][j] + m.data_[i][j];
        }
    }

    return res;
}

template<typename T>
matrix<T> matrix<T>::operator*(const matrix<T>& m) const {
    assert((*this).col_ == m.row_);

    if (std::max(std::max(col_, m.col_), std::max(row_, m.row_)) < 64) {
        return multiplication(*this, m);
    }

    //int n = get_new_dimension(*this, m);
    int n = 1000;
    matrix<T> A(n, n), B(n, n);
    A = *this;
    B = m;

    return matrix<T> (Strassen_multiplication<T>(A, B), {0, 0, (*this).row_ - 1, m.col_ - 1});
}

template<typename T>
void matrix<T>::print() {
    std::cout << std::endl;
    for (int i = 0; i < row_; i++) {
        for (int j = 0; j < col_; j++) {
            std::cout << data_[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

template<typename T>
void matrix<T>::random() {
    srand(time(0));

    for (int i = 0; i < row_; i++) {
        for (int j = 0; j < col_; j++) {
            data_[i][j] = rand();
        }
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

template<typename T>
int get_new_dimension(const matrix<T>& a, const matrix<T>& b) {
    return 1 << int(ceil(log2f(std::max(std::max(a.col_, a.row_), std::max(b.col_, b.row_)))));
}

int main() {
    int col, row;

    std::cin >> row >> col;
    matrix<int> a(row, col);
    std::cin >> row >> col;
    matrix<int> b(row, col);

    a.random();
    b.random();


    double s1 = clock(), s2;
    matrix<int> c = a * b;
    s2 = clock();

//    c.print();
    std::cout<<s2 - s1;
}
