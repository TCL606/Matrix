using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;

namespace MatrixCal
{
    /// <summary>
    /// Store.xaml 的交互逻辑
    /// </summary>
    public partial class Store : Window
    {
        const int ButtonHeight = 15;
        const int ButtonWidth = 30;
        const int VerticalInterval = 3;
        const int HorizontalInterval = 5;
        public Store()
        {
            App.storeFlag = true;
            InitializeComponent();
            Construct();
        }
        private void Construct()
        {
            StoreGrid.Children.Clear();
            int row = Convert.ToInt32(Math.Sqrt(App.matpool.Count - 4));
            int col = row + 1;
            Height = (ButtonHeight + VerticalInterval) * row + 30;
            Width = (ButtonWidth + HorizontalInterval) * col + 54;
            Button[] btns = new Button[App.matpool.Count - 4];//这里测试之后需要-4
            int i = 0;
            foreach (string s in App.matpool.Keys)
            {
                if (s != "0" && s != "1" && s != "2" && s != "3")
                {
                    btns[i] = new Button();
                    StoreGrid.Children.Add(btns[i]);
                    btns[i].Content = s;
                    btns[i].HorizontalAlignment = HorizontalAlignment.Left;
                    btns[i].VerticalAlignment = VerticalAlignment.Top;
                    btns[i].Height = ButtonHeight;
                    btns[i].Width = ButtonWidth;
                    btns[i].Opacity = 0.8;
                    btns[i].BorderBrush = new SolidColorBrush((Color)ColorConverter.ConvertFromString("#FF20D4E2"));
                    btns[i].Background = new SolidColorBrush((Color)ColorConverter.ConvertFromString("#FF20D4E2"));
                    btns[i].Foreground = new SolidColorBrush((Color)ColorConverter.ConvertFromString("#FFDCD6D6"));
                    btns[i].Margin = new Thickness(26 + (ButtonWidth + HorizontalInterval) * (i % col),
                                                           15 + (ButtonHeight + VerticalInterval) * (i / col),
                                                           0,
                                                           0);
                    btns[i].Click += new RoutedEventHandler(Display);
                    btns[i].MouseRightButtonDown += new MouseButtonEventHandler(Delete);
                    i++;
                }
            }
        }
        private void DragWindow(object sender, MouseButtonEventArgs e)
        {
            DragMove();
        }
        private void Display(object sender, RoutedEventArgs e)
        {
            Button? button = sender as Button;
            Output output = new(Convert.ToString(button.Content));
            output.Show();
        }
        private void Delete(object sender, MouseButtonEventArgs e)
        {
            Button? button = sender as Button;
            App.matpool.Remove(Convert.ToString(button.Content));
            Construct();
        }
            private void StoreClose(object sender,RoutedEventArgs e)
        {
            App.storeFlag = false;
            Close();
        }
    }
}
