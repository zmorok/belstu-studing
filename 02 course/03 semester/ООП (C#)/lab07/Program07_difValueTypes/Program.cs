using System;
using System.Collections.Generic;
using CommonSet;

namespace Program07
{
    internal class Program
    {
        static void Main(string[] args)
        {
            Console.WriteLine("INT ------------------------");
            var intSet = new CommonSet<int>();
            int[] Ivals = { 1, 4, 5, 67, 42, 2432, 5 };
            foreach (int val in Ivals) { intSet.Add(val); }
            Console.WriteLine(intSet.Contains(15));

            List<int> foundInts = (List<int>)intSet.Find(x => x > 5);
            Console.WriteLine($"Найден элемент в intSet: {string.Join(", ", foundInts)}");

            Console.WriteLine("\nDOUBLE ------------------------");
            var doubleSet = new CommonSet<double>();
            double[] Dvals = { 1.23,343.52,523.34,3.1,4d,0.13,.023,12d };
            foreach (double val in Dvals) { doubleSet.Add(val); }
            Console.WriteLine(doubleSet.Contains(1.23));

            List<double> foundDoubles = (List<double>)doubleSet.Find(x => x < 2.0);
            Console.WriteLine($"Найден элемент в doubleSet: {string.Join(", ", foundDoubles)}");

            Console.WriteLine("\nРАБОТА С ФАЙЛАМИ:");
            string filePathInt = "intSet.json";
            string filePathDouble = "doubleSet.json";

            intSet.SaveToFile(filePathInt);
            doubleSet.SaveToFile(filePathDouble);

            var newIntSet = new CommonSet<int>();
            newIntSet.LoadFromFile(filePathInt);

            Console.WriteLine("коллекция из файла {0}: {1}", filePathInt, newIntSet);

            var newDoubleSet = new CommonSet<double>();
            newDoubleSet.LoadFromFile(filePathDouble);

            Console.WriteLine("коллекция из файла {0}: {1}", filePathDouble, newDoubleSet);
        }
    }
}
