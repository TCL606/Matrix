using System.Windows;
using System.Collections.Generic;

namespace MatrixCal
{
    /// <summary>
    /// Interaction logic for App.xaml
    /// </summary>
    public partial class App : Application
    {
        public static Dictionary<string, TCL_Matrix.Matrix>? matpool,temppool;
        public static bool storeFlag;
    }
}
