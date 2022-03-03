using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;

namespace MatrixCal
{
    /// <summary>
    /// Output.xaml 的交互逻辑
    /// </summary>
    public partial class Output : Window
    {
        const int TextBoxHeight = 15;
        const int TextBoxWidth = 48;
        const int VerticalInterval = 3;
        const int HorizontalInterval = 5;
        public Output(string s)
        {
            InitializeComponent();
            Key.Text = s;
            Key.IsReadOnly = true;
            TCL_Matrix.Matrix p = App.matpool[s];
            Height += (TextBoxHeight + VerticalInterval) * p.Row;
            Width = (TextBoxWidth + HorizontalInterval) * p.Col + 54 > Width ?
                    (TextBoxWidth + HorizontalInterval) * p.Col + 54 : Width;
            Row.Text = Convert.ToString(p.Row);
            Col.Text = Convert.ToString(p.Col);
            textBoxes = new TextBox[p.Row, p.Col];
            for (int i = 0; i < p.Row; i++)
                for (int j = 0; j < p.Col; j++)
                {
                    textBoxes[i, j] = new();
                    textBoxes[i, j].Text = p[i, j].ToString("G4");
                    if (textBoxes[i, j].Text == "-0")
                        textBoxes[i, j].Text = "0";
                    textBoxes[i, j].IsReadOnly = true;
                    OutputGrid.Children.Add(textBoxes[i, j]);
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
        private void DragWindow(object sender, MouseButtonEventArgs e)
        {
            DragMove();
        }
        private void OutputClose(object sender, RoutedEventArgs e)
        {
            Close();
        }
        private TextBox[,]? textBoxes;
    }
}
