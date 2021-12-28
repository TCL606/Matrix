#include"TCL_Matrix.h"
#pragma warning(disable:4996)
using namespace TCL_Matrix;
/// <summary>
/// TCL_Matrix.h 使用样例
/// </summary>
/// <returns></returns>
int main()
{
    /*//样例1：创建矩阵
    double a[8][8] =   //注：这是一个正交矩阵，下面会用这个正交矩阵生成一个矩阵来求特征值
    {
        {0.33333333,-0.66666667,0,0.666666667,0,0,0,0},
        {-0.81649658,0,0.40824829,0.40824829,0,0,0,0},
        {0.21821789 * 2,0.21821789,0.21821789 * 4,0,0,0,0,0},
        {0.08908708 * 2,0.08908708 * 8,0.08908708 * -3,0.08908708 * 7,0,0,0,0},
        {0,0,0,0,0.33333333,-0.66666667,0,0.666666667},
        {0,0,0,0,-0.81649658,0,0.40824829,0.40824829},
        {0,0,0,0,0.21821789 * 2,0.21821789,0.21821789 * 4,0},
        {0,0,0,0,0.08908708 * 2,0.08908708 * 8,0.08908708 * -3,0.08908708 * 7}
    };
    Matrix A((double*)a, 8, 8);  //创建一个8*8，以数组a为矩阵的对象
    A.Display(10); //显示A矩阵
    std::cout << "=========================================================================================" << std::endl;

    //样例2：矩阵转置
    Matrix A1 = Transpose(A); //创建A1矩阵，为A矩阵的转置
    A1.Display(10);
    std::cout << "=========================================================================================" << std::endl;

    //样例3：求矩阵特征值（要求矩阵（半）正定或负定）
    double x[8] = { 1,2,3,4,5,6,7,8 };  //这是预备好的特征值
    double c[8][8] =
    {
        {x[0]},{0,x[1]},{0,0,x[2]},{0,0,0,x[3]},
        {0,0,0,0,x[4]},{0,0,0,0,0,x[5]},{0,0,0,0,0,0,x[6]}
        ,{0,0,0,0,0,0,0,x[7]}
    };  
    Matrix C((double*)c, 8, 8); //对角矩阵
    Matrix D = A * C * A1; //D正交相似于C，特征值为C的对角元素
    std::vector<double> v; //创建一个vector，用于接收特征值
    D.GetEigenValuesOfDefiniteMatrix(v,1e-5,50);  //调用函数，获得矩阵的所有特征值
    for (auto& t : v) //记t为特征值，则D-tI行列式应为0
    {                 //下面打印计算出的特征值和D-tI的行列式进行比较，验证结果的正确性
        std::cout << "eigen value: " << t << std::endl;  
        Matrix B = D - t * (Matrix::IdentityMatrix(8));
        std::cout << "det(D-tI) = " << B.Determinant() << std::endl << std::endl;
    }
    std::cout << "=======================" << std::endl;

    //样例4：求矩阵所有奇异值
    double y[6] =
    {
        1,1,0,
        0,1,1
    };
    Matrix Y(y, 2, 3);
    Y.GetAllSingularValues(v);
    for (const auto& i : v)
    {
        std::cout << "singular value: " << i << std::endl;
    }
    std::cout << "=======================" << std::endl;

    //样例5：矩阵求逆：
    double p[16] =
    {
        1,0,0,0,
        0,2,1,1,
        0,0,5,0,
        1,0,0,2
    };
    Matrix P(p, 4, 4);
    if (P.InverseMatrix(P))  //如果可逆
    {
        P.Display();
    }
    else
    {
        std::cout << "The matrix is not invertible!" << std::endl;
    }
    std::cout << "=======================" << std::endl;

    //样例7：求解线性方程组
    double q[20] = //这是增广矩阵
    {
        0,0,3,0,1,
        1,0,0,0,1,
        0,2,0,0,1,
        0,0,0,3,1
    };  
    Matrix S(q, 4, 5); //增广矩阵
    Matrix ret;        //接收解空间
    if (S.GetAllSolutionsForAugmentedMatrix(ret))   //如果有解
    {
        ret.Display();
    }
    else
    {
        std::cout << "The function has no solutions!" << std::endl;
    }
    std::cout << "=========================================================================================" << std::endl;
    

    //样例8：求矩阵的奇异值分解、简化奇异值分解
    double aUsedForSvd[5][5] = { {0,1},{0,0,1},{0,0,0,1},{0,0,0,0,1},{0,0,0,0,0} };
    //{ 
    //    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    //    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    //    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    //    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    //    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    //    {1,1,0,0,0,0,0,0,0,0,0,0,0,1,1},
    //    {1,1,0,0,0,0,0,0,0,0,0,0,0,1,1},
    //    {1,1,0,0,0,0,0,0,0,0,0,0,0,1,1},
    //    {1,1,0,0,0,1,1,1,1,1,0,0,0,1,1},
    //    {1,1,0,0,0,1,1,1,1,1,0,0,0,1,1},
    //    {1,1,0,0,0,1,1,1,1,1,0,0,0,1,1},
    //    {1,1,0,0,0,1,1,1,1,1,0,0,0,1,1},
    //    {1,1,0,0,0,1,1,1,1,1,0,0,0,1,1},
    //    {1,1,0,0,0,1,1,1,1,1,0,0,0,1,1},
    //    {1,1,0,0,0,1,1,1,1,1,0,0,0,1,1},
    //    {1,1,0,0,0,1,1,1,1,1,0,0,0,1,1},
    //    {1,1,0,0,0,1,1,1,1,1,0,0,0,1,1},
    //    {1,1,0,0,0,0,0,0,0,0,0,0,0,1,1},
    //    {1,1,0,0,0,0,0,0,0,0,0,0,0,1,1},
    //    {1,1,0,0,0,0,0,0,0,0,0,0,0,1,1},
    //    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    //    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    //    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    //    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    //    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    //};
    Matrix AUsedForSVD((double*)aUsedForSvd, 5, 5);
    AUsedForSVD.Display();
    Matrix U, Sigma, VT, Ur, Sigmar, VrT;
    if (AUsedForSVD.SVD(U, Sigma, VT))
    {
        std::cout << "SVD:" << std::endl << std::endl;
        U.Display(5);
        Sigma.Display();
        VT.Display();
    }
    else
    {
        std::cout << "No SVD！Wrong！" << std::endl;
    }
    if (AUsedForSVD.SimplifiedSVD(Ur, Sigmar, VrT))
    {
        std::cout << "Simplified SVD:" << std::endl << std::endl;
        Ur.Display(5);
        Sigmar.Display();
        VrT.Display();
    }
    else
    {
        std::cout << "No Simplified SVD！Wrong！" << std::endl;
    }
    std::cout << "=========================================================================================" << std::endl;
    */

    //样例9：广义逆与最小二乘法
    //y=ax2+bx+c，现有四个点(0,0),(1,8),(3,8),(4,20)，用广义逆进行最小二乘拟合
    /*double aMM[4][3] = { {1,0,0},{1,1,1},{1,3,9},{1,4,16} };
    Matrix MP((double*)aMM, 4, 3);
    MP.Display();
    MP = MP.MPGeneralizedInverseForFullRankMatrix();
    if (MP != NULL)
    {
        MP.Display();
        double yvec[4] = { 0,8,8,20 };
        (MP * (Matrix(yvec, 4, 1))).Display();
    }
    else
    {
        std::cout << "Error!" << std::endl;
    }
    std::cout << "=========================================================================================" << std::endl;*/

    //样例10：计算矩阵谱范数
    //double test[] = { 1,2,3,4,5,6 };
    //Matrix Test(test, 3, 2);
    //Test.Display();
    //std::cout << Test.SpectralNorm() << std::endl;
    //std::cout << "=========================================================================================" << std::endl; */


    //样例11：求矩阵谱半径
    //double radius[16] = {1,1,1,1,1,2,3,4,1,3,6,10,1,4,10,20 };
    //Matrix Radius(radius, 4, 4);
    //Radius.Display();
    //std::cout << Radius.SpectralRadius() << std::endl;
    //std::cout << "=========================================================================================" << std::endl; */

    Matrix file;
    if (ReadFromFile("C:\\Users\\admin\\Desktop\\t.txt", file))
    {
        file.Display();
        WriteToFile("C:\\Users\\admin\\Desktop\\ttt.txt", file);
    }
    return 0;
}