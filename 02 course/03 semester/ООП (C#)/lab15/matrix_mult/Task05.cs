using System;
using System.Diagnostics;
using System.Threading.Tasks;

namespace matrix_mult
{
    public class Task05
    {
        public static void ComputeTask05()
        {
            int size = 3000000;
            int it = 100;

            int[][] arrays = new int[it][];
            for (int i = 0; i < it; i++)
            {
                arrays[i] = new int[size];
            }

            Stopwatch sw = Stopwatch.StartNew();
            for (int i = 0; i < it; i++)
            {
                for (int j = 0; j < size; j++)
                {
                    arrays[i][j] = j * j;
                }
            }
            sw.Stop();
            Console.WriteLine($"Обычный for: {sw.ElapsedMilliseconds} ms");

            sw.Restart();
            Parallel.For(
                0,
                it,
                i =>
                {
                    for (int j = 0; j < size; j++)
                    {
                        arrays[i][j] = j * j;
                    }
                }
            );
            sw.Stop();
            Console.WriteLine($"Параллельный for: {sw.ElapsedMilliseconds} ms");

            sw.Restart();
            foreach (var ar in arrays)
            {
                for (int j = 0; j < size; j++)
                {
                    ar[j] *= 2;
                }
            }
            sw.Stop();
            Console.WriteLine($"Обычный foreach: {sw.ElapsedMilliseconds} ms");

            sw.Restart();
            Parallel.ForEach(
                arrays,
                ar =>
                {
                    for (int j = 0; j < size; j++)
                    {
                        ar[j] *=  2;
                    }
                }
            );
            sw.Stop();
            Console.WriteLine($"Параллельный foreach: {sw.ElapsedMilliseconds} ms");
        }
    }
}
