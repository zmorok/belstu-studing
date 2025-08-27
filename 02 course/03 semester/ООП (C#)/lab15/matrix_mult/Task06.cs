using System;
using System.Diagnostics;
using System.Threading.Tasks;

namespace matrix_mult
{
    public class Task06
    {
        public static void ComputeTask06()
        {
            Stopwatch sw = new Stopwatch();
            string filler = new('#', 50);

            Console.WriteLine("ПОСЛЕДОВАТЕЛЬНАЯ РАБОТА:\n{0}", filler);
            sw.Start();
            Part1();
            Part2();
            Part3();
            sw.Stop();
            Console.WriteLine(
                $"Последовательное выполнение: {sw.ElapsedMilliseconds} ms\n{filler}\n"
            );

            Console.WriteLine("ПАРАЛЛЕЛЬНЯ РАБОТА:\n{0}", filler);
            sw.Restart();
            Parallel.Invoke(Part1, Part2, Part3);
            sw.Stop();
            Console.WriteLine(
                $"Через параллельное Invoke: {sw.ElapsedMilliseconds} ms\n{filler}\n"
            );
        }

        static void Part1()
        {
            Console.WriteLine("Part1++++++++++++++++++");
            double sum = 0;
            for (int i = 0; i < 10000000; i++)
            {
                sum += Math.Sqrt(i);
            }
            Console.WriteLine($"\t\tСумма: {sum}\nPart1-----------------");
        }

        static void Part2()
        {
            Console.WriteLine("Part2++++++++++++++++++");
            long factorial = 1;
            for (int i = 1; i <= 20; i++)
            {
                factorial *= i;
            }
            Console.WriteLine($"\t\tФакториал 20: {factorial}\nPart2-----------------");
        }

        static void Part3()
        {
            Console.WriteLine("Part3++++++++++++++++++");
            int[] array = new int[1000000];
            for (int i = 0; i < array.Length; i++)
            {
                array[i] = i % 100;
            }
            Console.WriteLine(
                $"\t\tПоследний элемент: {array.GetValue(array.Length - 1)}\nPart3-----------------"
            );
        }
    }
}
