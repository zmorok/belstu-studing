using System;
using System.Threading.Tasks;

namespace matrix_mult
{
    public class Task03
    {
        public static void ComputeTask03()
        {
            Console.WriteLine("Решение задач с вводом значений.");

            Console.WriteLine("Введите 2 слогаемых через пробел:");
            (int a0, int b0) = ReadTwoIntegers();
            Task<int> task1 = Task.Run(() => Sum(a0, b0));

            Console.WriteLine("Введите 2 числа для разности через пробел:");
            (int a1, int b1) = ReadTwoIntegers();
            Task<int> task2 = Task.Run(() => Difference(a1, b1));

            Console.WriteLine("Введите коэффициенты квадратного уравнения a, b, c через пробел:");
            (int a, int b, int c) = ReadThreeIntegers();
            Task<double[]> task3 = Task.Run(() => GetX1_2(a, b, c));

            var sum = task1.GetAwaiter().GetResult();
            var dif = task2.GetAwaiter().GetResult();

            double[] xs = { };
            task3.ContinueWith(_ => { xs = task3.Result; }).Wait();

            Console.WriteLine(
                "\tСумма: {0}\n" + "\tРазность: {1}\n" + "\tКорни квадратного уравнения: {2}",
                sum,
                dif,
                xs.Length > 0 ? string.Join(", ", xs) : "Нет вещественных корней"
            );
        }

        static int Sum(int x, int y) => x + y;

        static int Difference(int x, int y) => x - y;

        static double[] GetX1_2(int a, int b, int c)
        {
            double discriminant = b * b - 4 * a * c;
            if (discriminant > 0)
            {
                double x1 = (-b + Math.Sqrt(discriminant)) / (2 * a);
                double x2 = (-b - Math.Sqrt(discriminant)) / (2 * a);
                return new double[] { x1, x2 };
            }
            else if (discriminant == 0)
            {
                double x = -b / (2 * a);
                return new double[] { x };
            }
            else
                return new double[0];
        }

        static (int, int) ReadTwoIntegers()
        {
            while (true)
            {
                string input = Console.ReadLine();
                string[] parts = input.Split(' ');

                if (
                    parts.Length == 2
                    && int.TryParse(parts[0], out int a)
                    && int.TryParse(parts[1], out int b)
                )
                {
                    return (a, b);
                }

                Console.WriteLine("Некорректный ввод. Введите два целых числа через пробел.");
            }
        }

        static (int, int, int) ReadThreeIntegers()
        {
            while (true)
            {
                string input = Console.ReadLine();
                string[] parts = input.Split(' ');

                if (
                    parts.Length == 3
                    && int.TryParse(parts[0], out int a)
                    && int.TryParse(parts[1], out int b)
                    && int.TryParse(parts[2], out int c)
                )
                {
                    return (a, b, c);
                }

                Console.WriteLine("Некорректный ввод. Введите три целых числа через пробел.");
            }
        }
    }
}
