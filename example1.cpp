#include "TCL_Matrix.h"

int main()
{
    double array[3][3] = {{1,2,3},{0,1,2},{3,4,5}};
    Matrix m((double*)(array),3,3);

    m.Display();

    std::cout << m.Determinant()<<' ' <<m.GetRank() <<std::endl;

    m.GetBasesOfNullSpace(m);
}