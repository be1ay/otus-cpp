#include "matrix.h"
#include <cassert>
#include <iostream>
#include <algorithm>
using namespace MyMatrix;

int main() {
    // Пример из задания перенес в test_mymatrix.cpp
// бесконечная матрица int заполнена значениями -1
#if 0
    Matrix<int, -1> matrix;
    assert(matrix.size() == 0); // все ячейки свободны
    auto a = matrix[0][0];
    assert(a == -1);
    assert(matrix.size() == 0);
    matrix[100][100] = 314;
    assert(matrix[100][100] == 314);
    assert(matrix.size() == 1);

   for (const auto& [pos, value] : matrix) {
    const auto& [x, y] = pos;
    std::cout << x << y << value << std::endl;
    }
#endif
//------------------
    Matrix<int,0> matrix2;
    for (int i= 0;i<10;++i){
        matrix2[i][i]=i;
        matrix2[9-i][i]=i;
    }
    for(int i = 1;i<9;++i){
        for(int j = 1; j<9;++j)
        {
            std::cout<<matrix2[i][j]<<" ";
        }
        std::cout<<std::endl;
    }
    assert(matrix2.size() == 18);
    std::cout<<matrix2.size()<<std::endl; //количество занятых ячеек

    for (const auto& [pos, value] : matrix2) { // вывод всех занятых ячеек с позициями
        const auto& [x, y] = pos;
        std::cout << x << y << value << std::endl;
    }
    return 0;
}