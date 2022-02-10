using System;
using System.Text;

namespace StringProcessing
{
    public static class InputProcessing
    {
        public static String Infix2Suffix(String str)
        {
            StringBuilder sb = new StringBuilder();
            Stack<string> stack = new Stack<string> ();
            var arr = str.Split(' ');
            for (int i = 0; i < arr.Length; i++)
            {
                if(arr[i] == "(")
                    stack.Push(arr[i]);
                else if(arr[i] == ")")
                {
                    var t = stack.Pop();
                    if (t != "(")
                        sb.Append(t + " ");
                }
                else if(!IsOperator(arr[i]))
                {
                    sb.Append(arr[i] + " ");
                }
                else
                {
                    if (stack.Count == 0)
                        stack.Push(arr[i]);
                    else if (GetPrioraty(stack.Peek()) < GetPrioraty(arr[i]))
                        stack.Push(arr[i]);
                    else
                    {
                        i--;
                        sb.Append(stack.Pop() + " ");
                    }
                }
            }
            while(stack.Count > 0)
            {
                sb.Append(stack.Pop() + " ");
            }
            return sb.ToString();
        }
        public static int GetPrioraty(string sign)
        {
            switch (sign)
            {
                case "+":
                case "-":
                    return 1;
                case "*":
                case "/":
                    return 2;
                case "^":
                    return 3;
                case "(":
                    return 10086;
                default:
                    return 0;
            }
        }
        public static bool IsOperator(string str)
        {
            return GetPrioraty(str) > 0;
        }
    }
}