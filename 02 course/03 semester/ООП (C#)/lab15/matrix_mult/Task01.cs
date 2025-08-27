using System;
using System.Diagnostics;
using System.Threading.Tasks;

namespace matrix_mult
{
    public class Task01
    {
        public static double[,] MultMatrixSequential(double[,] matrixA, double[,] matrixB)
        {
            int size = matrixA.GetLength(0);
            double[,] result = new double[size, size];

            for (int i = 0; i < size; i++)
            {
                for (int j = 0; j < size; j++)
                {
                    for (int k = 0; k < size; k++)
                    {
                        result[i, j] += matrixA[i, k] * matrixB[k, j];
                    }
                }
            }

            return result;
        }

        public static double[,] MultMatrixParallel(double[,] matrixA, double[,] matrixB)
        {
            int size = matrixA.GetLength(0);
            double[,] result = new double[size, size];

            Parallel.For(
                0,
                size,
                i =>
                {
                    for (int j = 0; j < size; j++)
                    {
                        for (int k = 0; k < size; k++)
                        {
                            result[i, j] += matrixA[i, k] * matrixB[k, j];
                        }
                    }
                }
            );

            return result;
        }

        public static void ComputeTask01()
        {
            Stopwatch sw = Stopwatch.StartNew();
            _ = MultMatrixSequential(Matrix.matrixA, Matrix.matrixB);
            sw.Stop();
            Console.WriteLine($"Время последовательного умножения: {sw.ElapsedMilliseconds} ms");

            sw.Restart();
            _ = MultMatrixParallel(Matrix.matrixA, Matrix.matrixB);
            sw.Stop();
            Console.WriteLine($"Время параллельного умножения: {sw.ElapsedMilliseconds} ms");
        }
    }
}
