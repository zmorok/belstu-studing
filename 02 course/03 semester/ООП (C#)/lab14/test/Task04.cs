using System;
using System.Threading;

namespace test
{
    static class Nums
    {
        private static readonly object locker = new object();
        public static int number = 0;
        public static int max = 20;

        public static int param;

        public static void EvenNumbers()
        {
            if (param == 2)
            {
                while (number <= max)
                {
                    lock (locker)
                    {
                        if (number % 2 == 0)
                        {
                            Console.Write($"{number} ");
                            number++;
                            Monitor.Pulse(locker);
                        }
                        else
                        {
                            Monitor.Wait(locker);
                        }
                    }
                    Thread.Sleep(100);
                }
            }
            else
            {
                lock (locker)
                {
                    while (number <= max)
                    {
                        if (number % 2 == 0)
                        {
                            Console.Write($"{number} ");
                        }
                        number++;
                    }
                    number = 0;
                }
            }
        }

        public static void OddNumbers()
        {
            if (param == 2)
            {
                while (number <= max)
                {
                    lock (locker)
                    {
                        if (number % 2 != 0)
                        {
                            Console.Write($"{number}| ");
                            number++;
                            Monitor.Pulse(locker);
                        }
                        else
                        {
                            Monitor.Wait(locker);
                        }
                    }
                    Thread.Sleep(10);
                }
            }
            else
            {
                lock (locker)
                {
                    while (number <= max)
                    {
                        if (number % 2 != 0)
                        {
                            Console.Write($"{number}| ");
                        }
                        number++;
                    }
                    number = 0;
                }
                
            }
        }
    }

    internal partial class Program
    {
        private static void Task04()
        {
            Console.WriteLine("\n\n\nПоток четных и нечетных чисел:");

            Thread evenThread = new(Nums.EvenNumbers) { Priority = ThreadPriority.Highest };
            Thread oddThread = new(Nums.OddNumbers) { Priority = ThreadPriority.Lowest };

            evenThread.Start();
            oddThread.Start();

            oddThread.Join();
            evenThread.Join();
        }
    }
}
