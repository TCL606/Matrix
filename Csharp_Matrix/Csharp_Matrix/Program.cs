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
            var v = A?.GetAllEigenValues();
            foreach(var i in v)
                Console.WriteLine(i);
        }
    }
}