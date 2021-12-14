# Matrix

## 简介

- 使用TCL_Matrix.h，可以方便地对矩阵进行运算。

- 本程序更强调算法的准确性，可用于小规模问题的测试。

  对于大规模问题，程序中可能有大量对动态内存的申请、释放等操作，因此不保证程序有最高的效率。

- 目前，TCL_Matrix.h中实现了矩阵的操作包括：

  - 基本运算：
    - 矩阵加法
    - 矩阵乘法
    - 矩阵转置
  - 矩阵常用运算：
    - 矩阵求逆
    - 矩阵Gauss-Jordan消元
    - 求矩阵秩
    - 求行列式
    - 矩阵快速幂
    - 求解线性方程组
    - 求矩阵零空间
    - 矩阵的迹
  - 常用数值计算
    - 求矩阵范数
    - 求对称非不定矩阵的所有特征值（不含重根）
    - 求任意矩阵的所有奇异值（不含重根）
    - 求任意矩阵的（复）特征值（含重根）
  - 矩阵高级操作
    - 获得方阵特征多项式的系数
    - 求任意矩阵的SVD分解或简化SVD分解
    - 求行/列满秩矩阵的Moore-Penrose广义逆
    - 求任意矩阵的Moore-Penrose广义逆
  - 其他操作
    - 从文件中读入矩阵

## 可能的BUG

- 用程序对矩阵处理，往往要考虑诸多情况，难以保证绝对正确。目前已上传的代码，在大部分样例上是通过的。
  - 关于求解矩阵特征值、奇异值的部分，如果问题高度敏感，则计算出的特征值/奇异值可能有偏差。
- 鉴于本人能力有限，程序中仍然可能存在其他bug。在使用过程中如果发现bug，欢迎联系我。
- 欢迎各位向该矩阵类添加更多功能。

## 参考资料

- 《线性代数入门》 梁鑫，田垠，杨一龙 编著。（清华大学内部讲义 2020年秋）
- [Faddeev–LeVerrier algorithm - Wikipedia](https://en.wikipedia.org/wiki/Faddeev–LeVerrier_algorithm)
- [Power iteration - Wikipedia](https://en.wikipedia.org/wiki/Power_iteration)
- [Aberth method - Wikipedia](https://en.wikipedia.org/wiki/Aberth_method)

