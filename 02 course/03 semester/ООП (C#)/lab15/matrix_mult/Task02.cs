using System;
using System.Diagnostics;
using System.Threading;
using System.Threading.Tasks;

namespace matrix_mult
{
    public class Task02
    {
        public static double[,] MultMatrixParallelToken(
            double[,] matrixA,
            double[,] matrixB,
            CancellationToken token
        )
        {
            int size = matrixA.GetLength(0);
            double[,] result = new double[size, size];

            Parallel.For(
                0,
                size,
                new ParallelOptions { CancellationToken = token },
                i =>
                {
                    for (int j = 0; j < size; j++)
                    {
                        for (int k = 0; k < size; k++)
                        {
                            token.ThrowIfCancellationRequested();
                            result[i, j] += matrixA[i, k] * matrixB[k, j];
                        }
                    }
                }
            );

            return result;
        }

        public static void ComputeTask02()
        {
            Stopwatch sw = Stopwatch.StartNew();

            CancellationTokenSource cancellationTokenSource = new CancellationTokenSource();
            CancellationToken token = cancellationTokenSource.Token;

            Task<double[,]> result = Task.Run(
                () => MultMatrixParallelToken(Matrix.matrixA, Matrix.matrixB, token)
            );

            Task.Delay(500).ContinueWith(_ => cancellationTokenSource.Cancel());

            try
            {
                double[,] res = result.Result;
                sw.Stop();
                Console.WriteLine(
                    $"Время параллельного умножения с токеном: {sw.ElapsedMilliseconds} ms"
                );
            }
            catch (AggregateException z)
            {
                z.Handle(inner =>
                {
                    if (inner is OperationCanceledException)
                    {
                        Console.WriteLine("Отмена операции при помощи токена.");
                        return true;
                    }
                    return false;
                });
            }
        }
    }
}
