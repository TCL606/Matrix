using System;
using TCL_Matrix;

namespace MatrixTest
{
    public partial class UnitTest
    {
        public double SVDAcc(int sum = 10000, int maxDim = 10)
        {
            Matrix A;
            Matrix? U, Sigma, VT;
            int testrow, testcol;
            Random r = new Random(Environment.TickCount);
            int total = 0;
            int right = 0;
            for (int _ = 0; _ < sum; _++)
            {
                testrow = r.Next(maxDim - 1) + 1;
                testcol = r.Next(maxDim - 1) + 1;
                A = Matrix.RandomMatrix(testrow, testcol);
                if (A.SVD(out U, out Sigma, out VT))
                {
                    total++;
                    if(IsOrthogonal(U)&& IsOrthogonal(VT)&& IsEqual(U * Sigma * VT, A))
                        right++;
                }
            }
            return (double)right / (double)total;
        }

        public double QRAcc(int sum = 10000, int maxDim = 10)
        {
            Matrix? Q, R;
            Matrix A;
            int testcol, testrow;
            int total = 0;
            int right = 0;
            Random r = new Random(Environment.TickCount);
            for (int _ = 0; _ < sum; _++)
            {
                testcol = r.Next(maxDim - 1) + 1;
                testrow = testcol + r.Next(maxDim);
                A = Matrix.RandomMatrix(testrow, testcol);
                if (A.QR(out Q, out R))
                {
                    total++;
                    if (IsColOrthogonal(Q) && IsUpTri(R) && IsEqual(Q * R, A))
                        right++;
                }
            }
            return (double)right / (double)total;
        }

        public double LUAcc(int sum = 10000, int maxDim = 10)
        {
            Matrix A;
            Matrix? L, U;
            int right = 0, total = 0;
            Random r = new Random(Environment.TickCount);
            int test;
            for (int _ = 0; _ < sum; _++)
            {
                test = r.Next(maxDim - 1) + 1;
                A = Matrix.RandomMatrix(test, test);
                if (A.LU(out L, out U))
                {
                    total++;
                    if (IsUpTri(U) && IsDownTri(L) && IsEqual(L * U, A))
                        right++;
                }
            }
            return (double)right / (double)total;
        }
    }
}
