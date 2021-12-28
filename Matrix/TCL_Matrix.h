#ifndef TCL_MATRIX_H
#define TCL_MATRIX_H
#pragma once
#include<iostream>
#include<cmath>
#include<iomanip>
#include<vector>
#include<algorithm>
#include<cfloat>
#include<utility>
#include<complex>
#include<string>
#include<fstream>
#define PRECISION_OF_DIFFERENCE 1e-3
#define PRECISION_WHEN_CALCULATING 1e-5

namespace TCL_Matrix
{
    class Matrix
    {
    private:
        /// <summary>
        /// ����ʽ��������������ֵ����
        /// </summary>
        /// <param name="coeff">����ʽϵ������</param>
        /// <param name="degree">����ʽ����</param>
        /// <param name="z">�Ա���</param>
        /// <returns></returns>
        std::complex<double> PolynomialFunc(const double* coeff, int degree, const std::complex<double>& z) const
        {
            std::complex<double> ret = { 0,0 };
            for (int i = 0; i < degree + 1; i++)
            {
                ret += (coeff[i] * std::pow(z, i));
            }
            return ret;
        }

        /// <summary>
        /// ��������ʱ��������
        /// </summary>
        /// <param name="z1"></param>
        /// <param name="z2"></param>
        /// <param name="precision"></param>
        /// <returns></returns>
        bool Greater(const std::complex<double>& z1, const std::complex<double>& z2, double precision = 0.1 * PRECISION_OF_DIFFERENCE) const
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
        int rank;
        int row;
        int col;

    public:
        double** matrix;

        /// <summary>
        /// �����о�����Ƶ���������У���ʼʱ��֪���ȣ�����Ϊ-1��
        /// </summary>
        /// <param name="matrix">ʹ��ʱ�����Խ�double**����ǿתΪdouble*��������</param>
        /// <param name="row"></param>
        /// <param name="col"></param>
        Matrix(double* newMatrix, int row, int col) :row(row), col(col), rank(-1)
        {
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
        /// �����¾��󲢳�ʼ��Ϊ0
        /// </summary>
        /// <param name="row"></param>
        /// <param name="col"></param>
        Matrix(int row = 1, int col = 1) :row(row), col(col), rank(-1)
        {
            if (row < 1)
                row = 1;
            if (col < 1)
                col = 1;
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

        /// <summary>
        /// ���ƹ��캯�������
        /// </summary>
        /// <param name="A"></param>
        Matrix(const Matrix& A) : row(A.row), col(A.col), rank(A.rank)
        {
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

        ~Matrix()
        {
            for (int i = 0; i < row; i++)
                delete[] matrix[i];
            delete[] matrix;
        }

        /// <summary>
        /// ��˹��Ԫ����Ϊ�м򻯽�����
        /// </summary>
        void Gauss_Jordan_Elimination()
        {
            int zeroRow = 0;
            for (int i = 0; i < col && i < row; i++)  //i������
            {
                int nonZero = row - 1;  //���������Ϊ���һ��
                for (int j = i - zeroRow; j < row; j++) // �ҵ���0Ԫ��
                    if (std::abs(matrix[j][i]) > PRECISION_OF_DIFFERENCE)
                    {
                        nonZero = j;
                        break;
                    }

                if (std::abs(matrix[nonZero][i]) > PRECISION_OF_DIFFERENCE)
                {
                    if (nonZero != i - zeroRow) //�����0Ԫ�ز��ǵ�i-zeroRow��
                    {
                        for (int k = i; k < col; k++) // �ѷ�0Ԫ�������н�������ǰ��
                        {
                            double t = matrix[i - zeroRow][k];
                            matrix[i - zeroRow][k] = matrix[nonZero][k];
                            matrix[nonZero][k] = t;
                        }
                        nonZero = i - zeroRow;
                    }

                    if (std::abs(matrix[i - zeroRow][i] - 1.0) > PRECISION_OF_DIFFERENCE)  //��һ��
                    {
                        double temp = matrix[i - zeroRow][i];
                        for (int k = i; k < col; k++)
                        {
                            matrix[i - zeroRow][k] /= temp;
                        }
                    }

                    for (int j = i - zeroRow + 1; j < row; j++) //�����沿����Ϊ0
                    {
                        if (std::abs(matrix[j][i]) > PRECISION_OF_DIFFERENCE) //���matrix[j][i]����0
                        {
                            double temp = matrix[j][i];
                            for (int k = i; k < col; k++)
                            {
                                matrix[j][k] = matrix[j][k] - matrix[i - zeroRow][k] * temp;
                            }
                        }
                    }

                    for (int j = i - zeroRow - 1; j >= 0; j--) //�����沿����Ϊ0
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
            if (col > row && rank < row) //����ʱ�����ȣ���col>row�������һ�п��ܲ��ᱻ��һ���������һ�е��Ȳ��ᱻ���㣬Ҫ��������
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
                    rank++; //�������һ�е���
                    for (int j = row - 2; j >= 0; j--) //�����沿����Ϊ0
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
        /// ������Ϊ������󣬿��ô˺������
        /// </summary>
        /// <param name="solution">���ڽ���һ��������</param>
        /// <returns>������ֵΪfalse���򷽳��޽⣬δ�Է��ز�������</returns>
        bool GetAnswerForAugmentedMatrix(Matrix& ret)
        {
            Matrix original(*this);
            int zeroRow = 0;
            for (int i = 0; i < original.col - 1 && i < original.row; i++)  //i�����У���ʱ���������i��ൽcol - 1
            {
                int nonZero = original.row - 1;  //���������Ϊ���һ��
                for (int j = i - zeroRow; j < original.row; j++) // �ҵ���0Ԫ��
                    if (std::abs(original.matrix[j][i]) > PRECISION_OF_DIFFERENCE)
                    {
                        nonZero = j;
                        break;
                    }

                if (std::abs(original.matrix[nonZero][i]) > PRECISION_OF_DIFFERENCE)
                {
                    if (nonZero != i - zeroRow) //�����0Ԫ�ز��ǵ�i-zeroRow��
                    {
                        for (int k = i; k < original.col; k++) // �ѷ�0Ԫ�������н�������ǰ��
                        {
                            double t = original.matrix[i - zeroRow][k];
                            original.matrix[i - zeroRow][k] = original.matrix[nonZero][k];
                            original.matrix[nonZero][k] = t;
                        }
                        nonZero = i - zeroRow;
                    }

                    if (std::abs(original.matrix[i - zeroRow][i] - 1.0) > PRECISION_OF_DIFFERENCE)  //��һ��
                    {
                        double temp = original.matrix[i - zeroRow][i];
                        for (int k = i; k < original.col; k++)
                        {
                            original.matrix[i - zeroRow][k] /= temp;
                        }
                    }

                    for (int j = i - zeroRow + 1; j < original.row; j++) //�����沿����Ϊ0
                    {
                        if (std::abs(original.matrix[j][i]) > PRECISION_OF_DIFFERENCE) //���matrix[j][i]����0
                        {
                            double temp = original.matrix[j][i];
                            for (int k = i; k < original.col; k++)
                            {
                                original.matrix[j][k] = original.matrix[j][k] - original.matrix[i - zeroRow][k] * temp;
                            }
                        }
                    }

                    for (int j = i - zeroRow - 1; j >= 0; j--) //�����沿����Ϊ0
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
            if (original.col - 1 > original.row)
                original.rank = original.row - zeroRow;
            else original.rank = original.col - 1 - zeroRow;
            if (original.col - 1 > original.row && original.rank < original.row) //����ʱԭ�������ȣ���col - 1>row�������һ�п��ܲ��ᱻ��һ���������һ�е��Ȳ��ᱻ���㣬Ҫ��������
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
                    original.rank++; //�������һ�е���
                    for (int j = original.row - 2; j >= 0; j--) //�����沿����Ϊ0
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

            //�ȿ���û�н⣬�����м򻯽����͵�ȫ���ж�Ӧ������������е�Ԫ���Ƿ�Ϊ0
            for (int i = original.row - 1; i >= original.rank; i--)
            {
                if (std::abs(original.matrix[i][original.col - 1]) > PRECISION_OF_DIFFERENCE)
                {
                    return false;
                }
            }

            Matrix solution(col - 1, 1);
            int numOfFreeColumn = 0;

            int temp;  //��¼��
            for (temp = 0; temp < original.col - 1 && temp - numOfFreeColumn < original.row; temp++)  //����λ�ö�Ӧ����ΪĿ�������Ķ�Ӧ�����������ж�Ӧ����ֱ��ȡ0
            {
                if (std::abs(original.matrix[temp - numOfFreeColumn][temp]) > PRECISION_OF_DIFFERENCE)
                {
                    solution.matrix[temp][0] = original.matrix[temp - numOfFreeColumn][original.col - 1];
                }
                else
                {
                    numOfFreeColumn++;
                    solution.matrix[temp][0] = 0;//������λ�ô�ȡ0�϶����㷽��
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
        /// ������Ϊ������󣬿��ô˺����ҵ����Է���������н�
        /// </summary>
        /// <param name="ret">ǰcol-1������ռ��һ��������1����һ���ؽ�</param>
        /// <returns>������ֵΪfalse���򷽳��޽⣬δ�Է��ز�������</returns>
        bool GetAllSolutionsForAugmentedMatrix(Matrix& ret)
        {
            Matrix original(*this);
            int zeroRow = 0;
            for (int i = 0; i < original.col - 1 && i < original.row; i++)  //i�����У���ʱ���������i��ൽcol - 1
            {
                int nonZero = original.row - 1;  //���������Ϊ���һ��
                for (int j = i - zeroRow; j < original.row; j++) // �ҵ���0Ԫ��
                    if (std::abs(original.matrix[j][i]) > PRECISION_OF_DIFFERENCE)
                    {
                        nonZero = j;
                        break;
                    }

                if (std::abs(original.matrix[nonZero][i]) > PRECISION_OF_DIFFERENCE)
                {
                    if (nonZero != i - zeroRow) //�����0Ԫ�ز��ǵ�i-zeroRow��
                    {
                        for (int k = i; k < original.col; k++) // �ѷ�0Ԫ�������н�������ǰ��
                        {
                            double t = original.matrix[i - zeroRow][k];
                            original.matrix[i - zeroRow][k] = original.matrix[nonZero][k];
                            original.matrix[nonZero][k] = t;
                        }
                        nonZero = i - zeroRow;
                    }

                    if (std::abs(original.matrix[i - zeroRow][i] - 1.0) > PRECISION_OF_DIFFERENCE)  //��һ��
                    {
                        double temp = original.matrix[i - zeroRow][i];
                        for (int k = i; k < original.col; k++)
                        {
                            original.matrix[i - zeroRow][k] /= temp;
                        }
                    }

                    for (int j = i - zeroRow + 1; j < original.row; j++) //�����沿����Ϊ0
                    {
                        if (std::abs(original.matrix[j][i]) > PRECISION_OF_DIFFERENCE) //���matrix[j][i]����0
                        {
                            double temp = original.matrix[j][i];
                            for (int k = i; k < original.col; k++)
                            {
                                original.matrix[j][k] = original.matrix[j][k] - original.matrix[i - zeroRow][k] * temp;
                            }
                        }
                    }

                    for (int j = i - zeroRow - 1; j >= 0; j--) //�����沿����Ϊ0
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
            if (original.col - 1 > original.row)
                original.rank = original.row - zeroRow;  //������ָԭ������ȣ��������������ȡ�
            else original.rank = original.col - 1 - zeroRow;
            if (original.col - 1 > original.row && original.rank < original.row) //����ʱԭ�������ȣ���col - 1>row�������һ�п��ܲ��ᱻ��һ���������һ�е��Ȳ��ᱻ���㣬Ҫ��������
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
                    original.rank++; //�������һ�е���
                    for (int j = original.row - 2; j >= 0; j--) //�����沿����Ϊ0
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
            //�ȿ���û�н⣬�����м򻯽����͵�ȫ���ж�Ӧ������������е�Ԫ���Ƿ�Ϊ0
            for (int i = original.row - 1; i >= original.rank; i--)
            {
                if (std::abs(original.matrix[i][original.col - 1]) > PRECISION_OF_DIFFERENCE)
                {
                    return false;
                }
            }

            Matrix solution(original.col - 1, original.col - original.rank);
            int numOfFreeColumn = 0;
            int temp = 0;
            for (temp = 0; temp < original.col - 1 && temp - numOfFreeColumn < original.row; temp++) //������ռ�Ļ�
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
            //���ؽ�
            for (temp = 0; temp < original.col - 1 && temp - numOfFreeColumn < original.row; temp++)  //����λ�ö�Ӧ����ΪĿ�������Ķ�Ӧ�����������ж�Ӧ����ֱ��ȡ0
            {
                if (std::abs(original.matrix[temp - numOfFreeColumn][temp]) > PRECISION_OF_DIFFERENCE)
                {
                    solution.matrix[temp][original.col - original.rank - 1] = original.matrix[temp - numOfFreeColumn][original.col - 1];
                }
                else
                {
                    numOfFreeColumn++;
                    solution.matrix[temp][original.col - original.rank - 1] = 0;
                }
            }
            while (temp < original.col - 1)
            {
                solution.matrix[temp++][original.col - original.rank - 1] = 0;
            }
            ret = solution;
            return true;
        }

        /// <summary>
        /// �ҵ���ռ��һ���
        /// </summary>
        /// <param name="bases">���ڷ��ػ�</param>
        /// <returns>��������棬�򷵻�false�����򣬷���һ�����ע�����۾����Ƿ���棬��������Ϊ����ֵ�Ĳ���bases���ᷢ���仯��</returns>
        bool GetBasesOfNullSpace(Matrix& bases)
        {
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
        /// ����ʽ����Ϊ����ʱ����0
        /// </summary>
        double Determinant() const
        {
            if (row != col)
                return 0;
            int swapTimes = 0; //�н�������
            double** mtemp = new double* [row];
            for (int i = 0; i < row; i++)
                mtemp[i] = new double[col];
            for (int i = 0; i < row; i++)
                for (int j = 0; j < col; j++)
                {
                    mtemp[i][j] = matrix[i][j];
                }
            int zeroRow = 0;
            for (int i = 0; i < col && i < row; i++)  //i������
            {
                int nonZero = row - 1;  //���������Ϊ���һ��
                for (int j = i - zeroRow; j < row; j++) // �ҵ���0Ԫ��
                    if (std::abs(mtemp[j][i]) > PRECISION_OF_DIFFERENCE)
                    {
                        nonZero = j;
                        break;
                    }

                if (std::abs(mtemp[nonZero][i]) > PRECISION_OF_DIFFERENCE)
                {
                    if (nonZero != i - zeroRow) //�����0Ԫ�ز��ǵ�i-zeroRow��
                    {
                        for (int k = i; k < col; k++) // �ѷ�0Ԫ�������н�������ǰ��
                        {
                            double t = mtemp[i - zeroRow][k];
                            mtemp[i - zeroRow][k] = mtemp[nonZero][k];
                            mtemp[nonZero][k] = t;
                        }
                        nonZero = i - zeroRow;
                        swapTimes++;
                    }

                    for (int j = i - zeroRow + 1; j < row; j++) //�����沿����Ϊ0
                    {
                        if (std::abs(mtemp[j][i]) > PRECISION_OF_DIFFERENCE) //���mtemp[j][i]����0
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
        /// �����������
        /// </summary>
        /// <param name="inverse">inverseӦΪrow*row�ľ���������ڽ��շ���ֵ����������棬�������ͨ���ò������أ������󲻿��棬��inverse��������κβ���</param>
        /// <returns>���ؾ����Ƿ����</returns>
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
                    caculateInverse.matrix[i][j] = matrix[i][j];
                    caculateInverse.matrix[i][j + row] = i == j ? 1 : 0;
                }
            caculateInverse.Gauss_Jordan_Elimination();
            if (std::abs(caculateInverse.matrix[row - 1][row - 1]) < PRECISION_OF_DIFFERENCE) //��˹��Ԫ�����½�Ϊ0���򲻿��棻������档
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
        /// ����ת��
        /// </summary>
        /// <param name="A"></param>
        /// <returns>����ת�ú�ľ���</returns>
        friend Matrix Transpose(const Matrix& A)
        {
            Matrix temp = A;
            temp.TransposeDirectly();
            return temp;
        } const

        /// <summary>
                /// ֱ�ӽ��������ת��
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
        /// LU�ֽ�
        /// </summary>
        /// <param name="L"></param>
        /// <param name="U"></param>
        /// <returns>����trueʱ���þ������LU�ֽ⣬�ֽ����L��Uͨ����������</returns>
        bool LU(Matrix& L, Matrix& U) const
        {
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
        /// ����ֵ�ֽ�
        /// </summary>
        /// <param name="U">��������U</param>
        /// <param name="Sigma">����ֵ����Sigma</param>
        /// <param name="VT">��������V��ת��</param>
        /// <param name="precision">���㾫��</param>
        /// <param name="minIteration">�ݷ���С��������</param>
        /// <returns>����Ϊfalseʱ������ֵ�ֽ�ʧ�ܣ����������ڼ�������еľ�����ʧ��ɵģ�����ʱU��Sigma��Vδ����</returns>
        bool SVD(Matrix& U, Matrix& Sigma, Matrix& VT, double precision = PRECISION_WHEN_CALCULATING, int minIteration = 50) const
        {
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
                int len = 0;  //��0����ֵ����

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
                //V1��������
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
                        V1.matrix[i][j] /= mod; //��һ��
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
                            V1.matrix[p][nowCol] /= mod; //��һ��
                        }
                        nowCol++;
                    }
                }

                int now = 0;
                Matrix Sigmar(ATADim, ATADim);  //��һ��Sigmar����
                for (int i = 0; i < len; i++)
                {
                    for (int j = 0; j < multiplicity[i]; j++)
                    {
                        if (std::abs(values[i]) > PRECISION_OF_DIFFERENCE)
                        {
                            Sigmar.matrix[now][now] = 1 / sqrt(values[i]); //Sigmar�������
                            now++;
                        }
                    }
                }

                now = 0;
                for (int i = 0; i < values.size(); i++)
                {
                    for (int j = 0; j < multiplicity[i]; j++)
                    {
                        if (std::abs(values[i]) > PRECISION_OF_DIFFERENCE)
                        {
                            Sigma.matrix[now][now] = sqrt(values[i]); //Sigma����ֵ
                            now++;
                        }
                    }
                }

                Matrix U1((*this) * V1 * Sigmar);

                //��Ҫ����V��������
                if (V1.col < V.col)
                {
                    Matrix I(IdentityMatrix(V.row));
                    Matrix zero(V.row, 1);
                    Matrix V2(V.row, V.col - V1.col);
                    Matrix V1T(Transpose(V1));
                    int p = 0;
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

                //����U��������
                if (U1.col < U.col)  //��ԭ�����Ƿ���ʱ�������U1������U��Ҫ������������
                {
                    Matrix I(IdentityMatrix(U.row));
                    Matrix zero(U.row, 1);
                    Matrix U2(U.row, U.col - U1.col);
                    Matrix U1T(Transpose(U1));
                    int p = 0;
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
                //U1��������
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
                        U1.matrix[i][j] /= mod; //��һ��
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
                            U1.matrix[p][nowCol] /= mod; //��һ��
                        }
                        nowCol++;
                    }
                }

                int now = 0;
                Matrix Sigmar(AATDim, AATDim);  //��һ��Sigmar����
                for (int i = 0; i < len; i++)
                {
                    for (int j = 0; j < multiplicity[i]; j++)
                    {
                        if (std::abs(values[i]) > PRECISION_OF_DIFFERENCE)
                        {
                            Sigmar.matrix[now][now] = 1 / sqrt(values[i]); //Sigmar�������
                            now++;
                        }
                    }
                }

                now = 0;
                for (int i = 0; i < values.size(); i++)
                {
                    for (int j = 0; j < multiplicity[i]; j++)
                    {
                        if (std::abs(values[i]) > PRECISION_OF_DIFFERENCE)
                        {
                            Sigma.matrix[now][now] = sqrt(values[i]); //Sigma����ֵ
                            now++;
                        }
                    }
                }

                Matrix V1(A * U1 * Sigmar);

                //��Ҫ����U��������
                if (U1.col < U.col)
                {
                    Matrix I(IdentityMatrix(U.row));
                    Matrix zero(U.row, 1);
                    Matrix U2(U.row, U.col - U1.col);
                    Matrix U1T(Transpose(U1));
                    int p = 0;
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

                //����V��������
                if (V1.col < V.col)  //��ԭ�����Ƿ���ʱ�������V1������V��Ҫ������������
                {
                    Matrix I(IdentityMatrix(V.row));
                    Matrix zero(V.row, 1);
                    Matrix V2(V.row, V.col - V1.col);
                    Matrix V1T(Transpose(V1));
                    int p = 0;
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
        /// ��SVD
        /// </summary>
        /// <param name="Ur"></param>
        /// <param name="Sigmar"></param>
        /// <param name="VrT"></param>
        /// <param name="precision"></param>
        /// <param name="minIteration"></param>
        /// <returns></returns>
        bool SimplifiedSVD(Matrix& Ur, Matrix& Sigmar, Matrix& VrT, double precision = PRECISION_WHEN_CALCULATING, int minIteration = 50)
        {
            Matrix A(*this);
            A.TransposeDirectly(); //A��ԭ�����ת��
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
                //Vr��������
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
                        Vr.matrix[i][j] /= mod; //��һ��
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
                            Vr.matrix[p][nowCol] /= mod; //��һ��
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
                            Sigmar.matrix[now][now] = 1 / sqrt(values[i]); //Sigmar�������
                            now++;
                        }
                    }
                }

                Ur = (*this) * Vr * Sigmar;

                now = 0;
                for (int i = 0; i < Sigmar.row; i++)
                {
                    Sigmar.matrix[i][i] = 1.0 / Sigmar.matrix[i][i];  //Sigmar��ԭ
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
                //Ur��������
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
                        Ur.matrix[i][j] /= mod; //��һ��
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
                            Ur.matrix[p][nowCol] /= mod; //��һ��
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
                            Sigmar.matrix[now][now] = 1 / sqrt(values[i]); //Sigmar�������
                            now++;
                        }
                    }
                }

                VrT = Transpose(A * Ur * Sigmar);

                now = 0;
                for (int i = 0; i < Sigmar.row; i++)
                {
                    Sigmar.matrix[i][i] = 1.0 / Sigmar.matrix[i][i];  //Sigmar��ԭ
                }
            }
            return true;
        }

        /// <summary>
        /// ����������Moore-Penrose������
        /// </summary>
        /// <param name="precision"></param>
        /// <param name="minIteration"></param>
        /// <returns>������������޴����򷵻ع����棻���򷵻�NULL</returns>
        Matrix MoorePenrosGeneralizedInverse(double precision = PRECISION_WHEN_CALCULATING, int minIteration = 50)
        {
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
                return NULL;
            }
        }

        /// <summary>
        /// ����/�����Ⱦ����Moore-Penrose������
        /// </summary>
        /// <param name="judgeRank">�Ƿ��жϾ��������Ȼ�������</param>
        /// <returns>������������޴����򷵻ع����棻���򷵻�NULL</returns>
        Matrix MPGeneralizedInverseForFullRankMatrix(bool judgeRank = true)
        {
            int theoreticRank = row < col ? row : col;
            if (judgeRank)
            {
                int r = GetRank();
                this->rank = r;
                if (r != theoreticRank)
                {
                    std::cout << "The matrix is not a full-rank matrix. Return NULL" << std::endl;
                    return NULL;
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
                    std::cout << "The matrix is not a full-rank matrix. Return NULL" << std::endl;
                    return NULL;
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
                    std::cout << "The matrix is not a full-rank matrix. Return NULL" << std::endl;
                    return NULL;
                }
            }
        }
       
        /// <summary>
        /// ����˷�
        /// </summary>
        /// <param name="A"></param>
        /// <param name="B"></param>
        /// <returns>��ά��ʧ�䣬�򷵻ص�һ��������</returns>
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
        /// ��������
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
        /// ����ӷ�
        /// </summary>
        /// <param name="A"></param>
        /// <param name="B"></param>
        /// <returns>��ά��ʧ�䣬�򷵻ص�һ��������</returns>
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
        /// �������
        /// </summary>
        /// <param name="A"></param>
        /// <param name="B"></param>
        /// <returns>��ά��ʧ�䣬�򷵻ص�һ��������</returns>
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
        /// ��������ж�
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

        /// <summary>
        /// ���󲻵��ж�
        /// </summary>
        /// <param name="A"></param>
        /// <param name="B"></param>
        /// <returns></returns>
        friend bool operator !=(const Matrix& A, const Matrix& B)
        {
            return !(A == B);
        }

        /// <summary>
        /// �������ϲ�
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
        /// ����ֵ
        /// </summary>
        /// <param name="A"></param>
        /// <returns></returns>
        Matrix operator =(const Matrix& A)
        {
            rank = A.rank;
            if (A.col != col || A.row != row)
            {
                for (int i = 0; i < row; i++)
                    delete[] matrix[i];
                delete[] matrix;
                row = A.row;
                col = A.col;
                matrix = new double* [row];
                for (int i = 0; i < row; i++)
                    matrix[i] = new double[col];
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
        /// ���������
        /// </summary>
        /// <param name="column"></param>
        /// <returns></returns>
        Matrix operator [](int column)
        {
            if (column >= col || column < 0)
            {
                std::cout << "Index out of range! Return NULL." << std::endl;
                return NULL;
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
        /// ������ݣ��������Ϊ����
        /// </summary>
        /// <param name="n">��������Ȼ��</param>
        /// <returns>������Ϊ��������С��0���򷵻�ԭ����</returns>
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
        /// ����������
        /// </summary>
        /// <returns></returns>
        int GetRank()
        {
            if (rank != -1)
                return rank;
            else
            {
                Matrix temp(*this);
                temp.Gauss_Jordan_Elimination();
                rank = temp.rank;
                return temp.rank;
            }
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
        /// ��þ������������ֵ�������ظ�
        /// </summary>
        /// <param name="v">���ڽ�������ֵ</param>
        /// <param name="precision">��������</param>
        /// <param name="minIteration">��С��������</param>
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
        /// ����ԳƷǲ����������������ֵ�������ظ�
        /// </summary>
        /// <param name="v">���ڽ�������ֵ��������ԳƷǲ��������㷨��֤�������������ֵ�����ڽ��������е�˳�򲻶��������󲻶Գƻ�Ϊ��������������жϣ�v�л����һ��DBL_MAXԪ�أ�������ԳƵ�������������ȷ�Բ�����֤</param>
        /// <param name="precision">��������</param>
        /// <param name="minIteration">��С��������</param>
        /// <param name="judgeSymmetry">�Ƿ�Ծ���Գ��Ի��Ƿ�Ϊ��������ж�</param>
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
            //���ݷ���������ֵ�������ֵ�������ϵ���
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
                        vector.matrix[p++][0] = 0;  //��ֹA*vector�õ�������
                    }
                    if (p >= row)
                        return;
                    vector = A * vector;
                    int maxpos = 0; //���λ��
                    for (int i = 0; i < row; i++)
                    {
                        if (std::abs(vector.matrix[i][0]) > std::abs(vector.matrix[maxpos][0]))
                            maxpos = i;
                    }
                    last = vector.matrix[maxpos][0];
                    now = last;
                    iteration = 0; //��0��������
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

                    bool isExist = false; //����ֵ�Ƿ��Ѿ��������
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
                        vector = vector - (Transpose(vector) * eigen).matrix[0][0] * eigen; //������
                    }
                    double mod = 0;
                    for (int i = 0; i < row; i++)
                    {
                        mod += (vector.matrix[i][0] * vector.matrix[i][0]);
                    }
                    mod = sqrt(mod);
                    if (vector.matrix[maxpos][0] > 0)
                        vector = (1 / mod) * vector; //��һ��
                    else
                        vector = (-1 / mod) * vector;
                    A = A - now * (vector * Transpose(vector));
                    //��ֹ���������
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
        /// ��÷�����������ʽ��ϵ��
        /// </summary>
        /// <param name="v">������Ϊ���󣬷���false��vectorδ�����κβ���</param>
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
        /// �����������У���������ֵ�����ظ�
        /// </summary>
        /// <param name="possibleMaxMultiplicity">����ֵ���ܵ�����������ò����ǳ�Ӱ������׼ȷ��</param>
        /// <param name="v">���ս��vector</param>
        /// <param name="aberthIteration">Aberth��������</param>
        /// <param name="newtonIteration">ţ�ٵ�������</param>
        /// <param name="possibleMaxMod">����ֵ���ܵ����ģ��</param>
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
                    if (std::abs(multiroots[j] - roots[i]) < PRECISION_OF_DIFFERENCE * 20) //����ľ��Ȳ���̫��
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
        /// ����һ����λ����
        /// </summary>
        /// <param name="n">����ά��</param>
        /// <returns></returns>
        static Matrix IdentityMatrix(int n)
        {
            Matrix I(n, n);
            for (int i = 0; i < n; i++)
            {
                I.matrix[i][i] = 1;
            }
            return I; //����ø��ƹ��캯�������Ч�ʱ�ֱ�ӹ���Ҫ�ͣ��˺���ֻ��Ϊ�˷���
        }

        /// <summary>
        /// �����׷���
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
            int maxpos = 0; //���λ��
            for (int i = 0; i < T.col; i++)
            {
                if (std::abs(vector.matrix[i][0]) > std::abs(vector.matrix[maxpos][0]))
                    maxpos = i;
            }
            double last = vector.matrix[maxpos][0];
            double now = last;
            int iteration = 0; //��0��������
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
        /// �����װ뾶
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
            int maxpos = 0; //���λ��
            for (int i = 0; i < T.col; i++)
            {
                if (std::abs(vector.matrix[i][0]) > std::abs(vector.matrix[maxpos][0]))
                    maxpos = i;
            }
            double last = vector.matrix[maxpos][0];
            double now = last;
            int iteration = 0; //��0��������
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
        /// ���㷽��ļ�
        /// </summary>
        /// <returns>�������Ƿ����򷵻�DBL_MAX</returns>
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
        /// ��ӡ����
        /// </summary>
        /// <param name="width">ָ�������ȣ�Ĭ��Ϊ3</param>
        /// <param name="precision">ָ��������ȣ�Ĭ��Ϊ3</param>
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
        /// ���ļ��ж�ȡ����
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
        /// ���ļ��б������
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
    };

    /// <summary>
    /// �쳣�࣬��ʱ�����ò���
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
#endif