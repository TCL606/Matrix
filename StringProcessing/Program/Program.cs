﻿using StringProcessing;

namespace Test
{
    public static class Program
    {
        public static void Main()
        {
            String str = "3 + 2 * 5 - 6";
            Console.WriteLine(InputProcessing.Infix2Suffix(str));
        }
    }
}

