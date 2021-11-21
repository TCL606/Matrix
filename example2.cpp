#include "TCL_Matrix.h"

int main()
{
    double A[2][2] = {{1,2},{3,4}};
    Matrix m((double*)A,2,2,true);
    m.Display();
    m.Display_LU();

    double v[2] = {3, 7};
    m.GetSolutionFromLU(v);
    for (int i = 0; i < 2; i++)
    {
        std::cout << v[i] << ' ';
    }
}