using System;
using TCL_Matrix;

namespace Program
{
    public class Program
    {
        static void Main()
        {
            //var A = Matrix.ReadFromFile("C:\\Users\\admin\\Desktop\\test.txt");
            Matrix A = Matrix.RandomMatrix(4, 3);
            A?.Display();
            Matrix? Q, R;
            if (A.QR(out Q, out R))
            {
                Q?.Display();
                R?.Display();
                (Q.Transpose()*Q).Display();
                (Q * R).Display();
            }
        }
    }
}