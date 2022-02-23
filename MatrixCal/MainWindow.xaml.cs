﻿using System;
using System.Windows;
using System.Windows.Input;
using System.Windows.Media;
using StringProcessing;
using System.Collections.Generic;

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
            App.matpool.Add("0", new TCL_Matrix.Matrix(1, 1));//字符0对应输入算式的暂时结果；DISP后结果存入temp1中
            App.matpool.Add("1", new TCL_Matrix.Matrix(1, 1));
            App.matpool.Add("2", new TCL_Matrix.Matrix(1, 1));
            App.matpool.Add("3", new TCL_Matrix.Matrix(1, 1));
            InitializeComponent();
        }

        private void DragWindow(object sender, MouseButtonEventArgs e)
        {
            DragMove();
        }

        private void Close(object sender, RoutedEventArgs e)
        {
            Application.Current.Shutdown(); 
        }
        private void Envalue1(object sender, RoutedEventArgs e)
        {
            try
            {
                if (!STOflag)
                {
                    if (!EnvalueFlag)
                    {
                        EnvalueFlag = true;
                        tmp1.Background = Brushes.Crimson;
                        Input input = new("1");
                        input.Show();
                    }
                }
                else
                {
                    if (Interface.Text.Length == 0)
                    {
                        throw new Exception("请输入矩阵符号");
                    }
                    else if (Interface.Text.Length >= 2)
                    {
                        throw new Exception("本功能只支持对单个矩阵操作");
                    }
                    else
                    {
                        STOflag = false;
                        App.matpool[Interface.Text] = App.matpool["1"];
                        log.Content = "已对矩阵" + Interface.Text + "赋值";
                    }
                }
            }
            catch (Exception ex)
            {
                err.errorDisplayer.Text = ex.Message;
                if (!errflag)
                {
                    err.Show();
                    errflag = true;
                }
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
                        EnvalueFlag = true;
                        tmp1.Background = Brushes.Crimson;
                        Input input = new("2");
                        input.Show();
                    }
                }
                else
                {
                    if (Interface.Text.Length == 0)
                    {
                        throw new Exception("请输入矩阵符号");
                    }
                    else if (Interface.Text.Length >= 2)
                    {
                        throw new Exception("本功能只支持对单个矩阵操作");
                    }
                    else
                    {
                        STOflag = false;
                        App.matpool[Interface.Text] = App.matpool["2"];
                        log.Content = "已对矩阵" + Interface.Text + "赋值";
                    }
                }
            }
            catch (Exception ex)
            {
                err.errorDisplayer.Text = ex.Message;
                if (!errflag)
                {
                    err.Show();
                    errflag = true;
                }
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
                        EnvalueFlag = true;
                        tmp1.Background = Brushes.Crimson;
                        Input input = new("3");
                        input.Show();
                    }
                }
                else
                {
                    if (Interface.Text.Length == 0)
                    {
                        throw new Exception("请输入矩阵符号");
                    }
                    else if (Interface.Text.Length >= 2)
                    {
                        throw new Exception("本功能只支持对单个矩阵操作");
                    }
                    else
                    {
                        STOflag = false;
                        App.matpool[Interface.Text] = App.matpool["3"];
                        log.Content = "已对矩阵" + Interface.Text + "赋值";
                    }
                }
            }
            catch (Exception ex)
            {
                err.errorDisplayer.Text = ex.Message;
                if (!errflag)
                {
                    err.Show();
                    errflag = true;
                }
            }
        }
        private void Minimize(object sender, RoutedEventArgs e)
        {
            WindowState=WindowState.Minimized;  
        }
        private void Display1(object sender, MouseButtonEventArgs e)
        {
            Output output=new("1");
            output.Show();
        }
        private void Display2(object sender, MouseButtonEventArgs e)
        {
            Output output = new("2");
            output.Show();
        }
        private void Display3(object sender, MouseButtonEventArgs e)
        {
            Output output = new("3");
            output.Show();
        }

        private void OpenStore(object sender, RoutedEventArgs e)
        {
            Store store = new();
            store.Show();
        }

        private void QR(object sender, RoutedEventArgs e)
        {
            try
            {
                if (Interface.Text.Length == 0)
                {
                    throw new Exception("请输入矩阵符号");
                }
                else if(Interface.Text.Length >=2)
                {
                    throw new Exception("输入的式子并非矩阵符号，请检查。");
                }
                else if(App.matpool.ContainsKey(Interface.Text))
                {
                    TCL_Matrix.Matrix matrix = App.matpool[Interface.Text];
                    matrix.QR(out TCL_Matrix.Matrix temp1, out TCL_Matrix.Matrix temp2);
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
                err.errorDisplayer.Text = ex.Message;
                if(!errflag)
                {
                    err.Show();
                    errflag = true;
                }    
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
                else if (Interface.Text.Length >= 2)
                {
                    throw new Exception("输入的式子并非矩阵符号，请检查。");
                }
                else if (App.matpool.ContainsKey(Interface.Text))
                {
                    string key = Interface.Text;
                    TCL_Matrix.Matrix matrix = App.matpool[key];
                    matrix.Gauss_Elimination();
                    log.Content = "已对" + Interface.Text + "进行高斯消元";
                }
                else
                {
                    throw new Exception("输入的式子含有未被赋值的矩阵");
                }
            }
            catch (Exception ex)
            {
                err.errorDisplayer.Text = ex.Message;
                if (!errflag)
                {
                    err.Show();
                    errflag = true;
                }
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
                else if (Interface.Text.Length >= 2)
                {
                    throw new Exception("输入的式子并非矩阵符号，请检查。");
                }
                else if (App.matpool.ContainsKey(Interface.Text))
                {
                    string key = Interface.Text;
                    TCL_Matrix.Matrix matrix = App.matpool[key];
                    matrix.SVD(out TCL_Matrix.Matrix temp1, out TCL_Matrix.Matrix temp2, out TCL_Matrix.Matrix temp3);
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
                err.errorDisplayer.Text = ex.Message;
                if (!errflag)
                {
                    err.Show();
                    errflag = true;
                }
            }
        }
        private void STO(object sender, RoutedEventArgs e)
        {
            STOflag = true;
        }
        private void TRAN(object sender, RoutedEventArgs e)
        {
            try
            {
                if (Interface.Text.Length == 0)
                {
                    throw new Exception("请输入矩阵符号");
                }
                else if (Interface.Text.Length >= 2)
                {
                    throw new Exception("输入的式子并非矩阵符号，请检查。");
                }
                else if (App.matpool.ContainsKey(Interface.Text))
                {
                    string key = Interface.Text;
                    TCL_Matrix.Matrix matrix = App.matpool[key];
                    TCL_Matrix.Matrix temp1;
                    temp1=matrix.Transpose();
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
                err.errorDisplayer.Text = ex.Message;
                if (!errflag)
                {
                    err.Show();
                    errflag = true;
                }
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
                else if (Interface.Text.Length >= 2)
                {
                    throw new Exception("输入的式子并非矩阵符号，请检查。");
                }
                else if (App.matpool.ContainsKey(Interface.Text))
                {
                    string key = Interface.Text;
                    TCL_Matrix.Matrix matrix = App.matpool[key];
                    matrix.InverseMatrix(out TCL_Matrix.Matrix temp1);
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
                err.errorDisplayer.Text = ex.Message;
                if (!errflag)
                {
                    err.Show();
                    errflag = true;
                }
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
                else if (Interface.Text.Length >= 2)
                {
                    throw new Exception("输入的式子并非矩阵符号，请检查。");
                }
                else if (App.matpool.ContainsKey(Interface.Text))
                {
                    string key = Interface.Text;
                    TCL_Matrix.Matrix matrix = App.matpool[key];
                    int rank = matrix.GetRank();
                    log.Content = Interface.Text+"的秩是"+Convert.ToString(rank);
                }
                else
                {
                    throw new Exception("输入的式子含有未被赋值的矩阵");
                }
            }
            catch (Exception ex)
            {
                err.errorDisplayer.Text = ex.Message;
                if (!errflag)
                {
                    err.Show();
                    errflag = true;
                }
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
                else if (Interface.Text.Length >= 2)
                {
                    throw new Exception("输入的式子并非矩阵符号，请检查。");
                }
                else if (App.matpool.ContainsKey(Interface.Text))
                {
                    string key = Interface.Text;
                    TCL_Matrix.Matrix matrix = App.matpool[key];
                    double det=matrix.Determinant();
                    log.Content = Interface.Text + "的秩是" + Convert.ToString(det);
                }
                else
                {
                    throw new Exception("输入的式子含有未被赋值的矩阵");
                }
            }
            catch (Exception ex)
            {
                err.errorDisplayer.Text = ex.Message;
                if (!errflag)
                {
                    err.Show();
                    errflag = true;
                }
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
                else if (Interface.Text.Length >= 2)
                {
                    throw new Exception("输入的式子并非矩阵符号，请检查。");
                }
                else if (App.matpool.ContainsKey(Interface.Text))
                {
                    string key = Interface.Text;
                    TCL_Matrix.Matrix matrix = App.matpool[key];
                    matrix.LU(out TCL_Matrix.Matrix temp1, out TCL_Matrix.Matrix temp2);
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
                err.errorDisplayer.Text = ex.Message;
                if (!errflag)
                {
                    err.Show();
                    errflag = true;
                }
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
                else if (Interface.Text.Length >= 2)
                {
                    throw new Exception("输入的式子并非矩阵符号，请检查。");
                }
                else if (App.matpool.ContainsKey(Interface.Text))
                {
                    string key = Interface.Text;
                    TCL_Matrix.Matrix matrix = App.matpool[key];
                    matrix.Gauss_Jordan_Elimination();
                    log.Content = "已对" + Interface.Text + "进行高斯-约旦消元";
                }
                else
                {
                    throw new Exception("输入的式子含有未被赋值的矩阵");
                }
            }
            catch (Exception ex)
            {
                err.errorDisplayer.Text = ex.Message;
                if (!errflag)
                {
                    err.Show();
                    errflag = true;
                }
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
                else if (Interface.Text.Length >= 2)
                {
                    throw new Exception("本功能只支持对单个矩阵操作");
                }
                else if (App.matpool.ContainsKey(Interface.Text))
                {
                    string key = Interface.Text;
                    TCL_Matrix.Matrix matrix = App.matpool[key];
                    double rad = matrix.SpectralRadius();
                    log.Content = Interface.Text + "的秩是" + Convert.ToString(rad);
                }
                else
                {
                    throw new Exception("输入的式子含有未被赋值的矩阵");
                }
            }
            catch (Exception ex)
            {
                err.errorDisplayer.Text = ex.Message;
                if (!errflag)
                {
                    err.Show();
                    errflag = true;
                }
            }
        }
        private void Others(object sender, RoutedEventArgs e)
        {
            err.errorDisplayer.Text = "更多算法请等待更新！";
            if (!errflag)
            {
                err.Show();
                errflag = true;
            }
        }
        private void DISP(object sender, RoutedEventArgs e)
        {
            try
            {
                formula = InputProcessing.Infix2Suffix(Interface.Text);
                var arr = formula.Split(' ');
                TCL_Matrix.Matrix temp;
                for (int i = 0; i < arr.Length; i++)
                {
                    if (arr[i] != "+" && arr[i] != "-" && arr[i] != "*" && arr[i] != "^")//不是运算符
                    {
                        stack.Push(arr[i]);
                    }
                    else//是运算符
                    {
                        CalculateAndPush(arr[i]);
                    }
                }
                App.matpool["1"] = App.temppool[stack.Pop()];//结果存在temp1中
                Output output = new("1");
                output.Show();
            }
            catch (Exception ex)
            {
                err.errorDisplayer.Text = ex.Message;
                if (!errflag)
                {
                    err.Show();
                    errflag = true;
                }
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
            if((s1.Length==1&& Convert.ToInt32(s1) >= 65 && Convert.ToInt32(s1) <= 90)&&
                (s2.Length == 1 && Convert.ToInt32(s2) >= 65 && Convert.ToInt32(s2) <= 90))//s1，s2表示矩阵
            {
                if(!(App.matpool.ContainsKey(s1)&& App.matpool.ContainsKey(s2)))
                {
                    throw new Exception("输入的式子含有未被赋值的矩阵");
                }
                else
                {

                }
            }
            else if (s1.Length == 1 && Convert.ToInt32(s1) >= 65 && Convert.ToInt32(s1) <= 90)//s1为矩阵，s2为数
            {
                if (!App.matpool.ContainsKey(s1))
                {
                    throw new Exception("输入的式子含有未被赋值的矩阵");
                }
                else
                {

                }
            }
            else if (s2.Length == 1 && Convert.ToInt32(s2) >= 65 && Convert.ToInt32(s2) <= 90)//s2为矩阵，s1为数
            {
                if (!App.matpool.ContainsKey(s2))
                {
                    throw new Exception("输入的式子含有未被赋值的矩阵");
                }
                else
                {

                }
            }
            else//全是数
            {
                switch(s3)
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
                        stack.Push(Convert.ToString(Math.Pow(Convert.ToDouble(s1),Convert.ToDouble(s2))));
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