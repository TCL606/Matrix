using Microsoft.VisualStudio.TestTools.UnitTesting;
using System;

namespace MatrixTest
{
    public partial class UnitTest
    {
        [TestMethod]
        public void LUTest()
        {
            double acc = LUAcc();
            Assert.IsTrue(acc == 1.0);
        }

        [TestMethod]
        public void QRTest()
        {
            double acc = QRAcc();
            Assert.IsTrue(acc == 1.0);
        }

        [TestMethod]
        public void SVDTest()
        {
            double acc = SVDAcc();
            Assert.IsTrue(acc > 0.55); //SVD的准确率其实不到六成
        }
    }
}
