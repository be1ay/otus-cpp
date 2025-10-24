#define BOOST_TEST_MODULE GraphicEditorTest
#include <boost/test/included/unit_test.hpp>
#include "matrix.h"

using namespace MyMatrix;

BOOST_AUTO_TEST_CASE(check_matrix) {
    Matrix<int, -1> matrix;
    BOOST_CHECK_EQUAL(matrix.size(),0);
    auto a = matrix[0][0];
    BOOST_CHECK_EQUAL(a,-1);
    BOOST_CHECK_EQUAL(matrix.size(),0);
    matrix[100][100] = 314;
    BOOST_CHECK_EQUAL(matrix[100][100],314);
    BOOST_CHECK_EQUAL(matrix.size(),1);

    MatrixN<int, -1, 3> matrix2;
    BOOST_CHECK_EQUAL(matrix2.size(),0);
    auto b = matrix2[0][0][0];
    BOOST_CHECK_EQUAL(b,-1);
    BOOST_CHECK_EQUAL(matrix2.size(),0);
    matrix2[100][100][100] = 314;
    BOOST_CHECK_EQUAL(matrix2[100][100][100],314);
    BOOST_CHECK_EQUAL(matrix2.size(),1);

}