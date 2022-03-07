# C++ / C# Matrix Processing Library

## Brief Introduction

- this is a matrix processing library for C++ or C#, which can be convenient for matrix operations.
  - C++ version is at Matrix/TCL_Matrix.h on master branch
  - C# version is at CSharp_Matrix on master branch

- GUI program for matrix calculation has been released. You are welcome to download and use the latest version. Please see detailed instructions at [MatrixCal/README.md](https://github.com/TCL606/Matrix/blob/main/MatrixCal/README.md)

- C++ and C# libraries implement almost the same algorithms, including

  - Basic Calculation

    - Addtion

    - Multiplication
    - Transpose

  - Matrix common operations:

    - Get the inverse of a matrix

    - Matrix Gauss-Jordan elimination

    - Matrix LU decomposition

    - Find the rank of the matrix

    - Get determinant

    - Matrix fast power
    - Solve linear equations
    - Find the null space of the matrix
    - Get trace of a matrix
    - Matrix QR decomposition

  - Common numerical calculation

    - Calculate the matrix norm
    - Calculate the spectral radius of the matrix
    - Find all singular values of any matrix (excluding multiple roots)
    - Find all (complex) eigenvalues of any matrix (including multiple roots)

  - Matrix advanced operations

    - Obtain the coefficients of the square matrix characteristic polynomial
    - Matrix SVD decomposition
    - Get the Moore-Penrose generalized inverse of any matrix


## Related instructions

- As for solving the eigenvalue and singular values of the matrix, if the problem is highly sensitive, the calculated eigenvalue/singular values may be biased
- Any question, bug report or improvement suggestion is welcome through [opening up a new issue](https://github.com/TCL606/Matrix/issues)

## References

- 《线性代数入门》 梁鑫，田垠，杨一龙 编著。（清华大学内部讲义 2020年秋）
- [Faddeev–LeVerrier algorithm - Wikipedia](https://en.wikipedia.org/wiki/Faddeev–LeVerrier_algorithm)
- [Power iteration - Wikipedia](https://en.wikipedia.org/wiki/Power_iteration)
- [Aberth method - Wikipedia](https://en.wikipedia.org/wiki/Aberth_method)

# C++ / C# 矩阵处理库

## 简介

- 这是一个 C++ 和 C# 的矩阵处理库，可以方便的对矩阵进行运算

  - C++处理库为master分支下的Matrix/TCL_Matrix.h
  - C#处理库位于master分支下的CSharp_Matrix
- 矩阵计算的GUI程序已发布，欢迎大家下载使用最新Release版本进行使用。详细说明请见[MatrixCal/README.md](https://github.com/TCL606/Matrix/blob/main/MatrixCal/README.md)

- C++ 与 C# 库实现的算法基本相同，包括

  - 基本运算：
    - 矩阵加法
    - 矩阵乘法
    - 矩阵转置
  - 矩阵常用运算：
    - 矩阵求逆
    - 矩阵Gauss-Jordan消元
    - 矩阵LU分解
    - 求矩阵秩
    - 求行列式
    - 矩阵快速幂
    - 求解线性方程组
    - 求矩阵零空间
    - 矩阵的迹
    - 矩阵QR分解
  - 常用数值计算
    - 求矩阵范数
    - 求矩阵谱半径
    - 求任意矩阵的所有奇异值（不含重根）
    - 求任意矩阵的（复）特征值（含重根）
  - 矩阵高级操作
    - 获得方阵特征多项式的系数
    - 矩阵SVD分解
    - 求任意矩阵的Moore-Penrose广义逆
  - 其他操作
    - 从文件中读入矩阵
    - 向文件中写入矩阵

## 相关说明

- 关于求解矩阵特征值、奇异值的部分，如果问题高度敏感，则计算出的特征值/奇异值可能有偏差
- 我们欢迎大家通过[issue](https://github.com/TCL606/Matrix/issues).，向我们询问任何问题，提出bug报告、或给予改进建议

## 参考资料

- 《线性代数入门》 梁鑫，田垠，杨一龙 编著。（清华大学内部讲义 2020年秋）
- [Faddeev–LeVerrier algorithm - Wikipedia](https://en.wikipedia.org/wiki/Faddeev–LeVerrier_algorithm)
- [Power iteration - Wikipedia](https://en.wikipedia.org/wiki/Power_iteration)
- [Aberth method - Wikipedia](https://en.wikipedia.org/wiki/Aberth_method)
