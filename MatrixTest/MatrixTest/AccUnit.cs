using System;
using TCL_Matrix;

namespace MatrixTest
{
    public static partial class UnitTest
    {
        public static double SVDAcc(int sum = 10000, int maxDim = 10)
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

        public static double QRAcc(int sum = 10000, int maxDim = 10)
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

        public static double LUAcc(int sum = 10000, int maxDim = 10)
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

        public static double EigenAcc(int maxEigen = 20, int sum = 10000, int maxDim = 10)
        {
            Matrix X, A, Y;
            int testrow;
            int total = 0, right = 0;
            Random r = new Random(Environment.TickCount);
            for (int _ = 0; _ < sum; _++)
            {
                testrow = r.Next(maxDim - 1) + 1;
                A = new Matrix(testrow, testrow);
                int[] eigen = new int[testrow];
                for(int i = 0; i < eigen.Length; i++)
                {
                    eigen[i] = r.NextDouble() * maxEigen * r.Next() % 2 == 1 ? 1 : -1;
                    A[i, i] = eigen[i];
                }
                //随机重数
                for (int i = 0; i < eigen.Length / 3; i++)
                {
                    int temp1 = r.Next(testrow);
                    int temp2 = r.Next(testrow);
                    eigen[temp1] = eigen[temp2];
                    A[temp1, temp1] = eigen[temp1];
                }
                X = Matrix.RandomMatrix(testrow, testrow);
                while(!X.InverseMatrix(out Y))
                {
                    X = Matrix.RandomMatrix(testrow, testrow);
                }
                A = X * A * Y;
                var li = A.GetAllEigenValues();
                bool succeed = true;
                if (li != null)
                {
                    total++;
                    bool flag;
                    foreach (var i in li)
                    {
                        flag = false;
                        for (int j = 0; j < eigen.Length; j++)
                        {
                            if (Math.Abs(eigen[j] - i.real) < Matrix.PRECISION_OF_DIFFERENCE)
                            {
                                eigen[j] = int.MaxValue;
                                flag = true;
                                break;
                            }
                        }
                        if(!flag)
                        {
                            succeed = false;
                            break;
                        }
                    }
                    if (succeed)
                        right++;
                }
            }
            return (double)right / (double)total;
        }
    }
}
