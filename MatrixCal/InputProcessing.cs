using System;
using System.Text;
using System.Collections.Generic;

namespace StringProcessing
{
    public static class InputProcessing
    {
        public static string PreProcess(string str)
        {
            foreach (var i in str)
            {
                if (i != '(' && i != ')' && i != '+' && i != '-' && i != '*' && i != '^' &&
                    (Convert.ToInt32(i) < 65 || Convert.ToInt32(i) > 90) &&
                    (Convert.ToInt32(i) < 48 || Convert.ToInt32(i) > 57))
                {
                    throw new Exception("检测到非法字符");
                }
            }
            for (int i = 1; i < str.Length; i++)
            {
                if ((!(Convert.ToInt32(str[i]) < 65 || Convert.ToInt32(str[i]) > 90) || str[i] == '(') &&
                   str[i - 1] != '+' && str[i - 1] != '-' && str[i - 1] != '*' && str[i - 1] != '^' && str[i - 1] != '(')
                    str = str.Insert(i, "*");
                i++;
            }
            for (int i = 0; i < str.Length; i++)
            {
                if ((Convert.ToInt32(str[i]) < 48 || Convert.ToInt32(str[i]) > 57))//不是数
                {
                    if (str[i] != ' ')
                        str = str.Insert(i + 1, " ");
                }
                else//是数
                {
                    if (i < str.Length - 1 && (Convert.ToInt32(str[i + 1]) < 48 || Convert.ToInt32(str[i + 1]) > 57))//后一个不是数
                        str = str.Insert(i + 1, " ");
                }
            }
            if (str[str.Length - 1] == ' ')
                str = str.Remove(str.Length - 1);
            return str;
        }
        public static string Infix2Suffix(string str)
        {
            str = PreProcess(str);
            StringBuilder sb = new StringBuilder();
            Stack<string> stack = new Stack<string>();
            var arr = str.Split(' ');
            for (int i = 0; i < arr.Length; i++)
            {
                if (arr[i] == "(")
                    stack.Push(arr[i]);
                else if (arr[i] == ")")
                {
                    var t = stack.Pop();
                    while (t != "(")
                    {
                        sb.Append(t + " ");
                        t = stack.Pop();
                    }
                }
                else if (!IsOperator(arr[i]))
                {
                    sb.Append(arr[i] + " ");
                }
                else
                {
                    if (stack.Count == 0 || stack.Peek() == "(")
                        stack.Push(arr[i]);
                    else if (GetPriority(stack.Peek()) < GetPriority(arr[i]))
                        stack.Push(arr[i]);
                    else
                    {
                        i--;
                        sb.Append(stack.Pop() + " ");
                    }
                }
            }
            while (stack.Count > 0)
            {
                sb.Append(stack.Pop() + " ");
            }
            //String temp=sb.ToString();
            //for(int i=0; i < temp.Length; i++)
            //{
            //    if (i < temp.Length - 1 && temp[i] == ' ' && temp[i + 1] == ' ')
            //    {
            //        temp=temp.Remove(i);
            //        i--;
            //    }
            //}//保险
            return sb.ToString();
        }
        public static int GetPriority(string sign)
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
            return GetPriority(str) > 0;
        }
    }
}