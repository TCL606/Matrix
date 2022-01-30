using System;
using TCL_Matrix;

namespace Program
{
    public class Program
    {
        static void Main()
        {
            Matrix m = new Matrix(5, 5);
            foreach(var i in m)
                Console.WriteLine(i);
            Matrix.WriteToFile(m, "C:\\Users\\admin\\Desktop\\test.txt");
        }
    }
}