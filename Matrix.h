#ifndef MATRIX_H
#define MATRIX_H
#pragma once
#include<iostream>
#include<cmath>
#include<iomanip>
#pragma warning(disable:4996)
#define PRECISION 1e-5

class Matrix
{
public:
    int row;
    int col;
    double** m;
    int rank;

    /// <summary>
    /// 将已有矩阵深复制到对象矩阵中，初始时不知道秩，令秩为-1。
    /// </summary>
    /// <param name="matrix">使用时，可以将double**类型强转为double*类型输入</param>
    /// <param name="row"></param>
    /// <param name="col"></param>
    Matrix(double* matrix, int row, int col) :row(row), col(col), rank(-1)
    {
        m = new double* [row];
        for (int i = 0; i < row; i++)
            m[i] = new double[col];
        for (int i = 0; i < row; i++)
            for (int j = 0; j < col; j++)
            {
                m[i][j] = matrix[col * i + j];
            }
    }

    /// <summary>
    /// 创建新矩阵并初始化为0
    /// </summary>
    /// <param name="row"></param>
    /// <param name="col"></param>
    Matrix(int row = 1, int col = 1) :row(row), col(col), rank(-1)
    {
        if (row < 1)
            row = 1;
        if (col < 1)
            col = 1;
        m = new double* [row];
        for (int i = 0; i < row; i++)
            m[i] = new double[col];
        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < col; j++)
            {
                m[i][j] = 0;
            }
        }
    }

    /// <summary>
    /// 复制构造函数，深复制
    /// </summary>
    /// <param name="A"></param>
    Matrix(const Matrix& A) : row(A.row), col(A.col), rank(A.rank)
    {
        m = new double* [row];
        for (int i = 0; i < row; i++)
            m[i] = new double[col];
        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < col; j++)
            {
                m[i][j] = A.m[i][j];
            }
        }
    }

    ~Matrix()
    {
        for (int i = 0; i < row; i++)
            delete[] m[i];
        delete[] m;
    }

    /// <summary>
    /// 高斯消元
    /// </summary>
    void Gauss_Jordan_Elimination()
    {
        int zeroRow = 0;
        for (int i = 0; i < col && i < row; i++)  //i看成列
        {
            int nonZero = row - 1;  //先设非零行为最后一行
            for (int j = i - zeroRow; j < row; j++) // 找到非0元素
                if (abs(m[j][i]) > PRECISION)
                {
                    nonZero = j;
                    break;
                }

            if (abs(m[nonZero][i]) > PRECISION)
            {
                if (nonZero != i - zeroRow) //如果非0元素不是第i-zeroRow行
                {
                    for (int k = i; k < col; k++) // 把非0元素所在行交换到当前行
                    {
                        double t = m[i - zeroRow][k];
                        m[i - zeroRow][k] = m[nonZero][k];
                        m[nonZero][k] = t;
                    }
                    nonZero = i - zeroRow;
                }

                if (abs(m[i - zeroRow][i] - 1.0) > PRECISION)  //归一化
                {
                    double temp = m[i - zeroRow][i];
                    for (int k = i; k < col; k++)
                    {
                        m[i - zeroRow][k] /= temp;
                    }
                }

                for (int j = i - zeroRow + 1; j < row; j++) //把下面部分消为0
                {
                    if (abs(m[j][i]) > PRECISION) //如果m[j][i]不是0
                    {
                        double temp = m[j][i];
                        for (int k = i; k < col; k++)
                        {
                            m[j][k] = m[j][k] - m[i - zeroRow][k] * temp;
                        }
                    }
                }

                for (int j = i - zeroRow - 1; j >= 0; j--) //把上面部分消为0
                {
                    if (abs(m[j][i]) > PRECISION)
                    {
                        double temp = m[j][i];
                        for (int k = i; k < col; k++)
                        {
                            m[j][k] = m[j][k] - m[i - zeroRow][k] * temp;
                        }
                    }
                }
            }
            else
            {
                zeroRow++;
            }
        }
        rank = row - zeroRow;
        if (col > row && rank < row) //若此时不满秩，且col>row，则最后一行可能不会被归一化，且最后一行的秩不会被计算，要单独处理
        {
            int k = row;
            while (abs(m[row - 1][k]) < PRECISION && k < col)
            {
                k++;
            }
            if (k < col)
            {
                double temp = m[row - 1][k];
                for (int i = k; i < col; i++)
                {
                    m[row - 1][i] /= temp;
                }
                rank++; //加上最后一行的秩
                for (int j = row - 2; j >= 0; j--) //把上面部分消为0
                {
                    if (abs(m[j][k]) > PRECISION)
                    {
                        double temp = m[j][k];
                        for (int t = k; t < col; t++)
                        {
                            m[j][t] = m[j][t] - m[row - 1][t] * temp;
                        }
                    }
                }
            }
        }
    }

    /// <summary>
    /// 若输入的矩阵是增广矩阵，则可以使用寻找一个解
    /// </summary>
    /// <returns>若解不存在，则返回NULL；否则，返回一个解。注意，若返回值不为NULL，则返回的指针需要手动delete</returns>
    double* GetAnswerForAugmentedMatrix()
    {
        if (rank < 0)
            return NULL;
        //先看有没有解，即看行简化阶梯型的全零行对应增广矩阵最右行的元素是否为0
        for (int i = row - 1; i >= rank; i--)
        {
            if (abs(m[i][col - 1]) > PRECISION)
                return NULL;
        }
        double* ret = new double[col - 1];
        int numOfFreeColumn = 0;

        int temp;  //记录列
        for (temp = 0; temp < col - 1 && temp - numOfFreeColumn < row; temp++)  //主列位置对应分量为目标向量的对应分量，自由列对应分量直接取0
        {
            if (abs(m[temp - numOfFreeColumn][temp]) > PRECISION)
            {
                ret[temp] = m[temp - numOfFreeColumn][col - 1];
            }
            else
            {
                numOfFreeColumn++;
                ret[temp] = 0; //自由列位置处取0肯定满足方程
            }
        }

        while (temp < col - 1)
        {
            ret[temp++] = 0;
        }
        return ret;
    }

    /// <summary>
    /// 行列式，不为方阵时返回0
    /// </summary>
    double Determinant() const
    {
        if (row != col)
            return 0;
        int swapTimes = 0; //行交换次数
        double** mtemp = new double* [row];
        for (int i = 0; i < row; i++)
            mtemp[i] = new double[col];
        for (int i = 0; i < row; i++)
            for (int j = 0; j < col; j++)
            {
                mtemp[i][j] = m[i][j];
            }
        int zeroRow = 0;
        for (int i = 0; i < col && i < row; i++)  //i看成列
        {
            int nonZero = row - 1;  //先设非零行为最后一行
            for (int j = i - zeroRow; j < row; j++) // 找到非0元素
                if (abs(mtemp[j][i]) > PRECISION)
                {
                    nonZero = j;
                    break;
                }

            if (abs(mtemp[nonZero][i]) > PRECISION)
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
                    if (abs(mtemp[j][i]) > PRECISION) //如果mtemp[j][i]不是0
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
        if (abs(mtemp[row - 1][col - 1]) < PRECISION)
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
                caculateInverse.m[i][j] = m[i][j];
                caculateInverse.m[i][j + row] = i == j ? 1 : 0;
            }
        caculateInverse.Gauss_Jordan_Elimination();
        if (abs(caculateInverse.m[row - 1][row - 1]) < PRECISION) //高斯消元后，右下角为0，则不可逆；否则可逆。
            return false;
        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < row; j++)
            {
                inverse.m[i][j] = caculateInverse.m[i][j + row];
            }
        }
        return true;
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
                    ret.m[i][j] += A.m[i][k] * B.m[k][j];
                }
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
                ret.m[i][j] += A.m[i][j] + B.m[i][j];
            }
        }
        return ret;
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
        Matrix ret(A.row, B.row);
        for (int i = 0; i < ret.row; i++)
        {
            for (int j = 0; j < ret.col; j++)
            {
                ret.m[i][j] += A.m[i][j] - B.m[i][j];
            }
        }
        return ret;
    }

    Matrix operator =(const Matrix& A)
    {
        if (A.col != col || A.row != row)
        {
            for (int i = 0; i < row; i++)
                delete[] m[i];
            delete[] m;
            row = A.row;
            col = A.col;
            rank = A.rank;
            m = new double* [row];
            for (int i = 0; i < row; i++)
                m[i] = new double[col];
        }
        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < col; j++)
            {
                m[i][j] = A.m[i][j];
            }
        }
        return *this;
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
        else
        {
            Matrix ori(*this), ret(row, row);
            for (int i = 0; i < row; i++)
                ret.m[i][i] = 1;
            while (n > 0)
            {
                if (n % 2)
                {
                    ret = ret * ori;
                    n -= 1;
                    ori = ori * ori;
                    n /= 2;
                }
                else
                {
                    ori = ori * ori;
                    n /= 2;
                }
            }
            return ret;
        }
    }

    void Display(int width = 3, int precision = 3)
    {
        for (int i = 0; i < row; i++)
            for (int j = 0; j < col; j++)
            {
                std::cout << std::setprecision(precision) << std::setw(width) << m[i][j] << " ";
                if (j == col - 1)
                    std::cout << std::endl;
            }
        std::cout << std::endl;
    }
};
#endif