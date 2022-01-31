using System.Collections;
using System.IO;

namespace TCL_Matrix
{
    public class Matrix : ICloneable, IEnumerable
    {
        private static void ExceptionHandling(Exception e)
        {
            Console.WriteLine(e.Message);
        }
        private const double PRECISION_OF_DIFFERENCE = 1e-3;

        protected int row;
        public int Row { get => row; }

        protected int col;
        public int Col { get => col; }

        public double[,] matrix { get; set; }

        #region 接口实现

        /// <summary>
        /// 克隆
        /// </summary>
        /// <returns></returns>
        public object Clone()
        {
            Matrix ret = new Matrix(row, col);
            if (matrix == null)
                ret.matrix = null;
            else
            {
                Array.Copy(matrix, ret.matrix, row * col);
            }
            return ret;
        }

        /// <summary>
        /// 迭代
        /// </summary>
        /// <returns></returns>
        IEnumerator IEnumerable.GetEnumerator()
        {
            return GetEnumerator();
        }
        public MatrixEnum GetEnumerator()
        {
            return new MatrixEnum(matrix);
        }
        #endregion

        #region 辅助函数
        /// <summary>
        /// 创建一个空矩阵
        /// </summary>
        /// <param name="row"></param>
        /// <param name="col"></param>
        public Matrix(int row, int col)
        {
            try
            {
                if (row <= 0)
                    throw new ArgumentOutOfRangeException(nameof(row), "The matrix row is not a positive integar!");
                if (col <= 0)
                    throw new ArgumentOutOfRangeException(nameof(col), "The matrix col is not a positive integar!");
                matrix = new double[row, col];
                this.row = row;
                this.col = col;
            }
            catch (Exception e)
            {
                ExceptionHandling(e);
                this.row = 0;
                this.col = 0;
                matrix = null;
            }
        }

        /// <summary>
        /// 创建一个矩阵
        /// </summary>
        /// <param name="m"></param>
        public Matrix(double[,] m)
        {
            this.row = m.GetLength(0);
            this.col = m.GetLength(1);
            this.matrix = m;
        }

        /// <summary>
        /// 显示
        /// </summary>
        /// <param name="width"></param>
        /// <param name="precision"></param>
        public void Display(int width = 8, int precision = 2)
        {
            try
            {
                if (matrix == null)
                    throw new Exception("Try to display a null matrix!");
                for (int i = 0; i < row; i++)
                    for (int j = 0; j < col; j++)
                    {
                        Console.Write(matrix[i, j].ToString($"F{precision}").PadLeft(width, ' '));
                        if (j == col - 1)
                            Console.WriteLine("");
                    }
                Console.WriteLine();
            }
            catch (Exception e)
            {
                ExceptionHandling(e);
            }
        }

        #region 产生矩阵
        static public Matrix IdentityMatrix(int n)
        {
            Matrix I = new Matrix(n, n);
            for (int i = 0; i < n; i++)
            {
                I.matrix[i, i] = 1;
            }
            return I;
        }

        static public Matrix RandomMatrix(int row, int col, double MaxPossibleValue = 10)
        {
            Matrix ret = new Matrix(row, col);
            Random r = new Random(Environment.TickCount);
            for (int i = 0; i < row; i++)
            {
                for(int j = 0; j < col; j++)
                {
                    ret.matrix[i, j] = r.NextDouble() * MaxPossibleValue * (r.Next() % 2 == 1 ? 1 : -1);
                }
            }
            return ret;
        }
        #endregion

        #region 文件读写
        public static bool WriteToFile(Matrix m, String path)
        {
            try 
            {   
                using(StreamWriter sw = new StreamWriter(path))
                {
                    sw.WriteLine($"{m.row}  {m.col}");
                    for(int i = 0; i < m.row; i++)
                    {
                        for(int j = 0; j < m.col; j++)
                        {
                            sw.Write($"{m.matrix[i, j]} ");
                        }
                        sw.WriteLine("");
                    }
                }
                return true;
            }
            catch(Exception e)
            {
                ExceptionHandling(e);
                return false;
            }
        }

        public static Matrix? ReadFromFile(String path)
        {
            try
            {
                int row, col;
                Matrix m;
                using (StreamReader sr = new StreamReader(path))
                {
                    var str = sr.ReadLine()?.Split(' ');
                    if (str != null && str.Length >= 2)
                    {
                        row = int.Parse(str[0]);
                        col = int.Parse(str[1]);
                        m = new Matrix(row, col);
                    }
                    else throw new Exception("Failed to read row and col!");
                    str = sr.ReadLine()?.Split(' ');
                    int k = 0;
                    while (str != null && k < row)
                    {
                        if (str.Length >= col)
                        {
                            for (int i = 0; i < col; i++)
                                m.matrix[k, i] = int.Parse(str[i]);
                            k++;
                            str = sr.ReadLine()?.Split(' ');
                        }
                        else throw new Exception("Failed to read the matrix!");
                    }
                    return m;
                }
            }
            catch(Exception e)
            {
                ExceptionHandling(e);
                return null;
            }
        }
        #endregion

        #endregion

        #region 计算函数

        #region 高斯消元相关
        /// <summary>
        /// 高斯消元
        /// </summary>
        public void Gauss_Elimination()
        {
            try
            {
                if (matrix == null)
                    throw new Exception("Gauss Elimination on a null matrix!");
                int zeroRow = 0;
                for (int i = 0; i < col && i < row; i++)  //i看成列
                {
                    int nonZero = row - 1;  //先设非零行为最后一行
                    for (int j = i - zeroRow; j < row; j++) // 找到非0元素
                        if (Math.Abs(matrix[j, i]) > PRECISION_OF_DIFFERENCE)
                        {
                            nonZero = j;
                            break;
                        }

                    if (Math.Abs(matrix[nonZero, i]) > PRECISION_OF_DIFFERENCE)
                    {
                        if (nonZero != i - zeroRow) //如果非0元素不是第i-zeroRow行
                        {
                            for (int k = i; k < col; k++) // 把非0元素所在行交换到当前行
                            {
                                double t = matrix[i - zeroRow, k];
                                matrix[i - zeroRow, k] = matrix[nonZero, k];
                                matrix[nonZero, k] = t;
                            }
                            nonZero = i - zeroRow;
                        }

                        for (int j = i - zeroRow + 1; j < row; j++) //把下面部分消为0
                        {
                            if (Math.Abs(matrix[j, i]) > PRECISION_OF_DIFFERENCE) //如果matrix[j , i]不是0
                            {
                                double temp = matrix[j, i] / matrix[i - zeroRow, i];
                                for (int k = i; k < col; k++)
                                {
                                    matrix[j, k] = matrix[j, k] - matrix[i - zeroRow, k] * temp;
                                }
                            }
                        }
                    }
                }
            }
            catch (Exception e)
            {
                ExceptionHandling(e);
            }
        }

        /// <summary>
        /// 高斯-约当消元
        /// </summary>
        public void Gauss_Jordan_Elimination()
        {
            try
            {
                if (matrix == null)
                    throw new Exception("Gauss Jordan Elimination on a null matrix!");
                int zeroRow = 0;
                int rank = -1;
                for (int i = 0; i < col && i < row; i++)  //i看成列
                {
                    int nonZero = row - 1;  //先设非零行为最后一行
                    for (int j = i - zeroRow; j < row; j++) // 找到非0元素
                        if (Math.Abs(matrix[j, i]) > PRECISION_OF_DIFFERENCE)
                        {
                            nonZero = j;
                            break;
                        }

                    if (Math.Abs(matrix[nonZero, i]) > PRECISION_OF_DIFFERENCE)
                    {
                        if (nonZero != i - zeroRow) //如果非0元素不是第i-zeroRow行
                        {
                            for (int k = i; k < col; k++) // 把非0元素所在行交换到当前行
                            {
                                double t = matrix[i - zeroRow, k];
                                matrix[i - zeroRow, k] = matrix[nonZero, k];
                                matrix[nonZero, k] = t;
                            }
                            nonZero = i - zeroRow;
                        }

                        if (Math.Abs(matrix[i - zeroRow, i] - 1.0) > PRECISION_OF_DIFFERENCE)  //归一化
                        {
                            double temp = matrix[i - zeroRow, i];
                            for (int k = i; k < col; k++)
                            {
                                matrix[i - zeroRow, k] /= temp;
                            }
                        }

                        for (int j = i - zeroRow + 1; j < row; j++) //把下面部分消为0
                        {
                            if (Math.Abs(matrix[j, i]) > PRECISION_OF_DIFFERENCE) //如果matrix[j , i]不是0
                            {
                                double temp = matrix[j, i];
                                for (int k = i; k < col; k++)
                                {
                                    matrix[j, k] = matrix[j, k] - matrix[i - zeroRow, k] * temp;
                                }
                            }
                        }

                        for (int j = i - zeroRow - 1; j >= 0; j--) //把上面部分消为0
                        {
                            if (Math.Abs(matrix[j, i]) > PRECISION_OF_DIFFERENCE)
                            {
                                double temp = matrix[j, i];
                                for (int k = i; k < col; k++)
                                {
                                    matrix[j, k] = matrix[j, k] - matrix[i - zeroRow, k] * temp;
                                }
                            }
                        }
                    }
                    else
                    {
                        zeroRow++;
                    }
                }
                if (col > row)
                    rank = row - zeroRow;
                else rank = col - zeroRow;
                if (col > row && rank < row) //若此时不满秩，且col>row，则最后一行可能不会被归一化，且最后一行的秩不会被计算，要单独处理
                {
                    int k = row;
                    while (Math.Abs(matrix[row - 1, k]) < PRECISION_OF_DIFFERENCE && k < col)
                    {
                        k++;
                    }
                    if (k < col)
                    {
                        double temp = matrix[row - 1, k];
                        for (int i = k; i < col; i++)
                        {
                            matrix[row - 1, i] /= temp;
                        }
                        rank++; //加上最后一行的秩
                        for (int j = row - 2; j >= 0; j--) //把上面部分消为0
                        {
                            if (Math.Abs(matrix[j, k]) > PRECISION_OF_DIFFERENCE)
                            {
                                double tem = matrix[j, k];
                                for (int t = k; t < col; t++)
                                {
                                    matrix[j, t] = matrix[j, t] - matrix[row - 1, t] * tem;
                                }
                            }
                        }
                    }
                }
            }
            catch (Exception e)
            {
                ExceptionHandling(e);
            }
        }

        /// <summary>
        /// 计算秩
        /// </summary>
        /// <returns></returns>
        public int GetRank()
        {
            int rank = 0;

            try
            {
                if (this.matrix == null)
                    throw new Exception("Try to get a null matrix's rank!");
                Matrix newMat = this.Clone() as Matrix;
                newMat?.Gauss_Elimination();
                for (int i = 0; i < row; i++)
                {
                    if (Math.Abs(newMat.matrix[i, i]) > PRECISION_OF_DIFFERENCE)
                        rank++;
                    else break;
                }
            }
            catch (Exception e)
            {
                ExceptionHandling(e);
                rank = -1;
            }
            return rank;
        }

        /// <summary>
        /// 求行列式
        /// </summary>
        /// <returns></returns>
        public double Determinant()
        {
            try
            {
                if (this.matrix == null)
                    throw new Exception("Try to calculate the determinant of null matrix! Return 0.");
                if (row != col)
                    throw new Exception("Try to calculate the determinant of a non-square matrix! Return 0.");
                int swapTimes = 0; //行交换次数
                double[,] mtemp = new double[row, row];
                for (int i = 0; i < row; i++)
                    for (int j = 0; j < col; j++)
                    {
                        mtemp[i, j] = matrix[i, j];
                    }
                int zeroRow = 0;
                for (int i = 0; i < col && i < row; i++)  //i看成列
                {
                    int nonZero = row - 1;  //先设非零行为最后一行
                    for (int j = i - zeroRow; j < row; j++) // 找到非0元素
                        if (Math.Abs(mtemp[j, i]) > PRECISION_OF_DIFFERENCE)
                        {
                            nonZero = j;
                            break;
                        }

                    if (Math.Abs(mtemp[nonZero, i]) > PRECISION_OF_DIFFERENCE)
                    {
                        if (nonZero != i - zeroRow) //如果非0元素不是第i-zeroRow行
                        {
                            for (int k = i; k < col; k++) // 把非0元素所在行交换到当前行
                            {
                                double t = mtemp[i - zeroRow, k];
                                mtemp[i - zeroRow, k] = mtemp[nonZero, k];
                                mtemp[nonZero, k] = t;
                            }
                            nonZero = i - zeroRow;
                            swapTimes++;
                        }

                        for (int j = i - zeroRow + 1; j < row; j++) //把下面部分消为0
                        {
                            if (Math.Abs(mtemp[j, i]) > PRECISION_OF_DIFFERENCE) //如果mtemp[j , i]不是0
                            {
                                double temp = mtemp[j, i] / mtemp[i - zeroRow, i];
                                for (int k = i; k < col; k++)
                                {
                                    mtemp[j, k] = mtemp[j, k] - mtemp[i - zeroRow, k] * temp;
                                }
                            }
                        }
                    }
                }
                double ret = 1;
                if (Math.Abs(mtemp[row - 1, col - 1]) < PRECISION_OF_DIFFERENCE)
                    ret = 0;
                else
                {
                    for (int i = 0; i < row; i++)
                    {
                        ret *= mtemp[i, i];
                    }
                    if (swapTimes % 2 == 1)
                        ret = -ret;
                }

                return ret;
            }
            catch (Exception e)
            {
                ExceptionHandling(e);
                return 0;
            }
        }

        /// <summary>
        /// 计算逆矩阵，矩阵不可逆时，inverse为零矩阵
        /// </summary>
        /// <param name="inverse"></param>
        /// <returns></returns>
        public bool InverseMatrix(out Matrix inverse)
        {
            inverse = new Matrix(row, row);
            try
            {
                if (this.matrix == null)
                {
                    throw new Exception("The matrix is null! Inverse Failed.");
                }
                if (row != col)
                {
                    throw new Exception("The matrix is not a square! Inverse Failed.");
                }
                Matrix caculateInverse = new Matrix(row, 2 * row);
                for (int i = 0; i < row; i++)
                    for (int j = 0; j < row; j++)
                    {
                        caculateInverse.matrix[i, j] = matrix[i, j];
                        caculateInverse.matrix[i, j + row] = i == j ? 1 : 0;
                    }
                caculateInverse.Gauss_Jordan_Elimination();
                if (Math.Abs(caculateInverse.matrix[row - 1, row - 1]) < PRECISION_OF_DIFFERENCE) //高斯消元后，右下角为0，则不可逆；否则可逆。
                    return false;
                for (int i = 0; i < row; i++)
                {
                    for (int j = 0; j < row; j++)
                    {
                        inverse.matrix[i, j] = caculateInverse.matrix[i, j + row];
                    }
                }
                return true;
            }
            catch (Exception e)
            {
                ExceptionHandling(e);
                return false;
            }
        }

        /// <summary>
        /// 计算增广矩阵的所有解
        /// </summary>
        /// <param name="ret">前col-1列是零空间的基，最后一列是特解</param>
        /// <returns>无解时，ret返回null</returns>
        public bool GetAllSolutionsForAugmentedMatrix(out Matrix? ret)
        {
            try
            {
                if (this.matrix == null)
                    throw new Exception("Try to get solutions on null matrix!");

                Matrix original = this.Clone() as Matrix;
                int zeroRow = 0;
                double temp;
                for (int i = 0; i < original.col - 1 && i < original.row; i++)  //i看成列，此时是增广矩阵，i最多到col - 1
                {
                    int nonZero = original.row - 1;  //先设非零行为最后一行
                    for (int j = i - zeroRow; j < original.row; j++) // 找到非0元素
                        if (Math.Abs(original.matrix[j, i]) > PRECISION_OF_DIFFERENCE)
                        {
                            nonZero = j;
                            break;
                        }
                    if (Math.Abs(original.matrix[nonZero, i]) > PRECISION_OF_DIFFERENCE)
                    {
                        if (nonZero != i - zeroRow) //如果非0元素不是第i-zeroRow行
                        {
                            for (int k = i; k < original.col; k++) // 把非0元素所在行交换到当前行
                            {
                                temp = original.matrix[i - zeroRow, k];
                                original.matrix[i - zeroRow, k] = original.matrix[nonZero, k];
                                original.matrix[nonZero, k] = temp;
                            }
                            nonZero = i - zeroRow;
                        }

                        if (Math.Abs(original.matrix[i - zeroRow, i] - 1.0) > PRECISION_OF_DIFFERENCE)  //归一化
                        {
                            temp = original.matrix[i - zeroRow, i];
                            for (int k = i; k < original.col; k++)
                            {
                                original.matrix[i - zeroRow, k] /= temp;
                            }
                        }

                        for (int j = i - zeroRow + 1; j < original.row; j++) //把下面部分消为0
                        {
                            if (Math.Abs(original.matrix[j, i]) > PRECISION_OF_DIFFERENCE) //如果matrix[j , i]不是0
                            {
                                temp = original.matrix[j, i];
                                for (int k = i; k < original.col; k++)
                                {
                                    original.matrix[j, k] = original.matrix[j, k] - original.matrix[i - zeroRow, k] * temp;
                                }
                            }
                        }

                        for (int j = i - zeroRow - 1; j >= 0; j--) //把上面部分消为0
                        {
                            if (Math.Abs(original.matrix[j, i]) > PRECISION_OF_DIFFERENCE)
                            {
                                temp = original.matrix[j, i];
                                for (int k = i; k < original.col; k++)
                                {
                                    original.matrix[j, k] = original.matrix[j, k] - original.matrix[i - zeroRow, k] * temp;
                                }
                            }
                        }
                    }
                    else
                    {
                        zeroRow++;
                    }
                }
                int rank;
                if (original.col - 1 > original.row)
                    rank = original.row - zeroRow;  //这里是指原矩阵的秩，不是增广矩阵的秩。
                else rank = original.col - 1 - zeroRow;
                if (original.col - 1 > original.row && rank < original.row) //若此时原矩阵不满秩，且col - 1>row，则最后一行可能不会被归一化，且最后一行的秩不会被计算，要单独处理
                {
                    int k = original.row;
                    while (Math.Abs(original.matrix[original.row - 1, k]) < PRECISION_OF_DIFFERENCE && k < original.col - 1)
                    {
                        k++;
                    }
                    if (k < original.col - 1)
                    {
                        temp = original.matrix[original.row - 1, k];
                        for (int i = k; i < original.col; i++)
                        {
                            original.matrix[original.row - 1, i] /= temp;
                        }
                        rank++; //加上最后一行的秩
                        for (int j = original.row - 2; j >= 0; j--) //把上面部分消为0
                        {
                            if (Math.Abs(original.matrix[j, k]) > PRECISION_OF_DIFFERENCE)
                            {
                                temp = original.matrix[j, k];
                                for (int t = k; t < original.col; t++)
                                {
                                    original.matrix[j, t] = original.matrix[j, t] - original.matrix[original.row - 1, t] * temp;
                                }
                            }
                        }
                    }
                }
                //先看有没有解，即看行简化阶梯型的全零行对应增广矩阵最右行的元素是否为0
                for (int i = original.row - 1; i >= rank; i--)
                {
                    if (Math.Abs(original.matrix[i, original.col - 1]) > PRECISION_OF_DIFFERENCE)
                    {
                        ret = null;
                        return false;
                    }
                }

                ret = new Matrix(original.col - 1, original.col - rank);
                int numOfFreeColumn = 0;
                int tem = 0;
                for (tem = 0; tem < original.col - 1 && tem - numOfFreeColumn < original.row; tem++) //先找零空间的基
                {
                    if (Math.Abs(original.matrix[tem - numOfFreeColumn, tem]) > PRECISION_OF_DIFFERENCE)
                        continue;
                    else
                    {
                        ret.matrix[tem, numOfFreeColumn] = 1;
                        for (int j = 0; j < tem - numOfFreeColumn; j++)
                        {
                            ret.matrix[j, numOfFreeColumn] = -original.matrix[j, tem];
                        }
                        numOfFreeColumn++;
                    }
                }
                while (tem < original.col - 1)
                {
                    ret.matrix[tem, numOfFreeColumn] = 1;
                    for (int j = 0; j < original.row; j++)
                    {
                        ret.matrix[j, numOfFreeColumn] = -original.matrix[j, tem];
                    }
                    numOfFreeColumn++;
                    tem++;
                }

                numOfFreeColumn = 0;
                //找特解
                for (tem = 0; tem < original.col - 1 && tem - numOfFreeColumn < original.row; tem++)  //主列位置对应分量为目标向量的对应分量，自由列对应分量直接取0
                {
                    if (Math.Abs(original.matrix[tem - numOfFreeColumn, tem]) > PRECISION_OF_DIFFERENCE)
                    {
                        ret.matrix[tem, original.col - rank - 1] = original.matrix[tem - numOfFreeColumn, original.col - 1];
                    }
                    else
                    {
                        numOfFreeColumn++;
                        ret.matrix[tem, original.col - rank - 1] = 0;
                    }
                }
                while (tem < original.col - 1)
                {
                    ret.matrix[tem++, original.col - rank - 1] = 0;
                }
                return true;
            }
            catch (Exception e)
            {
                ExceptionHandling(e);
                ret = null;
                return false;
            }
        }

        /// <summary>
        /// 计算矩阵零空间
        /// </summary>
        /// <param name="bases"></param>
        /// <returns>矩阵可逆时，bases返回null</returns>
        public bool GetBasesOfNullSpace(out Matrix? bases)
        {
            try
            {
                if (this.matrix == null)
                    throw new Exception("Try to get null space of null matrix!");
                Matrix ret = new Matrix(row, col + 1);
                for (int i = 0; i < row; i++)
                {
                    for (int j = 0; j < col; j++)
                    {
                        ret.matrix[i, j] = matrix[i, j];
                    }
                }
                if (ret.GetAllSolutionsForAugmentedMatrix(out ret))
                {
                    if (ret.col == 1)
                        throw new Exception("The matrix's null space only contains 0 so that it has no bases.");
                    Matrix nullSpace = new Matrix(ret.row, ret.col - 1);
                    for (int i = 0; i < nullSpace.row; i++)
                    {
                        for (int j = 0; j < nullSpace.col; j++)
                        {
                            nullSpace.matrix[i, j] = ret.matrix[i, j];
                        }
                    }
                    bases = nullSpace;
                    return true;
                }
                else
                {
                    throw new Exception("Something went wrong so that the matrix's null space is empty!");
                }
            }
            catch (Exception e)
            {
                ExceptionHandling(e);
                bases = null;
                return false;
            }
        }
        #endregion

        #region 矩阵分解
        public bool LU(out Matrix? L, out Matrix? U)
        {
            try
            {
                if (this.matrix == null)
                    throw new Exception("Try to LU factorization on a null matrix!");
                if (col != row)
                    throw new Exception("The matrix is not a square.LU decomposition failed.");
                L = IdentityMatrix(col);
                U = this.Clone() as Matrix;
                for (int i = 0; i < col; i++)
                {
                    if (Math.Abs(U.matrix[i, i]) > PRECISION_OF_DIFFERENCE)
                    {
                        for (int j = i + 1; j < row; j++)
                        {
                            L.matrix[j, i] = U.matrix[j, i] / U.matrix[i, i];
                            for (int k = i; k < col; k++)
                            {
                                U.matrix[j, k] -= L.matrix[j, i] * U.matrix[i, k];
                            }
                        }
                    }
                    else
                    {
                        bool flag = false;
                        for (int k = i + 1; k < row; k++)
                        {
                            if (Math.Abs(U.matrix[k, i]) > PRECISION_OF_DIFFERENCE)
                            {
                                flag = true;
                                break;
                            }
                        }
                        if (flag)
                            throw new Exception("The matrix can't be LU decomposed.");
                    }
                }
                return true;
            }
            catch (Exception e)
            {
                ExceptionHandling(e);
                L = null;
                U = null;
                return false;
            }
        }
        #endregion

        /// <summary>
        /// 求矩阵的迹
        /// </summary>
        /// <returns>若不是方阵，则返回Double.MaxValue</returns>
        public double Trace()
        {
            try
            {
                if (row != col)
                {
                    throw new Exception("The matrix is not a square so trace is not defined.");
                }
                double tr = 0;
                for (int i = 0; i < row; i++)
                {
                    tr += matrix[i, i];
                }
                return tr;
            }
            catch(Exception e)
            {
                ExceptionHandling(e);
                return Double.MaxValue;
            }
        }

        /// <summary>
        /// 矩阵的幂，矩阵必须为方阵
        /// </summary>
        /// <param name="n">必须是自然数</param>
        /// <returns>若矩阵不为方阵或参数小于0，则返回原矩阵</returns>
        public Matrix Power(int n)
        {
            try
            {
                if (row != col)
                {
                    throw new Exception("The matrix is not a square!");
                }
                if (n < 0)
                {
                    throw new Exception("The specified power is less than 0! No changes!");
                }
                if (this.matrix == null)
                {
                    throw new Exception("Null matrix cannot be exponentiated!");
                }
                else
                {
                    Matrix ori = new Matrix(this.matrix);
                    Matrix ret = IdentityMatrix(row);
                    while (n > 0)
                    {
                        if (n % 2 >= 1)
                        {
                            ret *= ori;
                            n -= 1;
                            ori *= ori;
                            n /= 2;
                        }
                        else
                        {
                            ori *= ori;
                            n /= 2;
                        }
                    }
                    return ret;
                }
            }
            catch (Exception e)
            {
                ExceptionHandling(e);
                return this;
            }
        }

        /// <summary>
        /// 矩阵转置
        /// </summary>
        /// <returns></returns>
        public Matrix Transpose()
        {
            Matrix ret = new Matrix(col, row);
            for (int i = 0; i < col; i++)
            {
                for (int j = 0; j < row; j++)
                {
                    ret.matrix[i, j] = this.matrix[j, i];
                }
            }
            return ret;
        }

        #region 特征值相关

        /// <summary>
        /// 计算方阵特征多项式系数
        /// </summary>
        /// <returns></returns>
        public IList GetCoefficientsOfCharacteristicPolynomial()
        {
            List<double> li = new List<double>();
            try
            {
                if (col != row)
                {
                    throw new Exception("The matrix is not a square so its characteristic polynomial is not defined.");
                }
                double[] tr = new double[col + 1];
                double[] co = new double[col + 1];
                Matrix A = new Matrix(matrix);
                for (int i = 1; i <= col; i++)
                {
                    tr[i] = A.Trace();
                    if (i != col)
                        A *= this;
                }
                co[col] = 1;
                for (int i = col - 1; i >= 0; i--)
                {
                    co[i] = 0;
                    for (int j = col - i; j >= 1; j--)
                    {
                        co[i] += (co[i + j] * tr[j]);
                    }
                    co[i] /= (i - col);
                }
                for (int i = 0; i < col; i++)
                {
                    li.Add(co[i]);
                }
                li.Add(co[col]);
            }
            catch (Exception e)
            {
                ExceptionHandling(e);
                li.Clear();
            }
            return li;
        }

        /// <summary>
        /// Aberth方法计算矩阵所有特征值
        /// </summary>
        /// <param name="possibleMaxMultiplicity"></param>
        /// <param name="aberthIteration"></param>
        /// <param name="newtonIteration"></param>
        /// <param name="possibleMaxMod"></param>
        /// <returns></returns>
        public IList GetAllEigenValues(int aberthIteration = 700, int newtonIteration = 5, int possibleMaxMod = 150)
        {
            List<Complex> v = new List<Complex>();
            Random rand = new Random();
            try
            {
                if (col != row)
                {
                    throw new Exception("The matrix is not a square so it doesn't have eigenvalues.");
                }
                List<double> coeff = GetCoefficientsOfCharacteristicPolynomial() as List<double>;
                int possibleMaxMultiplicity = row;
                double[][] poly = new double[possibleMaxMultiplicity + 1][];
                for (int i = 0; i < possibleMaxMultiplicity + 1; i++)
                {
                    poly[i] = new double[col + 1];
                }
                for (int i = 0; i < col + 1; i++)
                {
                    poly[0][i] = coeff[i];
                }
                for (int i = 1; i < possibleMaxMultiplicity + 1; i++)
                {
                    for (int j = 0; j < col + 1 - i; j++)
                    {
                        poly[i][j] = poly[i - 1][j + 1] * (j + 1);
                    }
                }
                Complex[] roots = new Complex[col];
                Complex[] multiroots = new Complex[col];
                int[] multiplicity = new int[col];
                int multirootnum = 0;
                for (int i = 0; i < col; i++)
                {
                    roots[i] = new Complex(rand.Next(2 * possibleMaxMod) - possibleMaxMod, rand.Next(2 * possibleMaxMod) - possibleMaxMod);
                }

                //Aberth
                Complex[] w = new Complex[col];
                Complex one = new Complex(1, 0);
                Complex zero = new Complex(0, 0);
                Complex sum;
                Func<double[], int, Complex, Complex> PolynomialFunc = (double[] coeff, int degree, Complex z) =>
                   {
                       Complex ret = new Complex(0, 0);
                       for (int i = 0; i < degree + 1; i++)
                       {
                           ret += (coeff[i] * Complex.Power(z, i));
                       }
                       return ret;
                   };
                Func<Complex, Complex, double, bool> Greater = (Complex z1, Complex z2, double precision) =>
                  {
                      if (z1.real > precision + z2.real)
                          return true;
                      else if (z1.real + precision < z2.real)
                          return false;
                      else if (Math.Abs(z1.imag) - Math.Abs(z2.imag) > precision)
                          return true;
                      else if (Math.Abs(z1.imag) < Math.Abs(z2.imag) + precision)
                          return false;
                      else if (z1.imag > z2.imag + precision)
                          return true;
                      else return false;
                  };
                while (aberthIteration > 0)
                {
                    aberthIteration--;
                    for (int i = 0; i < col; i++)
                    {
                        sum = zero;
                        Complex rate = PolynomialFunc(poly[0], col, roots[i]) / PolynomialFunc(poly[1], col - 1, roots[i]);
                        for (int j = 0; j < col; j++)
                        {
                            if (j != i)
                            {
                                sum += (one / (roots[i] - roots[j]));
                            }
                        }
                        w[i] = rate / (one - rate * sum);
                    }
                    for (int i = 0; i < col; i++)
                    {
                        roots[i] -= w[i];
                    }
                }
                
                //懒得定义新变量了，直接开辟一块区域算了
                {
                    int i, j, max = 0;
                    Complex temp;
                    for (i = 0; i < col - 1; i++)
                    {
                        max = i;
                        for (j = i + 1; j < col; j++)
                        {
                            if (Greater(roots[j], roots[max], 0.1 * PRECISION_OF_DIFFERENCE))
                            {
                                max = j;
                            }
                        }
                        temp = roots[i];
                        roots[i] = roots[max];
                        roots[max] = temp;
                    }
                }

                for (int i = 0; i < col; i++)
                    multiplicity[i] = 0;
                for (int i = 0; i < col; i++)
                {
                    int x = -1;
                    for (int j = 0; j < multirootnum; ++j)
                    {
                        if (Complex.Abs(multiroots[j] - roots[i]) < PRECISION_OF_DIFFERENCE * 20) //这里的精度不能太大
                        {
                            x = j;
                            break;
                        }
                    }
                    if (x != -1)
                    {
                        multiplicity[x]++;
                    }
                    else
                    {
                        multiroots[multirootnum] = roots[i];
                        multiplicity[multirootnum] = 1;
                        multirootnum++;
                    }
                }

                //Newton
                {
                    Complex z0, z1;
                    for (int i = 0; i < multirootnum; i++)
                    {
                        for (int j = 1; j <= possibleMaxMultiplicity; j++)
                        {
                            if (j == multiplicity[i])
                            {
                                for (int k = 0; k < newtonIteration; k++)
                                {
                                    multiroots[i] = multiroots[i] - PolynomialFunc(poly[j - 1], col - j + 1, multiroots[i]) / PolynomialFunc(poly[j], col - j, multiroots[i]);
                                }
                                break;
                            }
                            else if (j > possibleMaxMultiplicity)
                            {
                                for (int k = 0; k < newtonIteration; k++)
                                {
                                    multiroots[i] = multiroots[i] - PolynomialFunc(poly[possibleMaxMultiplicity - 1], col - possibleMaxMultiplicity + 1, multiroots[i]) / PolynomialFunc(poly[possibleMaxMultiplicity], col - possibleMaxMultiplicity, multiroots[i]);
                                }
                                break;
                            }
                        }
                    }
                    int x = 0;
                    for (int i = 0; i < multirootnum; i++)
                    {
                        for (int j = 0; j < multiplicity[i]; j++)
                        {
                            roots[x + j] = multiroots[i];
                        }
                        x += multiplicity[i];
                    }
                }

                for (int i = 0; i < col; i++)
                {
                    if (Math.Abs(roots[i].imag) < 0.1 * PRECISION_OF_DIFFERENCE)
                    {
                        v.Add(roots[i]);
                    }
                    else if (roots[i].imag >= 0)
                    {
                        v.Add(new Complex(roots[i].real, Math.Abs(roots[i].imag)));
                        v.Add(new Complex(roots[i].real, -Math.Abs(roots[i].imag)));
                    }
                }
            }
            catch(Exception e)
            {
                ExceptionHandling(e);
                v.Clear();
            }
            return v;
        }

        #endregion

        #endregion

        #region 操作符重载
        public static Matrix operator *(Matrix A, Matrix B)
        {
            try
            {
                if (A.matrix == null || B.matrix == null)
                {
                    throw new Exception("The matrix is null! Multiplication failed. Return the first matrix.");
                }
                if (A.col != B.row)
                {
                    throw new Exception("The dimension does not match! Matrix multiplication failed. Return the first matrix.");
                }
                Matrix ret = new Matrix(A.row, B.col);
                for (int i = 0; i < ret.row; i++)
                {
                    for (int j = 0; j < ret.col; j++)
                    {
                        for (int k = 0; k < A.col; k++)
                        {
                            ret.matrix[i, j] += A.matrix[i, k] * B.matrix[k, j];
                        }
                    }
                }
                return ret;
            }
            catch (Exception e)
            {
                ExceptionHandling(e);
                return A;
            }
        }
        public static Matrix operator *(double k, Matrix B)
        {
            try
            {
                if (B.matrix == null)
                {
                    throw new Exception("The matrix is null! Scalar multiplication failed. Return the original matrix.");
                }
                Matrix ret = new Matrix(B.row, B.col);
                for (int i = 0; i < ret.row; i++)
                {
                    for (int j = 0; j < ret.col; j++)
                    {
                        ret.matrix[i, j] = k * B.matrix[i, j];
                    }
                }
                return ret;
            }
            catch (Exception e)
            {
                ExceptionHandling(e);
            }
            return B;
        }
        public static Matrix operator +(Matrix A, Matrix B)
        {
            try
            {
                if (A.col != B.col || A.row != B.row)
                {
                    throw new Exception("The dimension does not match! Matrix addtion failed. Return the first matrix.");
                }
                Matrix ret = new Matrix(A.row, B.row);
                for (int i = 0; i < ret.row; i++)
                {
                    for (int j = 0; j < ret.col; j++)
                    {
                        ret.matrix[i, j] += A.matrix[i, j] + B.matrix[i, j];
                    }
                }
                return ret;
            }
            catch(Exception e)
            {
                ExceptionHandling(e);
            }
            return A;
        }
        public static Matrix operator -(Matrix A, Matrix B)
        {
            try
            {
                if (A.col != B.col || A.row != B.row)
                {
                    throw new Exception("The dimension does not match! Matrix subtraction failed. Return the first matrix.");
                }
                Matrix ret = new(A.row, A.col);
                for (int i = 0; i < ret.row; i++)
                {
                    for (int j = 0; j < ret.col; j++)
                    {
                        ret.matrix[i , j] += A.matrix[i , j] - B.matrix[i , j];
                    }
                }
                return ret;
            }
            catch (Exception e)
            {
                ExceptionHandling(e);
                return A;
            }
        }
        public static Matrix operator -(Matrix A)
        {
            try
            {
                Matrix ret = new(A.row, A.col);
                for (int i = 0; i < A.col; i++)
                {
                    for (int j = 0; j < A.row; j++)
                    {
                        ret[i, j] = -A.matrix[i, j];
                    }
                }
                return ret;
            }
            catch(Exception e)
            {
                ExceptionHandling(e);
                return A;
            }
        }
        public double this[int i, int j]
        {
            get
            {
                try
                {
                    if (matrix == null)
                        throw new Exception("Try to refer a null matrix! Return NAN.");
                    else if (i < 0 || j < 0 || i >= row || j >= col)
                        throw new Exception("Index out of range! Return NAN");
                }
                catch(Exception e)
                {
                    ExceptionHandling(e);
                    return double.NaN;
                }
                return matrix[i, j];
            }
            set
            {
                try
                {
                    if (matrix == null)
                        throw new Exception("Try to set a null matrix!");
                    else if (i < 0 || j < 0 || i >= row || j >= col)
                        throw new Exception("Index out of range!");
                }
                catch(Exception e)
                {
                    ExceptionHandling(e);
                    return;
                }
                matrix[i, j] = value;
            }
        }
        public Matrix? this[int j]
        {
            get
            {
                try
                {
                    if (matrix == null)
                        throw new Exception("Try to refer a null matrix! Return null.");
                    else if (j < 0 || j >= col)
                        throw new Exception("Index out of range! Return null");
                }
                catch (Exception e)
                {
                    ExceptionHandling(e);
                    return null;
                }
                Matrix ret = new Matrix(row, 1);
                for(int i = 0; i < row; i++)
                {
                    ret.matrix[i, 0] = matrix[i, j];
                }
                return ret;
            }
        }
        #endregion
    }

    public class MatrixEnum : IEnumerator
    {
        public double[,]? _matrix;
        private int row;
        private int col;
        private int row_position = 0;
        private int col_position = -1;

        public MatrixEnum(double[,]? matrix)
        {
            _matrix = matrix;
            row = matrix?.GetLength(0) ?? 0;
            col = matrix?.GetLength(1) ?? 0;
        }

        public bool MoveNext()
        {
            col_position++;
            if (col_position >= col)
            {
                col_position = 0;
                row_position++;
                return row_position < row;
            }
            else return true;
        }
        public void Reset()
        {
            row_position = 0;
            col_position = -1;
        }
        public double Current
        {
            get
            {
                try
                {
                    if (_matrix == null)
                        throw new NullReferenceException();
                    return _matrix[row_position, col_position];
                }
                catch (NullReferenceException e)
                {
                    throw e;
                }
                catch
                {
                    throw new InvalidOperationException();
                }
            }
        }

        // 装箱
        object IEnumerator.Current { get => Current; }
    }

    public class Complex
    {
        public double real;
        public double imag;
        public Complex(double real, double imag)
        {
            this.real = real;
            this.imag = imag;
        }
        public static double Abs(Complex z1)
        {
            return Math.Sqrt(z1.real * z1.real + z1.imag * z1.imag);
        }
        public static Complex operator +(Complex z1, Complex z2)
        {
            return new Complex(z1.real + z2.real, z1.imag + z2.imag);
        }
        public static Complex operator -(Complex z1, Complex z2)
        {
            return new Complex(z1.real - z2.real, z1.imag - z2.imag);
        }
        public static Complex operator +(Complex z1)
        {
            return new Complex(z1.real, z1.imag);
        }
        public static Complex operator -(Complex z1)
        {
            return new Complex(-z1.real, -z1.imag);
        }
        public static Complex operator *(Complex z1, Complex z2)
        {
            return new Complex(z1.real * z2.real - z1.imag * z2.imag, z1.imag * z2.real + z1.real * z2.imag);
        }
        public static Complex operator *(double k, Complex z1)
        {
            return new Complex(k * z1.real, k * z1.imag);
        }
        public static Complex operator /(Complex z1, Complex z2)
        {
            double z2mod2 = z2.real * z2.real + z2.imag * z2.imag;
            if (z2mod2 == 0)
                throw new Exception("Divided by zero!");
            return new((z1.real * z2.real + z1.imag * z2.imag) / z2mod2, (z1.imag * z2.real - z1.real * z2.imag) / z2mod2);
        }
        public static Complex Power(Complex z1, int n)
        {
            Complex ori = z1;
            Complex ret = new Complex(1, 0);
            if (n < 0)
            {
                ori = ret / ori;
                n = -n;
            }
            while (n > 0)
            {
                if (n % 2 >= 1)
                {
                    ret *= ori;
                    n -= 1;
                    ori *= ori;
                    n /= 2;
                }
                else
                {
                    ori *= ori;
                    n /= 2;
                }
            }
            return ret;
        }
        public override string ToString()
        {
            return new String($"({this.real}, {this.imag})");
        }
    }
}