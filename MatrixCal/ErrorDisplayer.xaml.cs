﻿using System;
using System.Windows;
using System.Windows.Input;
using System.Linq;
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
            var _mainWindow = Application.Current.Windows
            .Cast<Window>()
            .FirstOrDefault(window => window is MainWindow) as MainWindow;
            _mainWindow.errflag = false;
            Close();
        }
        private void DragWindow(object sender, MouseButtonEventArgs e)
        {
            DragMove();
        }
    }
}