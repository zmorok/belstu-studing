using System;
using System.IO;
using System.Threading;

namespace test
{
    internal partial class Program
    {
        private static void Task03()
        {
            Console.Write("Введите n: ");
            int n = int.Parse(Console.ReadLine());

            Thread thread = new(() => CalculatePrimes(n));
            thread.Start();

            while (thread.IsAlive)
            {
                Console.WriteLine($"ID потока: {thread.ManagedThreadId}, Имя: {thread.Name}, ");
                Console.WriteLine($"Приоритет: {thread.Priority}, Состояние: {thread.ThreadState}");
                Thread.Sleep(500);
            }

            thread.Join();
        }

        static void CalculatePrimes(int n)
        {
            using (StreamWriter writer = new StreamWriter("primes.txt"))
            {
                for (int i = 1; i <= n; i++)
                {
                    if (IsPrime(i))
                    {
                        Console.WriteLine(i);
                        writer.WriteLine(i);
                        Thread.Sleep(100);
                    }
                }
            }
        }

        static bool IsPrime(int number)
        {
            if (number < 2) return false;
            for (int i = 2; i <= Math.Sqrt(number); i++)
            {
                if (number % i == 0) return false;
            }
            return true;
        }
    }
}
