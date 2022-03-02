using Microsoft.VisualStudio.TestTools.UnitTesting;
using TCL_Matrix;
using System;

namespace MatrixTest
{
    [TestClass]
    public static partial class UnitTest
    {
        public static bool IsEqual(Matrix A, Matrix B)
        {
            if (A.Col != B.Col || A.Row != B.Row)
                return false;
            for (int i = 0; i < A.Row; i++)
            {
                for (int j = 0; j < A.Col; j++)
                {
                    if (Math.Abs(A[i, j] - B[i, j]) > 0.01)
                        return false;
                }
            }
            return true;
        }
        public static bool IsUpTri(Matrix A)
        {
            if (A.Col != A.Row)
                return false;
            for (int i = 0; i < A.Col; i++)
            {
                for (int j = i + 1; j < A.Row; j++)
                {
                    if (Math.Abs(A[j, i]) > 0.01)
                        return false;
                }
            }
            return true;
        }
        public static bool IsDownTri(Matrix A)
        {
            if (A.Col != A.Row)
                return false;
            for (int i = 0; i < A.Row; i++)
            {
                for (int j = i + 1; j < A.Col; j++)
                {
                    if (Math.Abs(A[i, j]) > 0.01)
                        return false;
                }
            }
            return true;
        }
        public static bool IsColOrthogonal(Matrix Q)
        {
            return IsEqual(Q.Transpose() * Q, Matrix.IdentityMatrix(Q.Col));
        }
        public static bool IsOrthogonal(Matrix Q)
        {
            return Q.Col == Q.Row && IsEqual(Q.Transpose() * Q, Matrix.IdentityMatrix(Q.Col));
        }
    }
}