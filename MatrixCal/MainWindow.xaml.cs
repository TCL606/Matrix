using StringProcessing;
using System;
using System.Collections.Generic;
using System.Windows;
using System.Windows.Input;
using TCL_Matrix;

namespace MatrixCal
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            stack = new();
            err = new("");
            EnvalueFlag = false;
            STOflag = false;
            errflag = false;
            App.matpool = new();
            App.temppool = new();
            App.matpool.Add("0", new Matrix(1, 1));//字符0对应输入算式的暂时结果；DISP后结果存入temp1中
            App.matpool.Add("1", new Matrix(1, 1));
            App.matpool.Add("2", new Matrix(1, 1));
            App.matpool.Add("3", new Matrix(1, 1));
            Matrix.ExceptionHandling = ErrorHandle;
            InitializeComponent();
        }

        public void ErrorHandle(Exception ex)
        {
            if (errflag) err.Close();
            err = new(ex.Message);
            err.Show();
            errflag = true;
        }
        private void DragWindow(object sender, MouseButtonEventArgs e)
        {
            DragMove();
        }

        private void Close(object sender, RoutedEventArgs e)
        {
            Environment.Exit(0);
        }
        private void Envalue1(object sender, RoutedEventArgs e)
        {
            try
            {
                if (!STOflag)
                {
                    if (!EnvalueFlag)
                    {
                        tmp1.Background = System.Windows.Media.Brushes.Crimson;
                        Input input = new("1");
                        input.Show();
                        EnvalueFlag = true;
                    }
                }
                else
                {
                    if (Interface.Text.Length == 0)
                    {
                        throw new Exception("请输入矩阵符号");
                    }
                    else if (!IsValidMatrixName(Interface.Text))
                    {
                        throw new Exception("输入的式子并非矩阵符号，请检查");
                    }
                    else
                    {
                        STOflag = false;
                        sto.Background = new System.Windows.Media.SolidColorBrush(
                            (System.Windows.Media.Color)System.Windows.Media.ColorConverter.ConvertFromString("#FFF6DF07"));
                        App.matpool[Interface.Text] = App.matpool["1"];
                        log.Content = "已对矩阵" + Interface.Text + "赋值";
                    }
                }
            }
            catch (Exception ex)
            {
                ErrorHandle(ex);
            }
        }
        private void Envalue2(object sender, RoutedEventArgs e)
        {
            try
            {
                if (!STOflag)
                {
                    if (!EnvalueFlag)
                    {
                        tmp1.Background = System.Windows.Media.Brushes.Crimson;
                        Input input = new("2");
                        input.Show();
                        EnvalueFlag = true;
                    }
                }
                else
                {
                    if (Interface.Text.Length == 0)
                    {
                        throw new Exception("请输入矩阵符号");
                    }
                    else if (!IsValidMatrixName(Interface.Text))
                    {
                        throw new Exception("输入的式子并非矩阵符号，请检查");
                    }
                    else
                    {
                        STOflag = false;
                        sto.Background = new System.Windows.Media.SolidColorBrush(
                                (System.Windows.Media.Color)System.Windows.Media.ColorConverter.ConvertFromString("#FFF6DF07"));
                        App.matpool[Interface.Text] = App.matpool["2"];
                        log.Content = "已对矩阵" + Interface.Text + "赋值";
                    }
                }
            }
            catch (Exception ex)
            {
                ErrorHandle(ex);
            }
        }
        private void Envalue3(object sender, RoutedEventArgs e)
        {
            try
            {
                if (!STOflag)
                {
                    if (!EnvalueFlag)
                    {
                        tmp1.Background = System.Windows.Media.Brushes.Crimson;
                        Input input = new("3");
                        input.Show();
                        EnvalueFlag = true;
                    }
                }
                else
                {
                    if (Interface.Text.Length == 0)
                    {
                        throw new Exception("请输入矩阵符号");
                    }
                    else if (!IsValidMatrixName(Interface.Text))
                    {
                        throw new Exception("输入的式子并非矩阵符号，请检查");
                    }
                    else
                    {
                        STOflag = false;
                        sto.Background = new System.Windows.Media.SolidColorBrush(
                                (System.Windows.Media.Color)System.Windows.Media.ColorConverter.ConvertFromString("#FFF6DF07"));
                        App.matpool[Interface.Text] = App.matpool["3"];
                        log.Content = "已对矩阵" + Interface.Text + "赋值";
                    }
                }
            }
            catch (Exception ex)
            {
                ErrorHandle(ex);
            }
        }
        private void Minimize(object sender, RoutedEventArgs e)
        {
            WindowState = WindowState.Minimized;
        }
        private void Display1(object sender, MouseButtonEventArgs e)
        {
            try
            {
                Output output = new("1");
                output.Show();
            }
            catch (Exception ex)
            {
                ErrorHandle(ex);
            }
        }
        private void Display2(object sender, MouseButtonEventArgs e)
        {
            try
            {
                Output output = new("2");
                output.Show();
            }
            catch (Exception ex)
            {
                ErrorHandle(ex);
            }
        }
        private void Display3(object sender, MouseButtonEventArgs e)
        {
            try
            {
                Output output = new("3");
                output.Show();
            }
            catch (Exception ex)
            {
                ErrorHandle(ex);
            }
        }

        private void OpenStore(object sender, RoutedEventArgs e)
        {
            try
            {
                Store store = new();
                store.Show();
            }
            catch (Exception ex)
            {
                ErrorHandle(ex);
            }
        }

        private void QR(object sender, RoutedEventArgs e)
        {
            try
            {
                if (Interface.Text.Length == 0)
                {
                    throw new Exception("请输入矩阵符号");
                }
                else if (Interface.Text.Length >= 2)
                {
                    throw new Exception("输入的式子并非矩阵符号，请检查");
                }
                else if (App.matpool.ContainsKey(Interface.Text))
                {
                    Matrix matrix = App.matpool[Interface.Text];
                    matrix.QR(out Matrix temp1, out Matrix temp2);
                    App.matpool["1"] = temp1;
                    App.matpool["2"] = temp2;
                    log.Content = "已对" + Interface.Text + "进行QR分解";
                }
                else
                {
                    throw new Exception("输入的式子含有未被赋值的矩阵");
                }
            }
            catch (Exception ex)
            {
                if (!errflag)
                {
                    ErrorHandle(ex);
                }
                else err.Content = ex.Message;
            }
        }
        private void ELM(object sender, RoutedEventArgs e)
        {
            try
            {
                if (Interface.Text.Length == 0)
                {
                    throw new Exception("请输入矩阵符号");
                }
                else if (!IsValidMatrixName(Interface.Text))
                {
                    throw new Exception("输入的式子并非矩阵符号，请检查");
                }
                else if (App.matpool.ContainsKey(Interface.Text))
                {
                    App.matpool[Interface.Text].Gauss_Elimination();
                    log.Content = "已对" + Interface.Text + "进行高斯消元";
                }
                else
                {
                    throw new Exception("输入的式子含有未被赋值的矩阵");
                }
            }
            catch (Exception ex)
            {
                ErrorHandle(ex);
            }
        }
        private void SVD(object sender, RoutedEventArgs e)
        {
            try
            {
                if (Interface.Text.Length == 0)
                {
                    throw new Exception("请输入矩阵符号");
                }
                else if (!IsValidMatrixName(Interface.Text))
                {
                    throw new Exception("输入的式子并非矩阵符号，请检查");
                }
                else if (App.matpool.ContainsKey(Interface.Text))
                {
                    string key = Interface.Text;
                    Matrix matrix = App.matpool[key];
                    matrix.SVD(out Matrix temp1, out Matrix temp2, out Matrix temp3);
                    App.matpool["1"] = temp1;
                    App.matpool["2"] = temp2;
                    App.matpool["3"] = temp3;
                    log.Content = "已对" + Interface.Text + "进行SVD分解";
                }
                else
                {
                    throw new Exception("输入的式子含有未被赋值的矩阵");
                }
            }
            catch (Exception ex)
            {
                ErrorHandle(ex);
            }
        }
        private void STO(object sender, RoutedEventArgs e)
        {
            try
            {
                if (!STOflag)
                {
                    STOflag = true;
                    sto.Background = System.Windows.Media.Brushes.Crimson;
                }
                else
                {
                    STOflag = false;
                    sto.Background = new System.Windows.Media.SolidColorBrush(
                        (System.Windows.Media.Color)System.Windows.Media.ColorConverter.ConvertFromString("#FFF6DF07"));
                }
            }
            catch (Exception ex)
            {
                ErrorHandle(ex);
            }
        }
        private void TRAN(object sender, RoutedEventArgs e)
        {
            try
            {
                if (Interface.Text.Length == 0)
                {
                    throw new Exception("请输入矩阵符号");
                }
                else if (!IsValidMatrixName(Interface.Text))
                {
                    throw new Exception("输入的式子并非矩阵符号，请检查");
                }
                else if (App.matpool.ContainsKey(Interface.Text))
                {
                    string key = Interface.Text;
                    Matrix matrix = App.matpool[key];
                    Matrix temp1;
                    temp1 = matrix.Transpose();
                    App.matpool["1"] = temp1;
                    log.Content = "已对" + Interface.Text + "进行转置";
                }
                else
                {
                    throw new Exception("输入的式子含有未被赋值的矩阵");
                }
            }
            catch (Exception ex)
            {
                ErrorHandle(ex);
            }
        }
        private void INV(object sender, RoutedEventArgs e)
        {
            try
            {
                if (Interface.Text.Length == 0)
                {
                    throw new Exception("请输入矩阵符号");
                }
                else if (!IsValidMatrixName(Interface.Text))
                {
                    throw new Exception("输入的式子并非矩阵符号，请检查");
                }
                else if (App.matpool.ContainsKey(Interface.Text))
                {
                    string key = Interface.Text;
                    Matrix matrix = App.matpool[key];
                    matrix.InverseMatrix(out Matrix temp1);
                    App.matpool["1"] = temp1;
                    log.Content = "已对" + Interface.Text + "进行求逆";
                }
                else
                {
                    throw new Exception("输入的式子含有未被赋值的矩阵");
                }
            }
            catch (Exception ex)
            {
                ErrorHandle(ex);
            }
        }
        private void RANK(object sender, RoutedEventArgs e)
        {
            try
            {
                if (Interface.Text.Length == 0)
                {
                    throw new Exception("请输入矩阵符号");
                }
                else if (!IsValidMatrixName(Interface.Text))
                {
                    throw new Exception("输入的式子并非矩阵符号，请检查");
                }
                else if (App.matpool.ContainsKey(Interface.Text))
                {
                    string key = Interface.Text;
                    Matrix matrix = App.matpool[key];
                    int rank = matrix.GetRank();
                    log.Content = Interface.Text + "的秩是" + Convert.ToString(rank);
                }
                else
                {
                    throw new Exception("输入的式子含有未被赋值的矩阵");
                }
            }
            catch (Exception ex)
            {
                ErrorHandle(ex);
            }
        }
        private void DET(object sender, RoutedEventArgs e)
        {
            try
            {
                if (Interface.Text.Length == 0)
                {
                    throw new Exception("请输入矩阵符号");
                }
                else if (!IsValidMatrixName(Interface.Text))
                {
                    throw new Exception("输入的式子并非矩阵符号，请检查");
                }
                else if (App.matpool.ContainsKey(Interface.Text))
                {
                    string key = Interface.Text;
                    Matrix matrix = App.matpool[key];
                    double det = matrix.Determinant();
                    log.Content = Interface.Text + "的行列式是" + Convert.ToString(det);
                }
                else
                {
                    throw new Exception("输入的式子含有未被赋值的矩阵");
                }
            }
            catch (Exception ex)
            {
                ErrorHandle(ex);
            }
        }
        private void LU(object sender, RoutedEventArgs e)
        {
            try
            {
                if (Interface.Text.Length == 0)
                {
                    throw new Exception("请输入矩阵符号");
                }
                else if (!IsValidMatrixName(Interface.Text))
                {
                    throw new Exception("输入的式子并非矩阵符号，请检查");
                }
                else if (App.matpool.ContainsKey(Interface.Text))
                {
                    string key = Interface.Text;
                    Matrix matrix = App.matpool[key];
                    matrix.LU(out Matrix temp1, out Matrix temp2);
                    App.matpool["1"] = temp1;
                    App.matpool["2"] = temp2;
                    log.Content = "已对" + Interface.Text + "进行LU分解";
                }
                else
                {
                    throw new Exception("输入的式子含有未被赋值的矩阵");
                }
            }
            catch (Exception ex)
            {
                ErrorHandle(ex);
            }
        }
        private void GJ(object sender, RoutedEventArgs e)
        {
            try
            {
                if (Interface.Text.Length == 0)
                {
                    throw new Exception("请输入矩阵符号");
                }
                else if (!IsValidMatrixName(Interface.Text))
                {
                    throw new Exception("输入的式子并非矩阵符号，请检查");
                }
                else if (App.matpool.ContainsKey(Interface.Text))
                {
                    App.matpool[Interface.Text].Gauss_Jordan_Elimination();
                    log.Content = "已对" + Interface.Text + "G-J消元";
                }
                else
                {
                    throw new Exception("输入的式子含有未被赋值的矩阵");
                }
            }
            catch (Exception ex)
            {
                ErrorHandle(ex);
            }
        }
        private void RAD(object sender, RoutedEventArgs e)
        {
            try
            {
                if (Interface.Text.Length == 0)
                {
                    throw new Exception("请输入矩阵符号");
                }
                else if (!IsValidMatrixName(Interface.Text))
                {
                    throw new Exception("输入的式子并非矩阵符号，请检查");
                }
                else if (App.matpool.ContainsKey(Interface.Text))
                {
                    string key = Interface.Text;
                    Matrix matrix = App.matpool[key];
                    double rad = matrix.SpectralRadius();
                    log.Content = Interface.Text + "的谱半径是" + Convert.ToString(rad);
                }
                else
                {
                    throw new Exception("输入的式子含有未被赋值的矩阵");
                }
            }
            catch (Exception ex)
            {
                ErrorHandle(ex);
            }
        }
        private void Guide(object sender, RoutedEventArgs e)
        {
            try
            {
                _ = System.Diagnostics.Process.Start("C:\\Program Files (x86)\\Microsoft\\Edge\\Application\\msedge.exe", "https://github.com/TCL606/Matrix/tree/dev/MatrixCal");
            }
            catch (Exception ex)
            {
                ErrorHandle(ex);
            }
        }
        private void DISP(object sender, RoutedEventArgs e)
        {
            try
            {
                if (Interface.Text.Length == 0)
                {
                    throw new Exception("请输入矩阵符号");
                }
                formula = InputProcessing.Infix2Suffix(Interface.Text);
                var arr = formula.Split(' ');
                for (int i = 0; i < arr.Length - 1; i++)
                {
                    if (arr[i] != "+" && arr[i] != "-" && arr[i] != "*" && arr[i] != "^")//不是运算符
                    {
                        stack.Push(arr[i]);
                        if (IsValidMatrixName(arr[i]))
                            App.temppool[arr[i]] = App.matpool[arr[i]];
                    }
                    else//是运算符
                    {
                        CalculateAndPush(arr[i]);
                    }
                }
                if (stack.Count > 1)
                    throw new Exception("表达式语法有误");
                if (IsValidMatrixName(stack.Peek()))
                {
                    App.matpool["1"] = App.temppool[stack.Pop()];//结果存在temp1中
                    Output output = new("1");
                    output.Show();
                    log.Content = "计算完成";
                }
                else log.Content = Convert.ToDouble(stack.Pop());
                App.temppool.Clear();
            }
            catch (Exception ex)
            {
                ErrorHandle(ex);
            }
        }
        private void EGV(object sender, RoutedEventArgs e)
        {
            try
            {
                if (Interface.Text.Length == 0)
                {
                    throw new Exception("请输入矩阵符号");
                }
                else if (!IsValidMatrixName(Interface.Text))
                {
                    throw new Exception("输入的式子并非矩阵符号，请检查");
                }
                else if (App.matpool.ContainsKey(Interface.Text))
                {
                    string key = Interface.Text;
                    Matrix matrix = App.matpool[key];
                    var EigenValues = matrix.GetAllEigenValues();
                    double[,] ReEigenValues = new double[1, EigenValues.Count];
                    double[,] ImEigenValues = new double[1, EigenValues.Count];
                    for (int i = 0; i < EigenValues.Count; i++)
                    {
                        ReEigenValues[0, i] = EigenValues[i].real;
                        ImEigenValues[0, i] = EigenValues[i].imag;
                    }
                    Matrix Remat = new(ReEigenValues);
                    Matrix Immat = new(ImEigenValues);
                    App.matpool["1"] = Remat;
                    App.matpool["2"] = Immat;
                    log.Content = "已对" + Interface.Text + "求特征值";
                }
                else
                {
                    throw new Exception("输入的式子含有未被赋值的矩阵");
                }
            }
            catch (Exception ex)
            {
                ErrorHandle(ex);
            }
        }
        private void RSV(object sender, RoutedEventArgs e)
        {
            try
            {
                if (Interface.Text.Length == 0)
                {
                    throw new Exception("请输入矩阵符号");
                }
                else if (!IsValidMatrixName(Interface.Text))
                {
                    throw new Exception("输入的式子并非矩阵符号，请检查");
                }
                else if (App.matpool.ContainsKey(Interface.Text))
                {
                    string key = Interface.Text;
                    Matrix matrix = App.matpool[key];
                    matrix.GetAllSolutionsForAugmentedMatrix(out Matrix temp1);
                    App.matpool["1"] = temp1;
                    log.Content = "已对增广矩阵" + Interface.Text + "求解";
                }
                else
                {
                    throw new Exception("输入的式子含有未被赋值的矩阵");
                }
            }
            catch (Exception ex)
            {
                ErrorHandle(ex);
            }
        }
        private void EGC(object sender, RoutedEventArgs e)
        {
            try
            {
                if (Interface.Text.Length == 0)
                {
                    throw new Exception("请输入矩阵符号");
                }
                else if (!IsValidMatrixName(Interface.Text))
                {
                    throw new Exception("输入的式子并非矩阵符号，请检查");
                }
                else if (App.matpool.ContainsKey(Interface.Text))
                {
                    string key = Interface.Text;
                    Matrix matrix = App.matpool[key];
                    var Coefficients = matrix.GetCoefficientsOfCharacteristicPolynomial();
                    double[,] coefficients = new double[1, Coefficients.Count];
                    for (int i = 0; i < Coefficients.Count; i++)
                    {
                        coefficients[0, i] = Coefficients[i];
                    }
                    Matrix comat = new(coefficients);
                    App.matpool["1"] = comat;
                    log.Content = "已对" + Interface.Text + "求特征多项式系数";
                }
                else
                {
                    throw new Exception("输入的式子含有未被赋值的矩阵");
                }
            }
            catch (Exception ex)
            {
                ErrorHandle(ex);
            }
        }
        private void GINV(object sender, RoutedEventArgs e)
        {
            try
            {
                if (Interface.Text.Length == 0)
                {
                    throw new Exception("请输入矩阵符号");
                }
                else if (!IsValidMatrixName(Interface.Text))
                {
                    throw new Exception("输入的式子并非矩阵符号，请检查");
                }
                else if (App.matpool.ContainsKey(Interface.Text))
                {
                    string key = Interface.Text;
                    Matrix matrix = App.matpool[key];
                    Matrix temp1;
                    throw new Exception("广义逆正在开发");
                    App.matpool["1"] = temp1;
                    log.Content = "已对" + Interface.Text + "求广义逆";
                }
                else
                {
                    throw new Exception("输入的式子含有未被赋值的矩阵");
                }
            }
            catch (Exception ex)
            {
                ErrorHandle(ex);
            }
        }
        private void ZSP(object sender, RoutedEventArgs e)
        {
            try
            {
                if (Interface.Text.Length == 0)
                {
                    throw new Exception("请输入矩阵符号");
                }
                else if (!IsValidMatrixName(Interface.Text))
                {
                    throw new Exception("输入的式子并非矩阵符号，请检查");
                }
                else if (App.matpool.ContainsKey(Interface.Text))
                {
                    string key = Interface.Text;
                    Matrix matrix = App.matpool[key];
                    matrix.GetBasesOfNullSpace(out Matrix temp1);
                    App.matpool["1"] = temp1;
                    log.Content = "已对" + Interface.Text + "求零空间";
                }
                else
                {
                    throw new Exception("输入的式子含有未被赋值的矩阵");
                }
            }
            catch (Exception ex)
            {
                ErrorHandle(ex);
            }
        }
        private bool IsValidMatrixName(string str)
        {
            if (str.Length > 1) return false;
            else
            {
                if (str[0] >= 'A' && str[0] <= 'Z')
                    return true;
                return false;
            }
        }
        private void CalculateAndPush(string s3)
        {
            string s1, s2;
            if (stack.Count >= 2)
            {
                s2 = stack.Pop();
                s1 = stack.Pop();
            }
            else throw new Exception("表达式操作数个数有误");
            if (IsValidMatrixName(s1) && IsValidMatrixName(s2))//s1，s2表示矩阵
            {
                if (!(App.matpool.ContainsKey(s1) && App.matpool.ContainsKey(s2)))
                {
                    throw new Exception("输入的式子含有未被赋值的矩阵");
                }
                else
                {
                    switch (s3)
                    {
                        case "+":
                            App.temppool[s1] += App.temppool[s2];
                            stack.Push(s1);
                            break;
                        case "-":
                            App.temppool[s1] -= App.temppool[s2];
                            stack.Push(s1);
                            break;
                        case "*":
                            App.temppool[s1] *= App.temppool[s2];
                            stack.Push(s1);
                            break;
                        case "^":
                            throw new Exception("矩阵指数幂运算尚未定义，敬请期待");
                    }
                }
            }
            else if (IsValidMatrixName(s1))//s1为矩阵，s2为数
            {
                if (!App.matpool.ContainsKey(s1))
                {
                    throw new Exception("输入的式子含有未被赋值的矩阵");
                }
                else
                {
                    switch (s3)
                    {
                        case "+":
                        case "-":
                            throw new Exception("矩阵不能和数相加减");
                        case "*":
                            App.temppool[s1] = Convert.ToDouble(s2) * App.temppool[s1];
                            stack.Push(s1);
                            break;
                        case "^":
                            App.temppool[s1] = App.temppool[s1].Power(Convert.ToInt32(s2));
                            stack.Push(s1);
                            break;
                    }
                }
            }
            else if (IsValidMatrixName(s2))//s2为矩阵，s1为数
            {
                if (!App.matpool.ContainsKey(s2))
                {
                    throw new Exception("输入的式子含有未被赋值的矩阵");
                }
                else
                {
                    switch (s3)
                    {
                        case "+":
                        case "-":
                            throw new Exception("矩阵不能和数相加减");
                        case "*":
                            App.temppool[s2] = Convert.ToDouble(s1) * App.temppool[s2];
                            stack.Push(s2);
                            break;
                        case "^":
                            throw new Exception("矩阵指数幂运算尚未定义，敬请期待");
                    }
                }
            }
            else//全是数
            {
                switch (s3)
                {
                    case "+":
                        stack.Push(Convert.ToString(Convert.ToDouble(s1) + Convert.ToDouble(s2)));
                        break;
                    case "-":
                        stack.Push(Convert.ToString(Convert.ToDouble(s1) - Convert.ToDouble(s2)));
                        break;
                    case "*":
                        stack.Push(Convert.ToString(Convert.ToDouble(s1) * Convert.ToDouble(s2)));
                        break;
                    case "^":
                        stack.Push(Convert.ToString(Math.Pow(Convert.ToDouble(s1), Convert.ToDouble(s2))));
                        break;
                }
            }
            //这里的思路说明：字符串处理并压入stack中后，正常应该遇到字母或数字则压进栈，遇到算符出栈，再放进二者的和（或差，积...）。然而
            //由于stack存储的只是矩阵的名字，矩阵的值存储在字典matpool中，和也必须有一个名字，且名字必须是大写字母。这时会想到，
            //如果对栈顶的A和B（或A和一个数字）进行操作，则把和的值在matpool中和A对应起来，把A再压进栈就行了。然而如果这样则会导致A的值被改变，
            //而这是不允许的。鉴于不能改动matpool中A对应的值，我们需要在把矩阵的名字A压进栈时，就在temppool中创造一个键值对副本，它的键是A，
            //值是A在matpool中的值。这样，之后所有操作均可以第三行提到的方法在temppool中进行。计算完毕后temppool中应还剩一个键值对，把
            //这个值赋给temp1（这行代码见571行），最后将temppool清空即可。此外程序可能有些地方忘记new()，如果出现bug请考虑这个可能。
        }

        public ErrorDisplayer err;
        private Stack<string> stack;
        public bool errflag;
        private string? formula;
        private bool STOflag;
        public bool EnvalueFlag;
    }
}
//更新：现在同一时间只有一个错误栏；增加输出日志；增加一些错误处理；
