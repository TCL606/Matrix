using System;
using TCL_Matrix;

namespace Program
{
    public class Program
    {
        static void Main()
        {
            Matrix m = new Matrix(new double[,]{
                { 1,1,1,1},
                { 1,2,3,4},
                { 1,3,6,10},
                { 1,3,6,10},
            });
            Matrix? bases;
            m.GetBasesOfNullSpace(out bases);
            bases?.Display();
        }
    }
}