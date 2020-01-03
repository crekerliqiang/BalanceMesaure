using System;
using System.Text.RegularExpressions;


namespace ConsoleApp1
{
    class Program
    {
        static void Main(string[] args)
        {
            Console.WriteLine("Hello World!");

            string str = "aaajsbbbjsccc";

            string[] sArray = Regex.Split(str, "j1s", RegexOptions.IgnoreCase);


            foreach (string i in sArray)
            {
                Console.WriteLine(i.ToString() + "");
            }

        }
    }
}
