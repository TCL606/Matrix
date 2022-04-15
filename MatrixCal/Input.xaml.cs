using System;
using System.Linq;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;
namespace MatrixCal
{
    /// <summary>
    /// Input.xaml 的交互逻辑
    /// </summary>
    public partial class Input : Window
    {
        const int TextBoxHeight = 17;
        const int TextBoxWidth = 48;
        const int VerticalInterval = 3;
        const int HorizontalInterval = 5;
        MatrixCal.MainWindow _mainWindow = Application.Current.Windows
             .Cast<Window>()
             .FirstOrDefault(window => window is MainWindow) as MainWindow;
        public Input(string s)
        {
            flag = false;
            key = s;
            InitializeComponent();
            Key.Text = key;
        }
        private void DragWindow(object sender, MouseButtonEventArgs e)
        {
            DragMove();
        }
        private void InputClose(object sender, RoutedEventArgs e)
        {
            _mainWindow.tmp1.Background = new SolidColorBrush((Color)ColorConverter.ConvertFromString("#FF0078D7"));
            _mainWindow.tmp2.Background = new SolidColorBrush((Color)ColorConverter.ConvertFromString("#FF0078D7"));
            _mainWindow.tmp3.Background = new SolidColorBrush((Color)ColorConverter.ConvertFromString("#FF0078D7"));
            _mainWindow.EnvalueFlag = false;
            Close();
        }
        private void InputConfirm(object sender, RoutedEventArgs e)
        {
            if (!flag)
            {
                try
                {
                    if (Row.Text.Length == 0 || Col.Text.Length == 0)
                    {
                        throw new Exception("请填写完整的信息");
                    }
                    else if (Convert.ToInt32(Col.Text) > 27 || Convert.ToInt32(Row.Text) > 27)
                    {
                        throw new Exception("受屏幕尺寸限制，不支持27行/列以上的输入");
                    }
                    else
                    {
                        Key.IsReadOnly = true;
                        Col.IsReadOnly = Row.IsReadOnly = true;
                        flag = true;
                        int col = Convert.ToInt32(Col.Text);
                        int row = Convert.ToInt32(Row.Text);
                        Height += (TextBoxHeight + VerticalInterval) * row;
                        Width = (TextBoxWidth + HorizontalInterval) * col + 54 > Width ?
                                (TextBoxWidth + HorizontalInterval) * col + 54 : Width;
                        textBoxes = new TextBox[row, col];
                        for (int i = 0; i < row; i++)
                            for (int j = 0; j < col; j++)
                            {
                                textBoxes[i, j] = new();
                                InputGrid.Children.Add(textBoxes[i, j]);
                                textBoxes[i, j].HorizontalAlignment = HorizontalAlignment.Left;
                                textBoxes[i, j].VerticalAlignment = VerticalAlignment.Top;
                                textBoxes[i, j].Height = TextBoxHeight;
                                textBoxes[i, j].Width = TextBoxWidth;
                                textBoxes[i, j].Opacity = 0.8;
                                textBoxes[i, j].BorderBrush = new SolidColorBrush((Color)ColorConverter.ConvertFromString("#FFABABAB"));
                                textBoxes[i, j].Background = new SolidColorBrush((Color)ColorConverter.ConvertFromString("#FFABABAB"));
                                textBoxes[i, j].Foreground = Brushes.White;
                                textBoxes[i, j].Margin = new Thickness(27 + (TextBoxWidth + HorizontalInterval) * j,
                                                                       56 + (TextBoxHeight + VerticalInterval) * i,
                                                                       0,
                                                                       0);
                            }
                    }
                }
                catch (Exception ex)
                {
                    _mainWindow.ErrorHandle(ex);
                }
            }
            else
            {
                try
                {
                    int col = Convert.ToInt32(Col.Text);
                    int row = Convert.ToInt32(Row.Text);
                    App.matpool.Remove(key);
                    TCL_Matrix.Matrix temp = new(row, col);
                    for (int i = 0; i < row; i++)
                        for (int j = 0; j < col; j++)
                        {
                            if (textBoxes[i, j].Text.Length == 0)
                            {
                                temp[i, j] = 0;
                            }
                            else
                            {
                                temp[i, j] = Convert.ToDouble(textBoxes[i, j].Text);
                            }
                        }
                    if (!App.matpool.ContainsKey(key))
                        App.matpool.Add(key, temp);
                    else
                        App.matpool[key] = temp;
                    _mainWindow.tmp1.Background = new SolidColorBrush((Color)ColorConverter.ConvertFromString("#FF0078D7"));
                    _mainWindow.tmp2.Background = new SolidColorBrush((Color)ColorConverter.ConvertFromString("#FF0078D7"));
                    _mainWindow.tmp3.Background = new SolidColorBrush((Color)ColorConverter.ConvertFromString("#FF0078D7"));
                    _mainWindow.EnvalueFlag = false;
                    Close();
                }
                catch (Exception ex)
                {
                    _mainWindow.ErrorHandle(ex);
                }
            }
        }
        private string key;
        private bool flag;
        private TextBox[,]? textBoxes;
    }
}
