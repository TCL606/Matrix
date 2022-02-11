using System;
using System.Windows;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Controls;
namespace MatrixCal
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            EnvalueFlag = false;
            STOflag = false;
            App.matpool = new();
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
                STOflag = false;
                App.matpool[Interface.Text] = App.matpool["1"];
            }
        }
        private void Envalue2(object sender, RoutedEventArgs e)
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
                STOflag = false;
                App.matpool[Interface.Text] = App.matpool["2"];
            }
        }
        private void Envalue3(object sender, RoutedEventArgs e)
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
                STOflag = false;
                App.matpool[Interface.Text] = App.matpool["3"];
                //此处应有操作反馈
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

        private void StringProcess(object sender, TextChangedEventArgs e)
        {
            try
            {
                
            }
            catch (Exception ex)
            {
                ErrorDisplayer err = new(ex.Message);
                err.Show();
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
                else if(Interface.Text.Length >=2)
                {
                    throw new Exception("本功能只支持对单个矩阵操作");
                }
                else
                {
                    string key=Interface.Text;
                    TCL_Matrix.Matrix matrix = App.matpool[key];
                    TCL_Matrix.Matrix temp1, temp2;
                    matrix.QR(out temp1, out temp2);
                    App.matpool["1"] = temp1;
                    App.matpool["2"] = temp2;
                    //此处应有操作反馈
                }
            }
            catch (Exception ex)
            {
                ErrorDisplayer err = new(ex.Message);
                err.Show();
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
                    throw new Exception("本功能只支持对单个矩阵操作");
                }
                else
                {
                    string key = Interface.Text;
                    TCL_Matrix.Matrix matrix = App.matpool[key];
                    matrix.Gauss_Elimination();
                    //此处应有操作反馈
                }
            }
            catch (Exception ex)
            {
                ErrorDisplayer err = new(ex.Message);
                err.Show();
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
                    throw new Exception("本功能只支持对单个矩阵操作");
                }
                else
                {
                    string key = Interface.Text;
                    TCL_Matrix.Matrix matrix = App.matpool[key];
                    TCL_Matrix.Matrix temp1, temp2,temp3;
                    matrix.SVD(out temp1, out temp2,out temp3);
                    App.matpool["1"] = temp1;
                    App.matpool["2"] = temp2;
                    App.matpool["3"] = temp3;   
                    //此处应有操作反馈
                }
            }
            catch (Exception ex)
            {
                ErrorDisplayer err = new(ex.Message);
                err.Show();
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
                    throw new Exception("本功能只支持对单个矩阵操作");
                }
                else
                {
                    string key = Interface.Text;
                    TCL_Matrix.Matrix matrix = App.matpool[key];
                    TCL_Matrix.Matrix temp1;
                    temp1=matrix.Transpose();
                    App.matpool["1"] = temp1;
                    //此处应有操作反馈
                }
            }
            catch (Exception ex)
            {
                ErrorDisplayer err = new(ex.Message);
                err.Show();
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
                    throw new Exception("本功能只支持对单个矩阵操作");
                }
                else
                {
                    string key = Interface.Text;
                    TCL_Matrix.Matrix matrix = App.matpool[key];
                    TCL_Matrix.Matrix temp1;
                    matrix.InverseMatrix(out temp1);
                    App.matpool["1"] = temp1;
                    //此处应有操作反馈
                }
            }
            catch (Exception ex)
            {
                ErrorDisplayer err = new(ex.Message);
                err.Show();
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
                    throw new Exception("本功能只支持对单个矩阵操作");
                }
                else
                {
                    string key = Interface.Text;
                    TCL_Matrix.Matrix matrix = App.matpool[key];
                    int rank = matrix.GetRank();
                    //操作反馈:在反馈栏直接显示rank
                }
            }
            catch (Exception ex)
            {
                ErrorDisplayer err = new(ex.Message);
                err.Show();
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
                    throw new Exception("本功能只支持对单个矩阵操作");
                }
                else
                {
                    string key = Interface.Text;
                    TCL_Matrix.Matrix matrix = App.matpool[key];
                    double det=matrix.Determinant();
                    //操作反馈:在反馈栏直接显示det
                }
            }
            catch (Exception ex)
            {
                ErrorDisplayer err = new(ex.Message);
                err.Show();
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
                    throw new Exception("本功能只支持对单个矩阵操作");
                }
                else
                {
                    string key = Interface.Text;
                    TCL_Matrix.Matrix matrix = App.matpool[key];
                    TCL_Matrix.Matrix temp1, temp2;
                    matrix.LU(out temp1, out temp2);
                    App.matpool["1"] = temp1;
                    App.matpool["2"] = temp2;
                    //此处应有操作反馈
                }
            }
            catch (Exception ex)
            {
                ErrorDisplayer err = new(ex.Message);
                err.Show();
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
                    throw new Exception("本功能只支持对单个矩阵操作");
                }
                else
                {
                    string key = Interface.Text;
                    TCL_Matrix.Matrix matrix = App.matpool[key];
                    matrix.Gauss_Jordan_Elimination();
                    //此处应有操作反馈
                }
            }
            catch (Exception ex)
            {
                ErrorDisplayer err = new(ex.Message);
                err.Show();
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
                else
                {
                    string key = Interface.Text;
                    TCL_Matrix.Matrix matrix = App.matpool[key];
                    double rad = matrix.SpectralRadius();
                    //操作反馈:在反馈栏直接显示rad
                }
            }
            catch (Exception ex)
            {
                ErrorDisplayer err = new(ex.Message);
                err.Show();
            }
        }
        private void Others(object sender, RoutedEventArgs e)
        {
            //功能待更新
        }
        private void DISP(object sender, RoutedEventArgs e)
        {
            Calculate(formula);
        }
        private void Calculate(string s)//输入字符串，返回对应的结果
        {
            //计算结果存到matpool["1"]中,但同时也是matpool["0"]
            Output output = new("0");
            output.Show();
        }
        private string formula;
        private bool STOflag;
        public bool EnvalueFlag;
    }
}
