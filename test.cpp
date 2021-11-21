#include"TCL_Matrix.h"

/// <summary>
/// TCL_Matrix.h ʹ������
/// </summary>
/// <returns></returns>
int main()
{
    //����1����������
    double a[8][8] =   //ע������һ��������������������������������һ��������������ֵ
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
    Matrix A((double*)a, 8, 8);  //����һ��8*8��������aΪ����Ķ���
    A.Display(); //��ʾA����
    std::cout << "=======================" << std::endl;

    //����2������ת��
    Matrix A1 = Transpose(A); //����A1����ΪA�����ת��
    A1.Display();
    std::cout << "=======================" << std::endl;

    //����3�����������ֵ��Ҫ����󣨰룩�����򸺶���
    double x[8] = { 1,2,3,4,5,6,7,8 };  //����Ԥ���õ�����ֵ
    double c[8][8] =
    {
        {x[0]},{0,x[1]},{0,0,x[2]},{0,0,0,x[3]},
        {0,0,0,0,x[4]},{0,0,0,0,0,x[5]},{0,0,0,0,0,0,x[6]}
        ,{0,0,0,0,0,0,0,x[7]}
    };  
    Matrix C((double*)c, 8, 8); //�ԽǾ���
    Matrix D = A * C * A1; //D����������C������ֵΪC�ĶԽ�Ԫ��
    std::vector<double> v; //����һ��vector�����ڽ�������ֵ
    D.GetEigenValuesOfDefiniteMatrix(v);  //���ú�������þ������������ֵ
    for (auto& t : v) //��tΪ����ֵ����D-tI����ʽӦΪ0
    {                 //�����ӡ�����������ֵ��D-tI������ʽ���бȽϣ���֤�������ȷ��
        std::cout << "eigen value: " << t << std::endl;  
        Matrix B = D - t * (Matrix::IdentityMatrix(8));
        std::cout << "det(D-tI) = " << B.Determinant() << std::endl << std::endl;
    }
    std::cout << "=======================" << std::endl;

    //����4���������������ֵ
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

    //����5���������棺
    double p[16] =
    {
        1,0,0,0,
        0,2,1,1,
        0,0,5,0,
        1,0,0,2
    };
    Matrix P(p, 4, 4);
    if (P.InverseMatrix(P))  //�������
    {
        P.Display();
    }
    else
    {
        std::cout << "The matrix is not invertible!" << std::endl;
    }
    std::cout << "=======================" << std::endl;

    //����7��������Է�����
    double q[20] = //�����������
    {
        0,0,3,0,1,
        1,0,0,0,1,
        0,2,0,0,1,
        0,0,0,3,1
    };  
    Matrix S(q, 4, 5); //�������
    Matrix ret;        //���ս�ռ�
    if (S.GetAllSolutionsForAugmentedMatrix(ret))   //����н�
    {
        ret.Display();
    }
    else
    {
        std::cout << "The function has no solutions!" << std::endl;
    }
    return 0;
}