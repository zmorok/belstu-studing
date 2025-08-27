using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace tuples
{
    internal class Program
    {
        static void Main(string[] args)
        {
            (int, string, char, string, ulong) t1 = (12, "sd", 'd', "qeqe", 12332);
            Console.WriteLine("Кортеж целиком: {0}", t1);
            Console.WriteLine("1-ый, 3-ий и 4-ый элементы кортежа: {0}, {1}, {2}", t1.Item1, t1.Item3, t1.Item4);

            // распаковки кортежа
            (int iVal, string sVal1, char cVal, string sVal2, ulong ulVal) = t1;
            var (v1, v2, v3, _, v5) = t1;
            Console.WriteLine($"{v1}, {v2}, {v3}, {v5}");

            // сравнение кортежей
            var t2 = (1, "ds", '3', "231", Convert.ToUInt64(32));
            Console.WriteLine(t2 == t1 ? "Равны" : "Не равны");
        }
    }
}
