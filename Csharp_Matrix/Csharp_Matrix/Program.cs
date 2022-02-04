using System;
using TCL_Matrix;

namespace Program
{
    public class Program
    {
        static void Main()
        {
            var A = Matrix.ReadFromFile("C:\\Users\\admin\\Desktop\\test.txt");
            A?.Display();
            Matrix? U, VT, Sigma;
            if (A.SVD(out U, out Sigma, out VT))
            {
                U?.Display();
                Sigma?.Display();
                VT?.Display();
            }
        }
    }
}