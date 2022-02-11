using Microsoft.VisualStudio.TestTools.UnitTesting;
using System;

namespace MatrixTest
{
    public static partial class UnitTest
    {
        [TestMethod]
        public static void LUTest()
        {
            double acc = LUAcc();
            Assert.IsTrue(acc == 1.0);
        }

        [TestMethod]
        public static void QRTest()
        {
            double acc = QRAcc();
            Assert.IsTrue(acc == 1.0);
        }

        [TestMethod]
        public static void SVDTest()
        {
            double acc = SVDAcc();
            Assert.IsTrue(acc > 0.55); //SVD的准确率其实不到六成
        }

        [TestMethod]
        public static void EigenTest()
        {
            double acc = EigenAcc();
            Assert.IsTrue(acc > 0.6);
        }
    }
}
