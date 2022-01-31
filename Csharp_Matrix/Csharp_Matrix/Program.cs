using System;
using TCL_Matrix;

namespace Program
{
    public class Program
    {
        static void Main()
        {
            //var A = Matrix.ReadFromFile("C:\\Users\\admin\\Desktop\\test.txt");
            //A?.Display();
            //var v = A?.GetAllEigenValues();
            //foreach(var i in v)
            //    Console.WriteLine(i);
            Matrix Lambda = new Matrix(10, 10);
            for(int i = 0; i < 10; i++)
            {
                Lambda[i, i] = i > 7 ? 1 : 2;
            }
            Lambda.Display();
            var X = Matrix.RandomMatrix(10, 10);
            X.Display();
            Matrix X_1 = new Matrix(10, 10);
            if(X.InverseMatrix(out X_1))
            {
                var Y = X * Lambda * X_1;
                var v = Y.GetAllEigenValues();
                foreach(var e in v)
                    Console.WriteLine(e);
            }
        }
    }
}