using System;
using System.Windows;
using System.Windows.Input;

namespace MatrixCal
{
    /// <summary>
    /// ErrorDisplayer.xaml 的交互逻辑
    /// </summary>
    public partial class ErrorDisplayer : Window
    {
        public ErrorDisplayer(string s)
        {
            InitializeComponent();
            errorDisplayer.Text = s + " Time:" + DateTime.Now.ToString();
        }
        private void ErrorClose(object sender,RoutedEventArgs e)
        {   
            Close();
        }
        private void DragWindow(object sender, MouseButtonEventArgs e)
        {
            DragMove();
        }
    }
}
