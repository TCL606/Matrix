#pragma once

#ifndef TCL_MATRIX_H
#define TCL_MATRIX_H

#include <iostream>
#include <cmath>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <cfloat>
#include <utility>
#include <complex>
#include <string>
#include <fstream>
#include <cstddef>
#include <initializer_list>

#define PRECISION_OF_DIFFERENCE 1e-3
#define PRECISION_WHEN_CALCULATING 1e-5

#ifndef TCL_MATRIX_UNUSED_PARAMETER
#define TCL_MATRIX_UNUSED_PARAMETER(x) ((void)(x))
#endif

namespace TCL_Matrix
{
    class Matrix
    {
    private:
        /// <summary>
        /// 多项式函数，用于特征值计算
        /// </summary>
        /// <param name="coeff">多项式系数数组</param>
        /// <param name="degree">多项式次数</param>
        /// <param name="z">自变量</param>
        /// <returns></returns>
        static std::complex<double> PolynomialFunc(const double* coeff, int degree, const std::complex<double>& z)
        {
            std::complex<double> ret = { 0,0 };
            for (int i = 0; i < degree + 1; i++)
            {
                ret += (coeff[i] * std::pow(z, i));
            }
            return ret;
        }

        /// <summary>
        /// 求特征根时的排序函数
        /// </summary>
        /// <param name="z1"></param>
        /// <param name="z2"></param>
        /// <param name="precision"></param>
        /// <returns></returns>
        static bool Greater(const std::complex<double>& z1, const std::complex<double>& z2, double precision = 0.1 * PRECISION_OF_DIFFERENCE)
        {
            if (z1.real() > precision + z2.real())
                return true;
            else if (z1.real() + precision < z2.real())
                return false;
            else if (std::abs(z1.imag()) - std::abs(z2.imag()) > precision)
                return true;
            else if (std::abs(z1.imag()) < std::abs(z2.imag()) + precision)
                return false;
            else if (z1.imag() > z2.imag() + precision)
                return true;
            else return false;
        }
    
    protected:
        int row;
        int col;

    public:
        double** matrix;

        /// <summary>
        /// 将已有矩阵深复制到对象矩阵中。
        /// </summary>
        /// <param name="matrix">使用时，可以将double**类型强转为double*类型输入</param>
        /// <param name="row"></param>
        /// <param name="col"></param>
        Matrix(double* newMatrix, int row, int col) :row(row), col(col)
        {
            if (row <= 0 || col <= 0)
            {
                row = col = 0;
                matrix = nullptr;
                return;
            }

            matrix = new double* [row];
            for (int i = 0; i < row; i++)
                matrix[i] = new double[col];
            for (int i = 0; i < row; i++)
                for (int j = 0; j < col; j++)
                {
                    matrix[i][j] = newMatrix[col * i + j];
                }
        }

        /// <summary>
        /// 创建新矩阵并初始化为0
        /// </summary>
        /// <param name="row"></param>
        /// <param name="col"></param>
        explicit Matrix(int row = 1, int col = 1) :row(row), col(col)
        {
            if (row < 0)
                row = 0;
            if (col < 0)
                col = 0;
            if (row == 0 || col == 0)
            {
                row = col = 0;
                matrix = nullptr;
                return;
            }

            matrix = new double* [row];
            for (int i = 0; i < row; i++)
                matrix[i] = new double[col];
            for (int i = 0; i < row; i++)
            {
                for (int j = 0; j < col; j++)
                {
                    matrix[i][j] = 0;
                }
            }
        }

        Matrix(std::nullptr_t) : row(0), col(0), matrix(nullptr)
        {
            
        }

        Matrix(std::initializer_list<std::initializer_list<double>> init)
        {
            row = init.size();
            if (row == 0)
            {
                col = 0;
                matrix = nullptr;
                return;
            }
            col = init.begin()->size();
            if (col == 0)
            {
                matrix = nullptr;
                return;
            }

            matrix = new double* [row];
            for (int i = 0; i < row; i++)
                matrix[i] = new double[col];

            auto p = init.begin();
            for (int i = 0; i < row; ++i, ++p)
            {
                auto q = p->begin();
                for (int j = 0; j < col; j++, ++q)
                {
                    matrix[i][j] = *q;
                }
            }
        }

        /// <summary>
        /// 复制构造函数，深复制
        /// </summary>
        /// <param name="A"></param>
        Matrix(const Matrix& A) : row(A.row), col(A.col)
        {
            if (&A == this || A == nullptr)
            {
                row = col = 0;
                matrix = nullptr;
                return;
            }

            matrix = new double* [row];
            for (int i = 0; i < row; i++)
                matrix[i] = new double[col];
            for (int i = 0; i < row; i++)
            {
                for (int j = 0; j < col; j++)
                {
                    matrix[i][j] = A.matrix[i][j];
                }
            }
        }

        /// <summary>
        /// 移动构造函数
        /// </summary>
        /// <param name="A"></param>
        Matrix(Matrix&& A) noexcept : row(A.row), col(A.col)
        {
            if (&A == this || A == nullptr)
            {
                row = col = 0;
                matrix = nullptr;
                return;
            }

            this->matrix = A.matrix;
            A.matrix = nullptr;
            A.row = A.col = 0;
        }

        ~Matrix()
        {
            if (row == 0) return;
            for (int i = 0; i < row; i++)
                delete[] matrix[i];
            delete[] matrix;
        }

        /// <summary>
        /// 高斯消元，化为行简化阶梯型
        /// </summary>
        void Gauss_Jordan_Elimination()
        {
            int zeroRow = 0;
            int rank = -1;
            for (int i = 0; i < col && i < row; i++)  //i看成列
            {
                int nonZero = row - 1;  //先设非零行为最后一行
                for (int j = i - zeroRow; j < row; j++) // 找到非0元素
                    if (std::abs(matrix[j][i]) > PRECISION_OF_DIFFERENCE)
                    {
                        nonZero = j;
                        break;
                    }

                if (std::abs(matrix[nonZero][i]) > PRECISION_OF_DIFFERENCE)
                {
                    if (nonZero != i - zeroRow) //如果非0元素不是第i-zeroRow行
                    {
                        for (int k = i; k < col; k++) // 把非0元素所在行交换到当前行
                        {
                            double t = matrix[i - zeroRow][k];
                            matrix[i - zeroRow][k] = matrix[nonZero][k];
                            matrix[nonZero][k] = t;
                        }
                        nonZero = i - zeroRow;
                    }

                    if (std::abs(matrix[i - zeroRow][i] - 1.0) > PRECISION_OF_DIFFERENCE)  //归一化
                    {
                        double temp = matrix[i - zeroRow][i];
                        for (int k = i; k < col; k++)
                        {
                            matrix[i - zeroRow][k] /= temp;
                        }
                    }

                    for (int j = i - zeroRow + 1; j < row; j++) //把下面部分消为0
                    {
                        if (std::abs(matrix[j][i]) > PRECISION_OF_DIFFERENCE) //如果matrix[j][i]不是0
                        {
                            double temp = matrix[j][i];
                            for (int k = i; k < col; k++)
                            {
                                matrix[j][k] = matrix[j][k] - matrix[i - zeroRow][k] * temp;
                            }
                        }
                    }

                    for (int j = i - zeroRow - 1; j >= 0; j--) //把上面部分消为0
                    {
                        if (std::abs(matrix[j][i]) > PRECISION_OF_DIFFERENCE)
                        {
                            double temp = matrix[j][i];
                            for (int k = i; k < col; k++)
                            {
                                matrix[j][k] = matrix[j][k] - matrix[i - zeroRow][k] * temp;
                            }
                        }
                    }
                }
                else
                {
                    zeroRow++;
                }
            }
            if (col > row)
                rank = row - zeroRow;
            else rank = col - zeroRow;
            if (col > row && rank < row) //若此时不满秩，且col>row，则最后一行可能不会被归一化，且最后一行的秩不会被计算，要单独处理
            {
                int k = row;
                while (std::abs(matrix[row - 1][k]) < PRECISION_OF_DIFFERENCE && k < col)
                {
                    k++;
                }
                if (k < col)
                {
                    double temp = matrix[row - 1][k];
                    for (int i = k; i < col; i++)
                    {
                        matrix[row - 1][i] /= temp;
                    }
                    rank++; //加上最后一行的秩
                    for (int j = row - 2; j >= 0; j--) //把上面部分消为0
                    {
                        if (std::abs(matrix[j][k]) > PRECISION_OF_DIFFERENCE)
                        {
                            double temp = matrix[j][k];
                            for (int t = k; t < col; t++)
                            {
                                matrix[j][t] = matrix[j][t] - matrix[row - 1][t] * temp;
                            }
                        }
                    }
                }
            }
        }

        /// <summary>
        /// 若对象为增广矩阵，可用此函数求解
        /// </summary>
        /// <param name="solution">用于接收一个解向量</param>
        /// <returns>若返回值为false，则方程无解，未对返回参数操作</returns>
        bool GetOneAnswerForAugmentedMatrix(Matrix& ret)
        {
            if (*this == nullptr) return false;

            Matrix original(*this);
            int zeroRow = 0;
            for (int i = 0; i < original.col - 1 && i < original.row; i++)  //i看成列，此时是增广矩阵，i最多到col - 1
            {
                int nonZero = original.row - 1;  //先设非零行为最后一行
                for (int j = i - zeroRow; j < original.row; j++) // 找到非0元素
                    if (std::abs(original.matrix[j][i]) > PRECISION_OF_DIFFERENCE)
                    {
                        nonZero = j;
                        break;
                    }

                if (std::abs(original.matrix[nonZero][i]) > PRECISION_OF_DIFFERENCE)
                {
                    if (nonZero != i - zeroRow) //如果非0元素不是第i-zeroRow行
                    {
                        for (int k = i; k < original.col; k++) // 把非0元素所在行交换到当前行
                        {
                            double t = original.matrix[i - zeroRow][k];
                            original.matrix[i - zeroRow][k] = original.matrix[nonZero][k];
                            original.matrix[nonZero][k] = t;
                        }
                        nonZero = i - zeroRow;
                    }

                    if (std::abs(original.matrix[i - zeroRow][i] - 1.0) > PRECISION_OF_DIFFERENCE)  //归一化
                    {
                        double temp = original.matrix[i - zeroRow][i];
                        for (int k = i; k < original.col; k++)
                        {
                            original.matrix[i - zeroRow][k] /= temp;
                        }
                    }

                    for (int j = i - zeroRow + 1; j < original.row; j++) //把下面部分消为0
                    {
                        if (std::abs(original.matrix[j][i]) > PRECISION_OF_DIFFERENCE) //如果matrix[j][i]不是0
                        {
                            double temp = original.matrix[j][i];
                            for (int k = i; k < original.col; k++)
                            {
                                original.matrix[j][k] = original.matrix[j][k] - original.matrix[i - zeroRow][k] * temp;
                            }
                        }
                    }

                    for (int j = i - zeroRow - 1; j >= 0; j--) //把上面部分消为0
                    {
                        if (std::abs(original.matrix[j][i]) > PRECISION_OF_DIFFERENCE)
                        {
                            double temp = original.matrix[j][i];
                            for (int k = i; k < original.col; k++)
                            {
                                original.matrix[j][k] = original.matrix[j][k] - original.matrix[i - zeroRow][k] * temp;
                            }
                        }
                    }
                }
                else
                {
                    zeroRow++;
                }
            }
            int rank;
            if (original.col - 1 > original.row)
                rank = original.row - zeroRow;
            else rank = original.col - 1 - zeroRow;
            if (original.col - 1 > original.row && rank < original.row) //若此时原矩阵不满秩，且col - 1>row，则最后一行可能不会被归一化，且最后一行的秩不会被计算，要单独处理
            {
                int k = original.row;
                while (std::abs(original.matrix[original.row - 1][k]) < PRECISION_OF_DIFFERENCE && k < original.col - 1)
                {
                    k++;
                }
                if (k < original.col - 1)
                {
                    double temp = original.matrix[original.row - 1][k];
                    for (int i = k; i < original.col; i++)
                    {
                        original.matrix[original.row - 1][i] /= temp;
                    }
                    rank++; //加上最后一行的秩
                    for (int j = original.row - 2; j >= 0; j--) //把上面部分消为0
                    {
                        if (std::abs(original.matrix[j][k]) > PRECISION_OF_DIFFERENCE)
                        {
                            double temp = original.matrix[j][k];
                            for (int t = k; t < original.col; t++)
                            {
                                original.matrix[j][t] = original.matrix[j][t] - original.matrix[original.row - 1][t] * temp;
                            }
                        }
                    }
                }
            }

            //先看有没有解，即看行简化阶梯型的全零行对应增广矩阵最右行的元素是否为0
            for (int i = original.row - 1; i >= rank; i--)
            {
                if (std::abs(original.matrix[i][original.col - 1]) > PRECISION_OF_DIFFERENCE)
                {
                    return false;
                }
            }

            Matrix solution(col - 1, 1);
            int numOfFreeColumn = 0;

            int temp;  //记录列
            for (temp = 0; temp < original.col - 1 && temp - numOfFreeColumn < original.row; temp++)  //主列位置对应分量为目标向量的对应分量，自由列对应分量直接取0
            {
                if (std::abs(original.matrix[temp - numOfFreeColumn][temp]) > PRECISION_OF_DIFFERENCE)
                {
                    solution.matrix[temp][0] = original.matrix[temp - numOfFreeColumn][original.col - 1];
                }
                else
                {
                    numOfFreeColumn++;
                    solution.matrix[temp][0] = 0;//自由列位置处取0肯定满足方程
                }
            }

            while (temp < original.col - 1)
            {
                solution.matrix[temp++][0] = 0;
            }
            ret = solution;
            return true;
        }

        /// <summary>
        /// 若对象为增广矩阵，可用此函数找到线性方程组的所有解
        /// </summary>
        /// <param name="ret">前col-1列是零空间的一组基，最后1列是一个特解</param>
        /// <returns>若返回值为false，则方程无解，未对返回参数操作</returns>
        bool GetAllSolutionsForAugmentedMatrix(Matrix& ret)
        {
            if (*this == nullptr) return false;

            Matrix original(*this);
            int zeroRow = 0;
            for (int i = 0; i < original.col - 1 && i < original.row; i++)  //i看成列，此时是增广矩阵，i最多到col - 1
            {
                int nonZero = original.row - 1;  //先设非零行为最后一行
                for (int j = i - zeroRow; j < original.row; j++) // 找到非0元素
                    if (std::abs(original.matrix[j][i]) > PRECISION_OF_DIFFERENCE)
                    {
                        nonZero = j;
                        break;
                    }

                if (std::abs(original.matrix[nonZero][i]) > PRECISION_OF_DIFFERENCE)
                {
                    if (nonZero != i - zeroRow) //如果非0元素不是第i-zeroRow行
                    {
                        for (int k = i; k < original.col; k++) // 把非0元素所在行交换到当前行
                        {
                            double t = original.matrix[i - zeroRow][k];
                            original.matrix[i - zeroRow][k] = original.matrix[nonZero][k];
                            original.matrix[nonZero][k] = t;
                        }
                        nonZero = i - zeroRow;
                    }

                    if (std::abs(original.matrix[i - zeroRow][i] - 1.0) > PRECISION_OF_DIFFERENCE)  //归一化
                    {
                        double temp = original.matrix[i - zeroRow][i];
                        for (int k = i; k < original.col; k++)
                        {
                            original.matrix[i - zeroRow][k] /= temp;
                        }
                    }

                    for (int j = i - zeroRow + 1; j < original.row; j++) //把下面部分消为0
                    {
                        if (std::abs(original.matrix[j][i]) > PRECISION_OF_DIFFERENCE) //如果matrix[j][i]不是0
                        {
                            double temp = original.matrix[j][i];
                            for (int k = i; k < original.col; k++)
                            {
                                original.matrix[j][k] = original.matrix[j][k] - original.matrix[i - zeroRow][k] * temp;
                            }
                        }
                    }

                    for (int j = i - zeroRow - 1; j >= 0; j--) //把上面部分消为0
                    {
                        if (std::abs(original.matrix[j][i]) > PRECISION_OF_DIFFERENCE)
                        {
                            double temp = original.matrix[j][i];
                            for (int k = i; k < original.col; k++)
                            {
                                original.matrix[j][k] = original.matrix[j][k] - original.matrix[i - zeroRow][k] * temp;
                            }
                        }
                    }
                }
                else
                {
                    zeroRow++;
                }
            }
            int rank;
            if (original.col - 1 > original.row)
                rank = original.row - zeroRow;  //这里是指原矩阵的秩，不是增广矩阵的秩。
            else rank = original.col - 1 - zeroRow;
            if (original.col - 1 > original.row && rank < original.row) //若此时原矩阵不满秩，且col - 1>row，则最后一行可能不会被归一化，且最后一行的秩不会被计算，要单独处理
            {
                int k = original.row;
                while (std::abs(original.matrix[original.row - 1][k]) < PRECISION_OF_DIFFERENCE && k < original.col - 1)
                {
                    k++;
                }
                if (k < original.col - 1)
                {
                    double temp = original.matrix[original.row - 1][k];
                    for (int i = k; i < original.col; i++)
                    {
                        original.matrix[original.row - 1][i] /= temp;
                    }
                    rank++; //加上最后一行的秩
                    for (int j = original.row - 2; j >= 0; j--) //把上面部分消为0
                    {
                        if (std::abs(original.matrix[j][k]) > PRECISION_OF_DIFFERENCE)
                        {
                            double temp = original.matrix[j][k];
                            for (int t = k; t < original.col; t++)
                            {
                                original.matrix[j][t] = original.matrix[j][t] - original.matrix[original.row - 1][t] * temp;
                            }
                        }
                    }
                }
            }
            //先看有没有解，即看行简化阶梯型的全零行对应增广矩阵最右行的元素是否为0
            for (int i = original.row - 1; i >= rank; i--)
            {
                if (std::abs(original.matrix[i][original.col - 1]) > PRECISION_OF_DIFFERENCE)
                {
                    return false;
                }
            }

            Matrix solution(original.col - 1, original.col - rank);
            int numOfFreeColumn = 0;
            int temp = 0;
            for (temp = 0; temp < original.col - 1 && temp - numOfFreeColumn < original.row; temp++) //先找零空间的基
            {
                if (std::abs(original.matrix[temp - numOfFreeColumn][temp]) > PRECISION_OF_DIFFERENCE)
                    continue;
                else
                {
                    solution.matrix[temp][numOfFreeColumn] = 1;
                    for (int j = 0; j < temp - numOfFreeColumn; j++)
                    {
                        solution.matrix[j][numOfFreeColumn] = -original.matrix[j][temp];
                    }
                    numOfFreeColumn++;
                }
            }
            while (temp < original.col - 1)
            {
                solution.matrix[temp][numOfFreeColumn] = 1;
                for (int j = 0; j < original.row; j++)
                {
                    solution.matrix[j][numOfFreeColumn] = -original.matrix[j][temp];
                }
                numOfFreeColumn++;
                temp++;
            }

            numOfFreeColumn = 0;
            //找特解
            for (temp = 0; temp < original.col - 1 && temp - numOfFreeColumn < original.row; temp++)  //主列位置对应分量为目标向量的对应分量，自由列对应分量直接取0
            {
                if (std::abs(original.matrix[temp - numOfFreeColumn][temp]) > PRECISION_OF_DIFFERENCE)
                {
                    solution.matrix[temp][original.col - rank - 1] = original.matrix[temp - numOfFreeColumn][original.col - 1];
                }
                else
                {
                    numOfFreeColumn++;
                    solution.matrix[temp][original.col - rank - 1] = 0;
                }
            }
            while (temp < original.col - 1)
            {
                solution.matrix[temp++][original.col - rank - 1] = 0;
            }
            ret = solution;
            return true;
        }

        /// <summary>
        /// 找到零空间的一组基
        /// </summary>
        /// <param name="bases">用于返回基</param>
        /// <returns>若矩阵可逆，则返回false；否则，返回一组基。注：无论矩阵是否可逆，理论上作为返回值的参数bases都会发生变化。</returns>
        bool GetBasesOfNullSpace(Matrix& bases)
        {
            if (*this == nullptr) return false;

            Matrix ret(row, col + 1);
            for (int i = 0; i < row; i++)
            {
                for (int j = 0; j < col; j++)
                {
                    ret.matrix[i][j] = matrix[i][j];
                }
            }
            if (ret.GetAllSolutionsForAugmentedMatrix(ret))
            {
                if (ret.col > 1)
                {
                    Matrix nullSpace(ret.row, ret.col - 1);
                    for (int i = 0; i < nullSpace.row; i++)
                    {
                        for (int j = 0; j < nullSpace.col; j++)
                        {
                            nullSpace.matrix[i][j] = ret.matrix[i][j];
                        }
                    }
                    bases = nullSpace;
                    return true;
                }
                else
                {
                    std::cout << "The matrix's null space only contains 0 so that it has no bases." << std::endl;
                    return false;
                }
            }
            else
            {
                std::cout << "Something went wrong so that the matrix's null space is empty!" << std::endl;
                return false;
            }
        }

        /// <summary>
        /// 行列式，不为方阵时返回0
        /// </summary>
        double Determinant() const
        {
            if (*this == nullptr) return 0.0;
            if (row != col)
                return 0;
            int swapTimes = 0; //行交换次数
            double** mtemp = new double* [row];
            for (int i = 0; i < row; i++)
                mtemp[i] = new double[col];
            for (int i = 0; i < row; i++)
                for (int j = 0; j < col; j++)
                {
                    mtemp[i][j] = matrix[i][j];
                }
            int zeroRow = 0;
            for (int i = 0; i < col && i < row; i++)  //i看成列
            {
                int nonZero = row - 1;  //先设非零行为最后一行
                for (int j = i - zeroRow; j < row; j++) // 找到非0元素
                    if (std::abs(mtemp[j][i]) > PRECISION_OF_DIFFERENCE)
                    {
                        nonZero = j;
                        break;
                    }

                if (std::abs(mtemp[nonZero][i]) > PRECISION_OF_DIFFERENCE)
                {
                    if (nonZero != i - zeroRow) //如果非0元素不是第i-zeroRow行
                    {
                        for (int k = i; k < col; k++) // 把非0元素所在行交换到当前行
                        {
                            double t = mtemp[i - zeroRow][k];
                            mtemp[i - zeroRow][k] = mtemp[nonZero][k];
                            mtemp[nonZero][k] = t;
                        }
                        nonZero = i - zeroRow;
                        swapTimes++;
                    }

                    for (int j = i - zeroRow + 1; j < row; j++) //把下面部分消为0
                    {
                        if (std::abs(mtemp[j][i]) > PRECISION_OF_DIFFERENCE) //如果mtemp[j][i]不是0
                        {
                            double temp = mtemp[j][i] / mtemp[i - zeroRow][i];
                            for (int k = i; k < col; k++)
                            {
                                mtemp[j][k] = mtemp[j][k] - mtemp[i - zeroRow][k] * temp;
                            }
                        }
                    }
                }
            }
            double ret = 1;
            if (std::abs(mtemp[row - 1][col - 1]) < PRECISION_OF_DIFFERENCE)
                ret = 0;
            else
            {
                for (int i = 0; i < row; i++)
                {
                    ret *= mtemp[i][i];
                }
                if (swapTimes % 2)
                    ret = -ret;
            }

            for (int i = 0; i < row; i++)
                delete[] mtemp[i];
            delete[]mtemp;
            return ret;
        }

        /// <summary>
        /// 求矩阵的逆矩阵
        /// </summary>
        /// <param name="inverse">inverse应为row*row的矩阵对象，用于接收返回值。若矩阵可逆，则逆矩阵通过该参数返回；若矩阵不可逆，则inverse不会进行任何操作</param>
        /// <returns>返回矩阵是否可逆</returns>
        bool InverseMatrix(Matrix& inverse)
        {
            if (*this == nullptr) return false;
            if (row != col)
                return false;
            if (inverse.row != inverse.col || inverse.row != row)
            {
                std::cout << "The matrix may be invertible, but the second parameter has wrong dimension so it failed to receive the returned value!" << std::endl;
                return false;
            }
            Matrix caculateInverse(row, 2 * row);
            for (int i = 0; i < row; i++)
                for (int j = 0; j < row; j++)
                {
                    caculateInverse.matrix[i][j] = matrix[i][j];
                    caculateInverse.matrix[i][j + row] = i == j ? 1 : 0;
                }
            caculateInverse.Gauss_Jordan_Elimination();
            if (std::abs(caculateInverse.matrix[row - 1][row - 1]) < PRECISION_OF_DIFFERENCE) //高斯消元后，右下角为0，则不可逆；否则可逆。
                return false;
            for (int i = 0; i < row; i++)
            {
                for (int j = 0; j < row; j++)
                {
                    inverse.matrix[i][j] = caculateInverse.matrix[i][j + row];
                }
            }
            return true;
        }

        /// <summary>
        /// 矩阵转置
        /// </summary>
        /// <param name="A"></param>
        /// <returns>返回转置后的矩阵</returns>
        friend Matrix Transpose(const Matrix& A)
        {
            Matrix temp = A;
            temp.TransposeDirectly();
            return temp;
        }

        /// <summary>
                /// 直接将对象矩阵转置
                /// </summary>
        void TransposeDirectly()
        {
            if (row != col)
            {
                double** tranm = new double* [col];
                for (int i = 0; i < col; i++)
                {
                    tranm[i] = new double[row];
                }

                for (int i = 0; i < row; i++)
                {
                    for (int j = 0; j < col; j++)
                    {
                        tranm[j][i] = matrix[i][j];
                    }
                }

                for (int i = 0; i < row; i++)
                    delete[] matrix[i];
                delete[] matrix;

                matrix = tranm;

                int t = col;
                col = row;
                row = t;
            }
            else
            {
                for (int i = 0; i < row; i++)
                {
                    for (int j = i + 1; j < col; j++)
                    {
                        double t = matrix[j][i];
                        matrix[j][i] = matrix[i][j];
                        matrix[i][j] = t;
                    }
                }
            }
        }

        /// <summary>
        /// LU分解
        /// </summary>
        /// <param name="L"></param>
        /// <param name="U"></param>
        /// <returns>返回true时，该矩阵可以LU分解，分解出的L和U通过参数返回</returns>
        bool LU(Matrix& L, Matrix& U) const
        {
            if (*this == nullptr) return false;
            if (col != row)
            {
                std::cout << "The matrix is not a square.LU decomposition failed." << std::endl;
                return false;
            }
            L = IdentityMatrix(col);
            U = *this;
            for (int i = 0; i < col; i++)
            {
                if (std::abs(U.matrix[i][i]) > PRECISION_OF_DIFFERENCE)
                {
                    for (int j = i + 1; j < row; j++)
                    {
                        L.matrix[j][i] = U.matrix[j][i] / U.matrix[i][i];
                        for (int k = i; k < col; k++)
                        {
                            U.matrix[j][k] -= L.matrix[j][i] * U.matrix[i][k];
                        }
                    }
                }
                else
                {
                    bool flag = false;
                    for (int k = i + 1; k < row; k++)
                    {
                        if (std::abs(U.matrix[k][i]) > PRECISION_OF_DIFFERENCE)
                        {
                            flag = true;
                            break;
                        }
                    }
                    if (flag)
                    {
                        std::cout << "The matrix can't be LU decomposed." << std::endl;
                        return false;
                    }
                }
            }
            return true;
        }

        /// <summary>
        /// 奇异值分解
        /// </summary>
        /// <param name="U">正交矩阵U</param>
        /// <param name="Sigma">奇异值矩阵Sigma</param>
        /// <param name="VT">正交矩阵V的转置</param>
        /// <param name="precision">计算精度</param>
        /// <param name="minIteration">幂法最小迭代次数</param>
        /// <returns>返回为false时，奇异值分解失败（可能是由于计算过程中的精度损失造成的），此时U、Sigma、V未定义</returns>
        bool SVD(Matrix& U, Matrix& Sigma, Matrix& VT, double precision = PRECISION_WHEN_CALCULATING, int minIteration = 50) const
        {
            if (*this == nullptr) return false;
            Matrix A(*this);
            A.TransposeDirectly();
            std::vector<double> values;
            Sigma = Matrix(this->row, this->col);
            Matrix V(this->col, this->col);
            U = Matrix(this->row, this->row);

            if (this->col < this->row)
            {
                Matrix ATA = A * (*this);
                ATA.GetEigenValuesOfDefiniteMatrix(values, precision, minIteration, false);
                std::sort(values.begin(), values.end(), std::greater<double>());
                std::vector<int> multiplicity;
                Matrix eigenSubSpace;
                int nowDim = 0;
                int ATADim = 0;
                double mod = 0;
                int len = 0;  //非0奇异值个数

                for (const auto& val : values)
                {
                    if ((ATA - val * IdentityMatrix(ATA.row)).GetBasesOfNullSpace(eigenSubSpace))
                    {
                        for (int i = 0; i < eigenSubSpace.row; i++)
                        {
                            for (int j = 0; j < eigenSubSpace.col; j++)
                            {
                                V.matrix[i][j + nowDim] = eigenSubSpace.matrix[i][j];
                            }
                        }
                        multiplicity.push_back(eigenSubSpace.col);
                        nowDim += eigenSubSpace.col;
                        if (std::abs(val) > PRECISION_OF_DIFFERENCE)
                        {
                            ATADim += eigenSubSpace.col;
                            len++;
                        }
                    }
                    else
                    {
                        std::cout << "The eigen value " << val << " is not correct! Maybe the precision or the minIteration is too small." << std::endl;
                        return false;
                    }
                }
                Matrix V1(V.row, ATADim);
                for (int i = 0; i < V1.row; i++)
                {
                    for (int j = 0; j < V1.col; j++)
                    {
                        V1.matrix[i][j] = V.matrix[i][j];
                    }
                }
                //V1作正交化
                int nowCol = 0;
                for (int j = 0; j < V1.col; j++)
                {
                    mod = 0;
                    for (int i = 0; i < V1.row; i++)
                    {
                        mod += V1.matrix[i][j] * V1.matrix[i][j];
                    }
                    mod = sqrt(mod);
                    for (int i = 0; i < V1.row; i++)
                    {
                        V1.matrix[i][j] /= mod; //归一化
                    }
                    mod = 0;
                    for (int k = 0; k < nowCol; k++)
                    {
                        double innerProduct = 0;
                        for (int p = 0; p < V1.row; p++)
                        {
                            innerProduct += V1.matrix[p][k] * V1.matrix[p][nowCol];
                        }
                        for (int p = 0; p < V1.row; p++)
                        {
                            V1.matrix[p][nowCol] -= innerProduct * V1.matrix[p][k];
                            mod += V1.matrix[p][nowCol] * V1.matrix[p][nowCol];
                        }
                        mod = sqrt(mod);
                        for (int p = 0; p < V1.row; p++)
                        {
                            V1.matrix[p][nowCol] /= mod; //归一化
                        }
                        nowCol++;
                    }
                }

                int now = 0;
                Matrix Sigmar(ATADim, ATADim);  //作一个Sigmar矩阵
                for (int i = 0; i < len; i++)
                {
                    for (int j = 0; j < multiplicity[i]; j++)
                    {
                        if (std::abs(values[i]) > PRECISION_OF_DIFFERENCE)
                        {
                            Sigmar.matrix[now][now] = 1 / sqrt(values[i]); //Sigmar的逆矩阵
                            now++;
                        }
                    }
                }

                now = 0;
                for (std::size_t i = 0; i < values.size(); i++)
                {
                    for (int j = 0; j < multiplicity[i]; j++)
                    {
                        if (std::abs(values[i]) > PRECISION_OF_DIFFERENCE)
                        {
                            Sigma.matrix[now][now] = sqrt(values[i]); //Sigma矩阵赋值
                            now++;
                        }
                    }
                }

                Matrix U1((*this) * V1 * Sigmar);

                //还要生成V的正交基
                if (V1.col < V.col)
                {
                    Matrix I(IdentityMatrix(V.row));
                    Matrix zero(V.row, 1);
                    Matrix V2(V.row, V.col - V1.col);
                    Matrix V1T(Transpose(V1));

                    int nowExpandDim = 0;
                    int iterateTimes = 0;
                    double mod;
                    while (iterateTimes < V.col - V1.col)
                    {
                        for (int i = 0; i < V.row; i++)
                        {
                            mod = 0;
                            Matrix vector(V.row, 1);
                            vector.matrix[i][0] = 1;
                            vector = (I - V1 * V1T) * vector;
                            for (int j = 0; j < nowExpandDim; j++)
                            {
                                double innerProduct = 0;
                                for (int p = 0; p < V2.row; p++)
                                {
                                    innerProduct += V2.matrix[p][j] * vector.matrix[p][0];
                                }
                                for (int p = 0; p < V1.row; p++)
                                {
                                    vector.matrix[p][0] -= innerProduct * V2.matrix[p][j];
                                }
                            }

                            if (vector != zero)
                            {
                                for (int p = 0; p < vector.row; p++)
                                {
                                    mod += vector.matrix[p][0] * vector.matrix[p][0];
                                }
                                mod = sqrt(mod);
                                for (int p = 0; p < V2.row; p++)
                                {
                                    V2.matrix[p][nowExpandDim] = vector.matrix[p][0] / mod;
                                }
                                nowExpandDim++;
                                break;
                            }
                        }
                        iterateTimes++;
                    }
                    if (nowExpandDim != V.col - V1.col)
                    {
                        std::cout << "Something went wrong that the program can't find all V's orthogonal bases!" << std::endl;
                        return false;
                    }
                    V = V1 & V2;
                }
                else V = V1;

                //生成U的正交基
                if (U1.col < U.col)  //当原矩阵不是方阵时，这里的U1还不是U，要补充正交向量
                {
                    Matrix I(IdentityMatrix(U.row));
                    Matrix zero(U.row, 1);
                    Matrix U2(U.row, U.col - U1.col);
                    Matrix U1T(Transpose(U1));

                    int nowExpandDim = 0;
                    int iterateTimes = 0;
                    double mod;
                    while (iterateTimes < U.col - U1.col)
                    {
                        for (int i = 0; i < U.row; i++)
                        {
                            mod = 0;
                            Matrix vector(U.row, 1);
                            vector.matrix[i][0] = 1;
                            vector = (I - U1 * U1T) * vector;
                            for (int j = 0; j < nowExpandDim; j++)
                            {
                                double innerProduct = 0;
                                for (int p = 0; p < U2.row; p++)
                                {
                                    innerProduct += U2.matrix[p][j] * vector.matrix[p][0];
                                }
                                for (int p = 0; p < U1.row; p++)
                                {
                                    vector.matrix[p][0] -= innerProduct * U2.matrix[p][j];
                                }
                            }

                            if (vector != zero)
                            {
                                for (int p = 0; p < vector.row; p++)
                                {
                                    mod += vector.matrix[p][0] * vector.matrix[p][0];
                                }
                                mod = sqrt(mod);
                                for (int p = 0; p < U2.row; p++)
                                {
                                    U2.matrix[p][nowExpandDim] = vector.matrix[p][0] / mod;
                                }
                                nowExpandDim++;
                                break;
                            }
                        }
                        iterateTimes++;
                    }
                    if (nowExpandDim != U.col - U1.col)
                    {
                        std::cout << "Something went wrong that the program can't find all U's orthogonal bases!" << std::endl;
                        return false;
                    }
                    U = U1 & U2;
                }
                else U = U1;
            }
            else
            {
                Matrix AAT = (*this) * A;
                AAT.GetEigenValuesOfDefiniteMatrix(values, precision, minIteration, false);
                std::sort(values.begin(), values.end(), std::greater<double>());
                std::vector<int> multiplicity;
                Matrix eigenSubSpace;
                int nowDim = 0;
                int AATDim = 0;
                double mod = 0;
                int len = 0;

                for (const auto& val : values)
                {
                    if ((AAT - val * IdentityMatrix(AAT.row)).GetBasesOfNullSpace(eigenSubSpace))
                    {
                        for (int i = 0; i < eigenSubSpace.row; i++)
                        {
                            for (int j = 0; j < eigenSubSpace.col; j++)
                            {
                                U.matrix[i][j + nowDim] = eigenSubSpace.matrix[i][j];
                            }
                        }
                        multiplicity.push_back(eigenSubSpace.col);
                        nowDim += eigenSubSpace.col;
                        if (std::abs(val) > PRECISION_OF_DIFFERENCE)
                        {
                            AATDim += eigenSubSpace.col;
                            len++;
                        }
                    }
                    else
                    {
                        std::cout << "The eigen value " << val << " is not correct! Maybe the precision or the minIteration is too small." << std::endl;
                        return false;
                    }
                }
                Matrix U1(U.row, AATDim);
                for (int i = 0; i < U1.row; i++)
                {
                    for (int j = 0; j < U1.col; j++)
                    {
                        U1.matrix[i][j] = U.matrix[i][j];
                    }
                }
                //U1作正交化
                int nowCol = 0;
                for (int j = 0; j < U1.col; j++)
                {
                    mod = 0;
                    for (int i = 0; i < U1.row; i++)
                    {
                        mod += U1.matrix[i][j] * U1.matrix[i][j];
                    }
                    mod = sqrt(mod);
                    for (int i = 0; i < U1.row; i++)
                    {
                        U1.matrix[i][j] /= mod; //归一化
                    }
                    mod = 0;
                    for (int k = 0; k < nowCol; k++)
                    {
                        double innerProduct = 0;
                        for (int p = 0; p < U1.row; p++)
                        {
                            innerProduct += U1.matrix[p][k] * U1.matrix[p][nowCol];
                        }
                        for (int p = 0; p < U1.row; p++)
                        {
                            U1.matrix[p][nowCol] -= innerProduct * U1.matrix[p][k];
                            mod += U1.matrix[p][nowCol] * U1.matrix[p][nowCol];
                        }
                        mod = sqrt(mod);
                        for (int p = 0; p < U1.row; p++)
                        {
                            U1.matrix[p][nowCol] /= mod; //归一化
                        }
                        nowCol++;
                    }
                }

                int now = 0;
                Matrix Sigmar(AATDim, AATDim);  //作一个Sigmar矩阵
                for (int i = 0; i < len; i++)
                {
                    for (int j = 0; j < multiplicity[i]; j++)
                    {
                        if (std::abs(values[i]) > PRECISION_OF_DIFFERENCE)
                        {
                            Sigmar.matrix[now][now] = 1 / sqrt(values[i]); //Sigmar的逆矩阵
                            now++;
                        }
                    }
                }

                now = 0;
                for (std::size_t i = 0; i < values.size(); i++)
                {
                    for (int j = 0; j < multiplicity[i]; j++)
                    {
                        if (std::abs(values[i]) > PRECISION_OF_DIFFERENCE)
                        {
                            Sigma.matrix[now][now] = sqrt(values[i]); //Sigma矩阵赋值
                            now++;
                        }
                    }
                }

                Matrix V1(A * U1 * Sigmar);

                //还要生成U的正交基
                if (U1.col < U.col)
                {
                    Matrix I(IdentityMatrix(U.row));
                    Matrix zero(U.row, 1);
                    Matrix U2(U.row, U.col - U1.col);
                    Matrix U1T(Transpose(U1));

                    int nowExpandDim = 0;
                    int iterateTimes = 0;
                    double mod;
                    while (iterateTimes < U.col - U1.col)
                    {
                        for (int i = 0; i < U.row; i++)
                        {
                            mod = 0;
                            Matrix vector(U.row, 1);
                            vector.matrix[i][0] = 1;
                            vector = (I - U1 * U1T) * vector;
                            for (int j = 0; j < nowExpandDim; j++)
                            {
                                double innerProduct = 0;
                                for (int p = 0; p < U2.row; p++)
                                {
                                    innerProduct += U2.matrix[p][j] * vector.matrix[p][0];
                                }
                                for (int p = 0; p < U1.row; p++)
                                {
                                    vector.matrix[p][0] -= innerProduct * U2.matrix[p][j];
                                }
                            }

                            if (vector != zero)
                            {
                                for (int p = 0; p < vector.row; p++)
                                {
                                    mod += vector.matrix[p][0] * vector.matrix[p][0];
                                }
                                mod = sqrt(mod);
                                for (int p = 0; p < U2.row; p++)
                                {
                                    U2.matrix[p][nowExpandDim] = vector.matrix[p][0] / mod;
                                }
                                nowExpandDim++;
                                break;
                            }
                        }
                        iterateTimes++;
                    }
                    if (nowExpandDim != U.col - U1.col)
                    {
                        std::cout << "Something went wrong that the program can't find all U's orthogonal bases!" << std::endl;
                        return false;
                    }
                    U = U1 & U2;
                }
                else U = U1;

                //生成V的正交基
                if (V1.col < V.col)  //当原矩阵不是方阵时，这里的V1还不是V，要补充正交向量
                {
                    Matrix I(IdentityMatrix(V.row));
                    Matrix zero(V.row, 1);
                    Matrix V2(V.row, V.col - V1.col);
                    Matrix V1T(Transpose(V1));

                    int nowExpandDim = 0;
                    int iterateTimes = 0;
                    double mod;
                    while (iterateTimes < V.col - V1.col)
                    {
                        for (int i = 0; i < V.row; i++)
                        {
                            mod = 0;
                            Matrix vector(V.row, 1);
                            vector.matrix[i][0] = 1;
                            vector = (I - V1 * V1T) * vector;
                            for (int j = 0; j < nowExpandDim; j++)
                            {
                                double innerProduct = 0;
                                for (int p = 0; p < V2.row; p++)
                                {
                                    innerProduct += V2.matrix[p][j] * vector.matrix[p][0];
                                }
                                for (int p = 0; p < V1.row; p++)
                                {
                                    vector.matrix[p][0] -= innerProduct * V2.matrix[p][j];
                                }
                            }

                            if (vector != zero)
                            {
                                for (int p = 0; p < vector.row; p++)
                                {
                                    mod += vector.matrix[p][0] * vector.matrix[p][0];
                                }
                                mod = sqrt(mod);
                                for (int p = 0; p < V2.row; p++)
                                {
                                    V2.matrix[p][nowExpandDim] = vector.matrix[p][0] / mod;
                                }
                                nowExpandDim++;
                                break;
                            }
                        }
                        iterateTimes++;
                    }
                    if (nowExpandDim != V.col - V1.col)
                    {
                        std::cout << "Something went wrong that the program can't find all V's orthogonal bases!" << std::endl;
                        return false;
                    }
                    V = V1 & V2;
                }
                else V = V1;
            }
            VT = Transpose(V);
            return true;
        }

        /// <summary>
        /// 简化SVD
        /// </summary>
        /// <param name="Ur"></param>
        /// <param name="Sigmar"></param>
        /// <param name="VrT"></param>
        /// <param name="precision"></param>
        /// <param name="minIteration"></param>
        /// <returns></returns>
        bool SimplifiedSVD(Matrix& Ur, Matrix& Sigmar, Matrix& VrT, double precision = PRECISION_WHEN_CALCULATING, int minIteration = 50)
        {
            if (*this == nullptr) return false;
            Matrix A(*this);
            A.TransposeDirectly(); //A是原矩阵的转置
            std::vector<double> values;
            Matrix Temp;
            if (this->col < this->row)
            {
                Temp = A * (*this);
                Temp.GetEigenValuesOfDefiniteMatrix(values, precision, minIteration, false);
            }
            else
            {
                Temp = (*this) * A;
                Temp.GetEigenValuesOfDefiniteMatrix(values, precision, minIteration, false);
            }

            int len = 0;
            if (this->col < this->row)
            {
                Matrix VrTemp(this->col, this->col);
                std::sort(values.begin(), values.end(), std::greater<double>());
                std::vector<int> multiplicity;
                Matrix eigenSubSpace;
                int nowDim = 0;
                int ATADim = 0;
                double mod = 0;

                for (const auto& val : values)
                {
                    if (std::abs(val) > PRECISION_OF_DIFFERENCE)
                    {
                        if ((Temp - val * IdentityMatrix(Temp.row)).GetBasesOfNullSpace(eigenSubSpace))
                        {
                            for (int i = 0; i < eigenSubSpace.row; i++)
                            {
                                for (int j = 0; j < eigenSubSpace.col; j++)
                                {
                                    VrTemp.matrix[i][j + nowDim] = eigenSubSpace.matrix[i][j];
                                }
                            }

                            multiplicity.push_back(eigenSubSpace.col);
                            nowDim += eigenSubSpace.col;
                            ATADim += eigenSubSpace.col;
                            len++;
                        }
                        else
                        {
                            std::cout << "The eigen value " << val << " is not correct! Maybe the precision or the minIteration is too small." << std::endl;
                            return false;
                        }
                    }
                }
                Sigmar = Matrix(ATADim, ATADim);
                Matrix Vr(this->col, ATADim);
                for (int i = 0; i < Vr.row; i++)
                {
                    for (int j = 0; j < Vr.col; j++)
                    {
                        Vr.matrix[i][j] = VrTemp.matrix[i][j];
                    }
                }
                //Vr作正交化
                int nowCol = 0;
                for (int j = 0; j < Vr.col; j++)
                {
                    mod = 0;
                    for (int i = 0; i < Vr.row; i++)
                    {
                        mod += Vr.matrix[i][j] * Vr.matrix[i][j];
                    }
                    mod = sqrt(mod);
                    for (int i = 0; i < Vr.row; i++)
                    {
                        Vr.matrix[i][j] /= mod; //归一化
                    }
                    mod = 0;
                    for (int k = 0; k < nowCol; k++)
                    {
                        double innerProduct = 0;
                        for (int p = 0; p < Vr.row; p++)
                        {
                            innerProduct += Vr.matrix[p][k] * Vr.matrix[p][nowCol];
                        }
                        for (int p = 0; p < Vr.row; p++)
                        {
                            Vr.matrix[p][nowCol] -= innerProduct * Vr.matrix[p][k];
                            mod += Vr.matrix[p][nowCol] * Vr.matrix[p][nowCol];
                        }
                        mod = sqrt(mod);
                        for (int p = 0; p < Vr.row; p++)
                        {
                            Vr.matrix[p][nowCol] /= mod; //归一化
                        }
                        nowCol++;
                    }
                }

                int now = 0;
                for (int i = 0; i < len; i++)
                {
                    for (int j = 0; j < multiplicity[i]; j++)
                    {
                        if (std::abs(values[i]) > PRECISION_OF_DIFFERENCE)
                        {
                            Sigmar.matrix[now][now] = 1 / sqrt(values[i]); //Sigmar的逆矩阵
                            now++;
                        }
                    }
                }

                Ur = (*this) * Vr * Sigmar;

                now = 0;
                for (int i = 0; i < Sigmar.row; i++)
                {
                    Sigmar.matrix[i][i] = 1.0 / Sigmar.matrix[i][i];  //Sigmar还原
                }
                VrT = Transpose(Vr);
            }
            else
            {
                Matrix UrTemp(this->row, this->row);
                std::sort(values.begin(), values.end(), std::greater<double>());
                std::vector<int> multiplicity;
                Matrix eigenSubSpace;
                int nowDim = 0;
                int AATDim = 0;
                double mod = 0;

                for (const auto& val : values)
                {
                    if (std::abs(val) > PRECISION_OF_DIFFERENCE)
                    {
                        if ((Temp - val * IdentityMatrix(Temp.row)).GetBasesOfNullSpace(eigenSubSpace))
                        {
                            for (int i = 0; i < eigenSubSpace.row; i++)
                            {
                                for (int j = 0; j < eigenSubSpace.col; j++)
                                {
                                    UrTemp.matrix[i][j + nowDim] = eigenSubSpace.matrix[i][j];
                                }
                            }

                            multiplicity.push_back(eigenSubSpace.col);
                            nowDim += eigenSubSpace.col;
                            AATDim += eigenSubSpace.col;
                            len++;
                        }
                        else
                        {
                            std::cout << "The eigen value " << val << " is not correct! Maybe the precision or the minIteration is too small." << std::endl;
                            return false;
                        }
                    }
                }
                Sigmar = Matrix(AATDim, AATDim);
                Ur = Matrix(this->row, AATDim);
                for (int i = 0; i < Ur.row; i++)
                {
                    for (int j = 0; j < Ur.col; j++)
                    {
                        Ur.matrix[i][j] = UrTemp.matrix[i][j];
                    }
                }
                //Ur作正交化
                int nowCol = 0;
                for (int j = 0; j < Ur.col; j++)
                {
                    mod = 0;
                    for (int i = 0; i < Ur.row; i++)
                    {
                        mod += Ur.matrix[i][j] * Ur.matrix[i][j];
                    }
                    mod = sqrt(mod);
                    for (int i = 0; i < Ur.row; i++)
                    {
                        Ur.matrix[i][j] /= mod; //归一化
                    }
                    mod = 0;
                    for (int k = 0; k < nowCol; k++)
                    {
                        double innerProduct = 0;
                        for (int p = 0; p < Ur.row; p++)
                        {
                            innerProduct += Ur.matrix[p][k] * Ur.matrix[p][nowCol];
                        }
                        for (int p = 0; p < Ur.row; p++)
                        {
                            Ur.matrix[p][nowCol] -= innerProduct * Ur.matrix[p][k];
                            mod += Ur.matrix[p][nowCol] * Ur.matrix[p][nowCol];
                        }
                        mod = sqrt(mod);
                        for (int p = 0; p < Ur.row; p++)
                        {
                            Ur.matrix[p][nowCol] /= mod; //归一化
                        }
                        nowCol++;
                    }
                }

                int now = 0;
                for (int i = 0; i < len; i++)
                {
                    for (int j = 0; j < multiplicity[i]; j++)
                    {
                        if (std::abs(values[i]) > PRECISION_OF_DIFFERENCE)
                        {
                            Sigmar.matrix[now][now] = 1 / sqrt(values[i]); //Sigmar的逆矩阵
                            now++;
                        }
                    }
                }

                VrT = Transpose(A * Ur * Sigmar);

                now = 0;
                for (int i = 0; i < Sigmar.row; i++)
                {
                    Sigmar.matrix[i][i] = 1.0 / Sigmar.matrix[i][i];  //Sigmar还原
                }
            }
            return true;
        }

        /// <summary>
        /// 求任意矩阵的Moore-Penrose广义逆
        /// </summary>
        /// <param name="precision"></param>
        /// <param name="minIteration"></param>
        /// <returns>若计算过程中无错误，则返回广义逆；否则返回nullptr</returns>
        Matrix MoorePenrosGeneralizedInverse(double precision = PRECISION_WHEN_CALCULATING, int minIteration = 50)
        {
            TCL_MATRIX_UNUSED_PARAMETER(precision);
            TCL_MATRIX_UNUSED_PARAMETER(minIteration);
            Matrix UrT, Vr, SigmarInverse, AT(Transpose((*this)));
            if (AT.SimplifiedSVD(Vr, SigmarInverse, UrT))
            {
                for (int i = 0; i < SigmarInverse.row; i++)
                {
                    SigmarInverse.matrix[i][i] = 1 / SigmarInverse.matrix[i][i];
                }
                return Vr * SigmarInverse * UrT;
            }
            else
            {
                return nullptr;
            }
        }

        /// <summary>
        /// 求行/列满秩矩阵的Moore-Penrose广义逆
        /// </summary>
        /// <param name="judgeRank">是否判断矩阵行满秩或列满秩</param>
        /// <returns>若计算过程中无错误，则返回广义逆；否则返回nullptr</returns>
        Matrix MPGeneralizedInverseForFullRankMatrix(bool judgeRank = true)
        {
            int theoreticRank = row < col ? row : col;
            if (judgeRank)
            {
                int r = GetRank();
                if (r != theoreticRank)
                {
                    std::cout << "The matrix is not a full-rank matrix. Return nullptr" << std::endl;
                    return nullptr;
                }
            }
            if (col < row)
            {
                Matrix AT(Transpose(*this));
                Matrix inv(AT * (*this));
                if (inv.InverseMatrix(inv))
                {
                    return inv * AT;
                }
                else
                {
                    std::cout << "The matrix is not a full-rank matrix. Return nullptr" << std::endl;
                    return nullptr;
                }
            }
            else
            {
                Matrix AT(Transpose(*this));
                Matrix inv((*this) * AT);
                if (inv.InverseMatrix(inv))
                {
                    return AT * inv;
                }
                else
                {
                    std::cout << "The matrix is not a full-rank matrix. Return nullptr" << std::endl;
                    return nullptr;
                }
            }
        }
       
        /// <summary>
        /// 矩阵乘法
        /// </summary>
        /// <param name="A"></param>
        /// <param name="B"></param>
        /// <returns>若维度失配，则返回第一个操作数</returns>
        friend Matrix operator *(const Matrix& A, const Matrix& B)
        {
            if (A.col != B.row)
            {
                std::cout << "The dimension does not match! Matrix multiplication failed. Return the first matrix." << std::endl;
                return A;
            }
            Matrix ret(A.row, B.col);
            for (int i = 0; i < ret.row; i++)
            {
                for (int j = 0; j < ret.col; j++)
                {
                    for (int k = 0; k < A.col; k++)
                    {
                        ret.matrix[i][j] += (A.matrix[i][k] * B.matrix[k][j]);
                    }
                }
            }
            return ret;
        }

        void operator *=(const Matrix& B)
        {
            if (col != B.row)
            {
                std::cout << "The dimension does not match! Matrix multiplication and equality failed. Nothing done." << std::endl;
                return;
            }

            Matrix ret(row, B.col);
            for (int i = 0; i < ret.row; i++)
            {
                for (int j = 0; j < ret.col; j++)
                {
                    for (int k = 0; k < col; k++)
                    {
                        ret.matrix[i][j] += (matrix[i][k] * B.matrix[k][j]);
                    }
                }
            }
            (*this) = ret;
        }

        /// <summary>
        /// 矩阵数乘
        /// </summary>
        /// <param name="k"></param>
        /// <param name="A"></param>
        /// <returns></returns>
        friend Matrix operator*(double k, const Matrix& A)
        {
            Matrix ret(A.row, A.col);
            for (int i = 0; i < ret.row; i++)
            {
                for (int j = 0; j < ret.col; j++)
                {
                    ret.matrix[i][j] = k * A.matrix[i][j];
                }
            }
            return ret;
        }

        /// <summary>
        /// 矩阵加法
        /// </summary>
        /// <param name="A"></param>
        /// <param name="B"></param>
        /// <returns>若维度失配，则返回第一个操作数</returns>
        friend Matrix operator +(const Matrix& A, const Matrix& B)
        {
            if (A.col != B.col || A.row != B.row)
            {
                std::cout << "The dimension does not match! Matrix addtion failed. Return the first matrix." << std::endl;
                return A;
            }
            Matrix ret(A.row, B.row);
            for (int i = 0; i < ret.row; i++)
            {
                for (int j = 0; j < ret.col; j++)
                {
                    ret.matrix[i][j] += A.matrix[i][j] + B.matrix[i][j];
                }
            }
            return ret;
        }

        void operator +=(const Matrix& B)
        {
            if (col != B.col || row != B.row)
            {
                std::cout << "The dimension does not match! Matrix addtion and equality failed. Nothing done." << std::endl;
                return;
            }
            for (int i = 0; i < row; i++)
            {
                for (int j = 0; j < col; j++)
                {
                    matrix[i][j] += B.matrix[i][j];
                }
            }
        }

        /// <summary>
        /// 矩阵减法
        /// </summary>
        /// <param name="A"></param>
        /// <param name="B"></param>
        /// <returns>若维度失配，则返回第一个操作数</returns>
        friend Matrix operator -(const Matrix& A, const Matrix& B)
        {
            if (A.col != B.col || A.row != B.row)
            {
                std::cout << "The dimension does not match! Matrix subtraction failed. Return the first matrix." << std::endl;
                return A;
            }
            Matrix ret(A.row, A.col);
            for (int i = 0; i < ret.row; i++)
            {
                for (int j = 0; j < ret.col; j++)
                {
                    ret.matrix[i][j] += A.matrix[i][j] - B.matrix[i][j];
                }
            }
            return ret;
        }

        void operator -=(const Matrix& B)
        {
            if (col != B.col || row != B.row)
            {
                std::cout << "The dimension does not match! Matrix subtraction and equality failed. Nothing done." << std::endl;
                return;
            }
            for (int i = 0; i < row; i++)
            {
                for (int j = 0; j < col; j++)
                {
                    matrix[i][j] -= B.matrix[i][j];
                }
            }
        }

        /// <summary>
        /// 矩阵相等判断
        /// </summary>
        /// <param name="A"></param>
        /// <param name="B"></param>
        /// <returns></returns>
        friend bool operator ==(const Matrix& A, const Matrix& B)
        {
            if (A.row != B.row)
                return false;
            if (A.col != B.col)
                return false;
            for (int i = 0; i < A.row; i++)
            {
                for (int j = 0; j < A.col; j++)
                {
                    if (std::abs(A.matrix[i][j] - B.matrix[i][j]) > PRECISION_OF_DIFFERENCE)
                        return false;
                }
            }
            return true;
        }

        bool operator ==(std::nullptr_t) const
        {
            return this->row == 0;
        }

        /// <summary>
        /// 矩阵不等判断
        /// </summary>
        /// <param name="A"></param>
        /// <param name="B"></param>
        /// <returns></returns>
        friend bool operator !=(const Matrix& A, const Matrix& B)
        {
            return !(A == B);
        }

        bool operator !=(std::nullptr_t nop) const
        {
            return !(*this == nop);
        }

        /// <summary>
        /// 矩阵横向合并
        /// </summary>
        /// <param name="A"></param>
        /// <param name="B"></param>
        /// <returns></returns>
        friend Matrix operator &(const Matrix& A, const Matrix& B)
        {
            if (A.row != B.row)
            {
                std::cout << "A and B don't have the same rows! Return the first matrix." << std::endl;
                return A;
            }
            else
            {
                Matrix ret(A.row, A.col + B.col);
                for (int i = 0; i < A.row; i++)
                {
                    for (int j = 0; j < A.col; j++)
                    {
                        ret.matrix[i][j] = A.matrix[i][j];
                    }
                }
                for (int i = 0; i < B.row; i++)
                {
                    for (int j = 0; j < B.col; j++)
                    {
                        ret.matrix[i][j + A.col] = B.matrix[i][j];
                    }
                }
                return ret;
            }
        }

        /// <summary>
        /// 矩阵赋值
        /// </summary>
        /// <param name="A"></param>
        /// <returns></returns>
        Matrix& operator =(const Matrix& A)
        {
            if (&A == this) return *this;

            if (A.col != col || A.row != row)
            {
                for (int i = 0; i < row; i++)
                    delete[] matrix[i];
                delete[] matrix;
                row = A.row;
                col = A.col;
                if (A == nullptr)
                {
                    matrix = nullptr;
                }
                else
                {
                    matrix = new double* [row];
                    for (int i = 0; i < row; i++)
                        matrix[i] = new double[col];
                }
            }
            for (int i = 0; i < row; i++)
            {
                for (int j = 0; j < col; j++)
                {
                    matrix[i][j] = A.matrix[i][j];
                }
            }
            return *this;
        }

        /// <summary>
        /// 移动赋值
        /// </summary>
        /// <param name="A"></param>
        /// <returns></returns>
        Matrix& operator =(Matrix&& A) noexcept
        {
            if (&A == this) return *this;
            if (A == nullptr)
            {
                row = col = 0;
                matrix = nullptr;
                return *this;
            }

            row = A.row;
            col = A.col;
            matrix = A.matrix;
            A.row = A.col = 0;
            A.matrix = nullptr;

            return *this;
        }

        /// <summary>
        /// 获得列向量
        /// </summary>
        /// <param name="column"></param>
        /// <returns></returns>
        Matrix operator [](int column)
        {
            if (column >= col || column < 0)
            {
                std::cout << "Index out of range! Return nullptr." << std::endl;
                return nullptr;
            }
            else
            {
                Matrix ret(row, 1);
                for (int i = 0; i < row; i++)
                {
                    ret.matrix[i][column] = matrix[i][column];
                }
                return ret;
            }
        }

        /// <summary>
        /// 矩阵的幂，矩阵必须为方阵
        /// </summary>
        /// <param name="n">必须是自然数</param>
        /// <returns>若矩阵不为方阵或参数小于0，则返回原矩阵</returns>
        Matrix Power(int n)
        {
            if (row != col)
            {
                std::cout << "The matrix is not a square!" << std::endl;
                return *this;
            }
            else if (n < 0)
            {
                std::cout << "The specified power is less than 0! No changes!" << std::endl;
                return *this;
            }
            else if (*this == nullptr)
            {
                return nullptr;
            }
            else
            {
                Matrix ori(*this), ret(row, row);
                for (int i = 0; i < row; i++)
                    ret.matrix[i][i] = 1;
                while (n > 0)
                {
                    if (n % 2)
                    {
                        ret *= ori;
                        n -= 1;
                        ori *= ori;
                        n /= 2;
                    }
                    else
                    {
                        ori *= ori;
                        n /= 2;
                    }
                }
                return ret;
            }
        }

        /// <summary>
        /// 计算矩阵的秩
        /// </summary>
        /// <returns></returns>
        int GetRank()
        {
            if (*this == nullptr) return 0;
            if (row != col)
                return 0;
            Matrix mtemp(*this);
            int rank = 0;
            int zeroRow = 0;
            for (int i = 0; i < col && i < row; i++)  //i看成列
            {
                int nonZero = row - 1;  //先设非零行为最后一行
                for (int j = i - zeroRow; j < row; j++) // 找到非0元素
                    if (std::abs(mtemp.matrix[j][i]) > PRECISION_OF_DIFFERENCE)
                    {
                        nonZero = j;
                        break;
                    }

                if (std::abs(mtemp.matrix[nonZero][i]) > PRECISION_OF_DIFFERENCE)
                {
                    if (nonZero != i - zeroRow) //如果非0元素不是第i-zeroRow行
                    {
                        for (int k = i; k < col; k++) // 把非0元素所在行交换到当前行
                        {
                            double t = mtemp.matrix[i - zeroRow][k];
                            mtemp.matrix[i - zeroRow][k] = mtemp.matrix[nonZero][k];
                            mtemp.matrix[nonZero][k] = t;
                        }
                        nonZero = i - zeroRow;
                    }

                    for (int j = i - zeroRow + 1; j < row; j++) //把下面部分消为0
                    {
                        if (std::abs(mtemp.matrix[j][i]) > PRECISION_OF_DIFFERENCE) //如果mtemp.matrix[j][i]不是0
                        {
                            double temp = mtemp.matrix[j][i] / mtemp.matrix[i - zeroRow][i];
                            for (int k = i; k < col; k++)
                            {
                                mtemp.matrix[j][k] = mtemp.matrix[j][k] - mtemp.matrix[i - zeroRow][k] * temp;
                            }
                        }
                    }
                }
            }
            for (int i = 0; i < row; i++)
            {
                if (std::abs(mtemp.matrix[i][i]) > PRECISION_OF_DIFFERENCE)
                    rank++;
                else break;
            }
            return rank; 
        }

        int GetRow() const
        {
            return row;
        }

        int GetCol() const
        {
            return col;
        }

        /// <summary>
        /// 获得矩阵的所有奇异值，不含重根
        /// </summary>
        /// <param name="v">用于接收奇异值</param>
        /// <param name="precision">迭代精度</param>
        /// <param name="minIteration">最小迭代次数</param>
        void GetAllSingularValues(std::vector<double>& v, double precision = PRECISION_WHEN_CALCULATING, int minIteration = 50)
        {
            Matrix T = Transpose(*this);
            int len;
            if (row < col)
            {
                len = row;
                T = *this * T;
            }
            else
            {
                len = col;
                T *= (*this);
            }
            T.GetEigenValuesOfDefiniteMatrix(v, precision, minIteration, false);
            for (auto& i : v)
            {
                if (i >= 0)
                    i = sqrt(i);
                else
                {
                    std::cout << "Something went wrong that the singular value is negative!" << std::endl;
                }
            }
        }

        /// <summary>
        /// 计算对称非不定矩阵的所有特征值，不含重根
        /// </summary>
        /// <param name="v">用于接收特征值。若矩阵对称非不定，则算法保证计算出所有特征值，但在接受向量中的顺序不定；若矩阵不对称或不为方阵，则如果进行判断，v中会添加一个DBL_MAX元素；若矩阵对称但不定，其结果正确性不作保证</param>
        /// <param name="precision">迭代精度</param>
        /// <param name="minIteration">最小迭代次数</param>
        /// <param name="judgeSymmetry">是否对矩阵对称性或是否为方阵进行判断</param>
        void GetEigenValuesOfDefiniteMatrix(std::vector<double>& v, double precision = PRECISION_WHEN_CALCULATING, int minIteration = 50, bool judgeSymmetry = true)
        {
            v.clear();
            if (judgeSymmetry)
            {
                if (row != col)
                {
                    std::cout << "The matrix is not a square!" << std::endl;
                    v.push_back(DBL_MAX);
                    return;
                }
                for (int i = 0; i < row; i++)
                {
                    for (int j = i + 1; j < col; j++)
                    {
                        if (std::abs(matrix[i][j] - matrix[j][i]) > PRECISION_OF_DIFFERENCE)
                        {
                            std::cout << "The matrix is not symmetric!" << std::endl;
                            v.push_back(DBL_MAX);
                            return;
                        }
                    }
                }
            }
            std::vector<Matrix> eigenVector;
            Matrix A(row, col);
            double now, last;
            int iteration;
            int multipleRootNum = 0;
            if (!InverseMatrix(A))
                v.push_back(0);
            A = *this;
            //用幂法求矩阵绝对值最大特征值，并不断迭代
            {
                do
                {
                    Matrix vector(row, 1);
                    for (int i = 0; i < row; i++)
                    {
                        vector.matrix[i][0] = 1;
                    }
                    Matrix zero(row, 1);
                    int p = 0;
                    while ((zero == A * vector) && p < row)
                    {
                        vector.matrix[p++][0] = 0;  //防止A*vector得到零向量
                    }
                    if (p >= row)
                        return;
                    vector = A * vector;
                    int maxpos = 0; //最大位置
                    for (int i = 0; i < row; i++)
                    {
                        if (std::abs(vector.matrix[i][0]) > std::abs(vector.matrix[maxpos][0]))
                            maxpos = i;
                    }
                    last = vector.matrix[maxpos][0];
                    now = last;
                    iteration = 0; //归0迭代次数
                    do
                    {
                        last = now;
                        for (int i = 0; i < row; i++)
                        {
                            vector.matrix[i][0] /= last;
                        }
                        vector = A * vector;
                        for (int i = 0; i < row; i++)
                        {
                            if (std::abs(vector.matrix[i][0]) > std::abs(vector.matrix[maxpos][0]))
                                maxpos = i;
                        }
                        now = vector.matrix[maxpos][0];
                        iteration++;
                    } while (iteration < minIteration || std::abs(now - last) > precision);

                    bool isExist = false; //特征值是否已经计算出来
                    for (auto& i : v)
                    {
                        if (std::abs(now - i) < PRECISION_OF_DIFFERENCE)
                        {
                            isExist = true;
                            multipleRootNum++;
                            break;
                        }
                    }
                    if (!isExist)
                    {
                        v.push_back(now);
                    }
                    for (const auto& eigen : eigenVector)
                    {
                        vector = vector - (Transpose(vector) * eigen).matrix[0][0] * eigen; //正交化
                    }
                    double mod = 0;
                    for (int i = 0; i < row; i++)
                    {
                        mod += (vector.matrix[i][0] * vector.matrix[i][0]);
                    }
                    mod = sqrt(mod);
                    if (vector.matrix[maxpos][0] > 0)
                        vector = (1 / mod) * vector; //归一化
                    else
                        vector = (-1 / mod) * vector;
                    A = A - now * (vector * Transpose(vector));
                    //防止出现零矩阵
                    bool isZero = true;
                    for (int i = 0; i < A.row && isZero; i++)
                    {
                        for (int j = 0; j < A.col; j++)
                        {
                            if (std::abs(A.matrix[i][j]) > PRECISION_OF_DIFFERENCE)
                            {
                                isZero = false;
                                break;
                            }

                        }
                    }
                    if (isZero)
                        break;
                    eigenVector.push_back(vector);
                } while (v.size() < row - multipleRootNum);
            }
        }

        /// <summary>
        /// 获得方阵特征多项式的系数
        /// </summary>
        /// <param name="v">若矩阵不为方阵，返回false，vector未进行任何操作</param>
        bool GetCoefficientsOfCharacteristicPolynomial(std::vector<double>& v) const
        {
            if (col != row)
            {
                std::cout << "The matrix is not a square so its characteristic polynomial is not defined." << std::endl;
                return false;
            }
            v.clear();
            double* tr = new double[col + 1];
            double* co = new double[col + 1];
            Matrix A(*this);
            for (int i = 1; i <= col; i++)
            {
                tr[i] = A.Trace();
                if (i != col)
                    A *= (*this);
            }
            co[col] = 1;
            for (int i = col - 1; i >= 0; i--)
            {
                co[i] = 0;
                for (int j = col - i; j >= 1; j--)
                {
                    co[i] += (co[i + j] * tr[j]);
                }
                co[i] /= (i - col);
            }
            for (int i = 0; i < col; i++)
            {
                v.push_back(co[i]);
            }
            v.push_back(co[col]);
            delete[]co;
            delete[]tr;
            return true;
        }

        /// <summary>
        /// 计算矩阵的所有（复）特征值，含重根
        /// </summary>
        /// <param name="possibleMaxMultiplicity">特征值可能的最大重数，该参数非常影响结果的准确性</param>
        /// <param name="v">接收解的vector</param>
        /// <param name="aberthIteration">Aberth迭代次数</param>
        /// <param name="newtonIteration">牛顿迭代次数</param>
        /// <param name="possibleMaxMod">特征值可能的最大模长</param>
        void GetAllEigenValues(std::vector<std::complex<double>>& v, int possibleMaxMultiplicity = 5, int aberthIteration = 700, int newtonIteration = 5, int possibleMaxMod = 150) const
        {
            v.clear();
            if (col != row)
            {
                std::cout << "The matrix is not a square so it doesn't have eigenvalues." << std::endl;
                return;
            }
            std::vector<double> coeff;
            if (!GetCoefficientsOfCharacteristicPolynomial(coeff))
            {
                return;
            }
            double** poly = new double* [possibleMaxMultiplicity + 1];
            for (int i = 0; i < possibleMaxMultiplicity + 1; i++)
            {
                poly[i] = new double[col + 1];
            }
            for (int i = 0; i < col + 1; i++)
            {
                poly[0][i] = coeff[i];
            }
            for (int i = 1; i < possibleMaxMultiplicity + 1; i++)
            {
                for (int j = 0; j < col + 1 - i; j++)
                {
                    poly[i][j] = poly[i - 1][j + 1] * (j + 1);
                }
            }

            std::complex<double>* roots = new std::complex<double>[col];
            std::complex<double>* multiroots = new std::complex<double>[col];
            int* multiplicity = new int[col];
            int multirootnum = 0;
            for (int i = 0; i < col; i++)
            {
                roots[i] = std::complex<double>(rand() % (2 * possibleMaxMod) - possibleMaxMod, rand() % (2 * possibleMaxMod) - possibleMaxMod);
            }

            //Aberth
            std::complex<double>* w = new std::complex<double>[col];
            const std::complex<double> one{ 1,0 };
            const std::complex<double> zero{ 0,0 };
            std::complex<double> sum;
            while (aberthIteration > 0)
            {
                aberthIteration--;
                for (int i = 0; i < col; i++)
                {
                    sum = zero;
                    std::complex<double> rate = PolynomialFunc(poly[0], col, roots[i]) / PolynomialFunc(poly[1], col - 1, roots[i]);
                    for (int j = 0; j < col; j++)
                    {
                        if (j != i)
                        {
                            sum += (one / (roots[i] - roots[j]));
                        }
                    }
                    w[i] = rate / (one - rate * sum);
                }
                for (int i = 0; i < col; i++)
                {
                    roots[i] -= w[i];
                }
            }
            delete[]w;

            {
                int i, j, max = 0;
                std::complex<double> temp;
                for (i = 0; i < col - 1; i++)
                {
                    max = i;
                    for (j = i + 1; j < col; j++)
                    {
                        if (Greater(roots[j], roots[max]))
                        {
                            max = j;
                        }
                    }
                    temp = roots[i];
                    roots[i] = roots[max];
                    roots[max] = temp;
                }
            }

            for (int i = 0; i < col; i++)
                multiplicity[i] = 0;
            for (int i = 0; i < col; i++)
            {
                int x = -1;
                for (int j = 0; j < multirootnum; ++j)
                {
                    if (std::abs(multiroots[j] - roots[i]) < PRECISION_OF_DIFFERENCE * 20) //这里的精度不能太大
                    {
                        x = j;
                        break;
                    }
                }
                if (x != -1)
                {
                    multiplicity[x]++;
                }
                else
                {
                    multiroots[multirootnum] = roots[i];
                    multiplicity[multirootnum] = 1;
                    multirootnum++;
                }
            }

            //Newton
            {
                std::complex<double> z0, z1;
                for (int i = 0; i < multirootnum; i++)
                {
                    for (int j = 1; j <= possibleMaxMultiplicity; j++)
                    {
                        if (j == multiplicity[i])
                        {
                            for (int k = 0; k < newtonIteration; k++)
                            {
                                multiroots[i] = multiroots[i] - PolynomialFunc(poly[j - 1], col - j + 1, multiroots[i]) / PolynomialFunc(poly[j], col - j, multiroots[i]);
                            }
                            break;
                        }
                        else if (j > possibleMaxMultiplicity)
                        {
                            for (int k = 0; k < newtonIteration; k++)
                            {
                                multiroots[i] = multiroots[i] - PolynomialFunc(poly[possibleMaxMultiplicity - 1], col - possibleMaxMultiplicity + 1, multiroots[i]) / PolynomialFunc(poly[possibleMaxMultiplicity], col - possibleMaxMultiplicity, multiroots[i]);
                            }
                            break;
                        }
                    }
                }
                int x = 0;
                for (int i = 0; i < multirootnum; i++)
                {
                    for (int j = 0; j < multiplicity[i]; j++)
                    {
                        roots[x + j] = multiroots[i];
                    }
                    x += multiplicity[i];
                }
            }

            for (int i = 0; i < col; i++)
            {
                if (std::abs(roots[i].imag()) < 0.1 * PRECISION_OF_DIFFERENCE)
                {
                    v.push_back(roots[i]);
                }
                else if (roots[i].imag() >= 0)
                {
                    v.push_back(std::complex<double>{ roots[i].real(), std::abs(roots[i].imag())});
                    v.push_back(std::complex<double>{ roots[i].real(), -std::abs(roots[i].imag())});
                }
            }

            delete[]multiroots;
            delete[]multiplicity;
            delete[]roots;
            for (int i = 0; i < possibleMaxMultiplicity + 1; i++)
            {
                delete[]poly[i];
            }
            delete[]poly;
        }

        /// <summary>
        /// 产生一个单位矩阵
        /// </summary>
        /// <param name="n">矩阵维度</param>
        /// <returns></returns>
        static Matrix IdentityMatrix(int n)
        {
            Matrix I(n, n);
            for (int i = 0; i < n; i++)
            {
                I.matrix[i][i] = 1;
            }
            return I; //会调用复制构造函数，因此效率比直接构造要低；此函数只是为了方便
        }

        /// <summary>
        /// 矩阵谱范数
        /// </summary>
        /// <param name="precision"></param>
        /// <param name="minIteration"></param>
        /// <returns></returns>
        double SpectralNorm(double precision = PRECISION_WHEN_CALCULATING, int minIteration = 10) const
        {
            Matrix T = Transpose(*this);
            int len;
            if (row < col)
            {
                len = row;
                T = *this * T;
            }
            else
            {
                len = col;
                T *= (*this);
            }
            Matrix vector(T.col, 1);
            for (int i = 0; i < T.col; i++)
            {
                vector.matrix[i][0] = 1;
            }

            vector = T * vector;
            int maxpos = 0; //最大位置
            for (int i = 0; i < T.col; i++)
            {
                if (std::abs(vector.matrix[i][0]) > std::abs(vector.matrix[maxpos][0]))
                    maxpos = i;
            }
            double last = vector.matrix[maxpos][0];
            double now = last;
            int iteration = 0; //归0迭代次数
            do
            {
                last = now;
                for (int i = 0; i < T.col; i++)
                {
                    vector.matrix[i][0] /= last;
                }
                vector = T * vector;
                for (int i = 0; i < T.col; i++)
                {
                    if (std::abs(vector.matrix[i][0]) > std::abs(vector.matrix[maxpos][0]))
                        maxpos = i;
                }
                now = vector.matrix[maxpos][0];
                iteration++;
            } while (iteration < minIteration || std::abs(now - last) > precision);
            return sqrt(now);
        }

        /// <summary>
        /// 求方阵谱半径
        /// </summary>
        /// <param name="precision"></param>
        /// <param name="minIteration"></param>
        /// <returns></returns>
        double SpectralRadius(double precision = PRECISION_WHEN_CALCULATING, int minIteration = 10)const
        {
            if (row != col)
            {
                std::cout << "The matrix is not a square so its spectral radius is not defined! Return DBL_MAX." << std::endl;
                return DBL_MAX;
            }
            Matrix T(*this);
            Matrix vector(T.col, 1);
            for (int i = 0; i < T.col; i++)
            {
                vector.matrix[i][0] = 1;
            }

            vector = T * vector;
            int maxpos = 0; //最大位置
            for (int i = 0; i < T.col; i++)
            {
                if (std::abs(vector.matrix[i][0]) > std::abs(vector.matrix[maxpos][0]))
                    maxpos = i;
            }
            double last = vector.matrix[maxpos][0];
            double now = last;
            int iteration = 0; //归0迭代次数
            do
            {
                last = now;
                for (int i = 0; i < T.col; i++)
                {
                    vector.matrix[i][0] /= last;
                }
                vector = T * vector;
                for (int i = 0; i < T.col; i++)
                {
                    if (std::abs(vector.matrix[i][0]) > std::abs(vector.matrix[maxpos][0]))
                        maxpos = i;
                }
                now = vector.matrix[maxpos][0];
                iteration++;
            } while (iteration < minIteration || std::abs(now - last) > precision);
            return now;
        }

        /// <summary>
        /// 计算方阵的迹
        /// </summary>
        /// <returns>若矩阵不是方阵，则返回DBL_MAX</returns>
        double Trace() const
        {
            if (row != col)
            {
                std::cout << "The matrix is not a square so trace is not defined." << std::endl;
                return DBL_MAX;
            }
            double tr = 0;
            for (int i = 0; i < row; i++)
            {
                tr += matrix[i][i];
            }
            return tr;
        }

        /// <summary>
        /// 打印矩阵
        /// </summary>
        /// <param name="width">指定输出宽度，默认为3</param>
        /// <param name="precision">指定输出精度，默认为3</param>
        void Display(int width = 3, int precision = 3) const
        {
            for (int i = 0; i < row; i++)
                for (int j = 0; j < col; j++)
                {
                    std::cout << std::setprecision(precision) << std::setw(width) << matrix[i][j] << " ";
                    if (j == col - 1)
                        std::cout << std::endl;
                }
            std::cout << std::endl;
        }

        /// <summary>
        /// 从文件中读取矩阵
        /// </summary>
        /// <param name="str"></param>
        /// <param name="ret"></param>
        /// <returns></returns>
        friend bool ReadFromFile(const char* str, Matrix& ret)
        {
            std::ifstream in;
            in.open(str, std::ios_base::in);
            if (in.is_open())
            {
                int col, row;
                in >> row >> col;
                ret = Matrix(row, col);
                for (int i = 0; i < row; i++)
                {
                    for (int j = 0; j < col; j++)
                    {
                        in >> ret.matrix[i][j];
                    }
                }
                in.close();
                return true;
            }
            else return false;
        }

        /// <summary>
        /// 向文件中保存矩阵
        /// </summary>
        /// <param name="str"></param>
        /// <param name="ret"></param>
        /// <param name="width"></param>
        friend void WriteToFile(const char* str, const Matrix& ret, int width = 3)
        {
            std::ofstream of;
            of.open(str, std::ios_base::out);
            of << ret.row << std::setw(width) << ret.col << std::endl << std::setw(width);
            for (int i = 0; i < ret.row; i++)
            {
                for (int j = 0; j < ret.col; j++)
                {
                    of << ret.matrix[i][j] << std::setw(3);
                }
                of << std::endl;
            }
            of.close();
        }
        
        class const_row_vector_iterator
        {
        public:
            class const_row_vector_reference
            {
            public:
                using const_iterator = const double*;

                const_iterator cbegin() const
                {
                    return ptr;
                }

                const_iterator cend() const
                {
                    return end_ptr;
                }

                const_iterator begin() const
                {
                    return this->cbegin();
                }

                const_iterator end() const
                {
                    return this->cend();
                }

                const_row_vector_reference(const const_row_vector_reference&) = default;
                const_row_vector_reference& operator=(const const_row_vector_reference&) = default;
                
            protected:
                const_row_vector_reference() = default;
                const_row_vector_reference(double* ptr, double* end_ptr) : ptr(ptr), end_ptr(end_ptr) {}

                double* ptr = nullptr;
                double* end_ptr = nullptr;

                friend class const_row_vector_iterator;
            };

            using value_type = const_row_vector_reference;

            const_row_vector_iterator() = default;
            const_row_vector_iterator(const const_row_vector_iterator&) = default;
            const_row_vector_iterator& operator=(const const_row_vector_iterator&) = default;

            explicit const_row_vector_iterator(double** ptr, int col) : ptr(ptr), col(col) {}

            bool operator==(const const_row_vector_iterator& op) const
            {
                return this->ptr == op.ptr;
            }

            bool operator!=(const const_row_vector_iterator& op) const
            {
                return !(*this == op);
            }
            
            value_type operator*() const
            {
                return const_row_vector_reference(*ptr, *ptr + col);
            }

            const_row_vector_iterator& operator++()
            {
                ++this->ptr;
                return *this;
            }

            const_row_vector_iterator operator++(int)
            {
                auto org = *this;
                this->operator++();
                return org;
            }

        protected:
            double** ptr = nullptr;
            int col = 0;
        };

        class row_vector_iterator final : public const_row_vector_iterator
        {
        public:

            class row_vector_reference final : public const_row_vector_reference
            {
            public:
                using const_row_vector_reference::begin;
                using const_row_vector_reference::end;
                using const_row_vector_reference::cbegin;
                using const_row_vector_reference::cend;
                using const_row_vector_reference::const_iterator;
                using iterator = double*;

                row_vector_reference& operator=(const row_vector_reference& op)
                {
                    this->const_row_vector_reference::operator=(op);
                    return *this;
                }

                row_vector_reference(const row_vector_reference&) = default;

                iterator begin()
                {
                    return ptr;
                }

                iterator end()
                {
                    return end_ptr;
                }

            protected:
                row_vector_reference() = default;
                row_vector_reference(double* ptr, double* end_ptr) : const_row_vector_reference(ptr, end_ptr) {}
                friend class row_vector_iterator;
            };

            using const_row_vector_iterator::operator==;
            using const_row_vector_iterator::operator!=;
            
            using value_type = row_vector_reference;

            row_vector_iterator() = default;
            row_vector_iterator(const row_vector_iterator& op) = default;

            row_vector_iterator& operator=(const row_vector_iterator& op)
            {
                this->const_row_vector_iterator::operator=(op);
                return *this;
            }

            explicit row_vector_iterator(double** ptr, int col) : const_row_vector_iterator(ptr, col) {}

            row_vector_reference operator*()
            {
                return row_vector_reference(*ptr, *ptr + col);
            }

            row_vector_iterator& operator++()
            {
                this->const_row_vector_iterator::operator++();
                return *this;
            }

            row_vector_iterator& operator++(int)
            {
                auto origin = *this;
                this->operator++();
                return origin;
            }
        };

        const_row_vector_iterator cbegin() const
        {
            return const_row_vector_iterator(this->matrix, this->col);
        }

        const_row_vector_iterator cend() const
        {
            return const_row_vector_iterator(this->matrix + this->row, this->col);
        }

        const_row_vector_iterator begin() const
        {
            return this->cbegin();
        }

        const_row_vector_iterator end() const
        {
            return this->cend();
        }

        row_vector_iterator begin()
        {
            return row_vector_iterator(this->matrix, this->col);
        }

        row_vector_iterator end()
        {
            return row_vector_iterator(this->matrix + this->row, this->col);
        }
    };

    /// <summary>
    /// 异常类，暂时可能用不到
    /// </summary>
    class Exception
    {
    protected:
        std::string str;
    public:
        Exception(const std::string& str) :str(str) {}
        void Show()
        {
            std::cout << str << std::endl;
        }
    };
}

#undef TCL_MATRIX_UNUSED_PARAMETER

#endif
