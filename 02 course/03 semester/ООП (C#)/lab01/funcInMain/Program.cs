using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace funcInMain
{
    internal class Program
    {
        static void Main(string[] args)
        {
            int[] numbers = { 1, 5, 3, 9, 2 };
            string text = "Пример текста";

            (int max, int min, int sum, char firstLetter) ProcessArrayAndString(int[] arr, string str)
            {
                int maxElement = arr.Max();
                int minElement = arr.Min();
                int sumElements = arr.Sum();
                char firstLetter = str.Length > 0 ? str[0] : '\0';

                return (maxElement, minElement, sumElements, firstLetter);
            }

            var result = ProcessArrayAndString(numbers, text);

            Console.WriteLine($"Max: {result.max}, Min: {result.min}, Sum: {result.sum}, Первая буква: {result.firstLetter}");
        }
    }
}
