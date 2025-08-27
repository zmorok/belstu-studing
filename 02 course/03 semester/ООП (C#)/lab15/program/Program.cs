using System;
using System.Threading.Tasks;
using matrix_mult;

namespace program
{
    internal class Program
    {
        static async Task Main(string[] args)
        {
            while (true)
            {
                Console.Clear();
                Menu();
                int.TryParse(Console.ReadLine(), out int choice);
                switch (choice)
                {
                    case 1:
                        Task01();
                        break;
                    case 2:
                        Task02();
                        break;
                    case 3:
                        Task03();
                        break;
                    case 4:
                        Task03();
                        break;
                    case 5:
                        Task05();
                        break;
                    case 6:
                        Task06();
                        break;
                    case 7:
                        Task07();
                        break;
                    case 8:
                        await Task08();
                        break;
                    case 0:
                        return;
                }
            }
        }

        static void Menu()
        {
            Console.WriteLine("Лабораторная работа 15 (Task Parallel Library)");
            Console.WriteLine("1. Перемножение матриц (последовательно и паралельно)");
            Console.WriteLine("2. Перемножение матриц с токеном (последовательно и паралельно)");
            Console.WriteLine("3 (+4). Работа с тасками и получение результатов");
            Console.WriteLine("5. Генерация и обработка массива");
            Console.WriteLine("6. Работа с параллельным Invoke");
            Console.WriteLine("7. BlockingCollection - Склад: завозчики и покупатели");
            Console.WriteLine("8. async и await");

            Console.Write("Введите задание -> ");
        }

        static void Task01()
        {
            Console.WriteLine("Выполнение 1...");
            Console.Write("Введите размер матриц -> ");
            int.TryParse(Console.ReadLine(), out int size);
            Matrix.SetSize(size);
            matrix_mult.Task01.ComputeTask01();
            Console.WriteLine("Нажмите любую клавишу, чтобы продолжить...");
            Console.ReadKey();
        }

        static void Task02()
        {
            Console.WriteLine("Выполнение 2...");
            Console.Write("Введите размер матриц -> ");
            int.TryParse(Console.ReadLine(), out int size);
            Matrix.SetSize(size);
            matrix_mult.Task02.ComputeTask02();
            Console.WriteLine("Нажмите любую клавишу, чтобы продолжить...");
            Console.ReadKey();
        }

        static void Task03()
        {
            Console.WriteLine("Выполнение 3 - 4...");
            matrix_mult.Task03.ComputeTask03();
            Console.WriteLine("Нажмите любую клавишу, чтобы продолжить...");
            Console.ReadKey();
        }

        static void Task05()
        {
            Console.WriteLine("Выполнение 5...\n");
            matrix_mult.Task05.ComputeTask05();
            Console.WriteLine("Нажмите любую клавишу, чтобы продолжить...");
            Console.ReadKey();
        }

        static void Task06()
        {
            Console.WriteLine("Выполнение 6...\n");
            matrix_mult.Task06.ComputeTask06();
            Console.WriteLine("Нажмите любую клавишу, чтобы продолжить...");
            Console.ReadKey();
        }

        static void Task07()
        {
            Console.WriteLine("Выполнение 7...\n");
            matrix_mult.Task07.ComputeTask07();
            Console.WriteLine("Нажмите любую клавишу, чтобы продолжить...");
            Console.ReadKey();
        }

        static async Task Task08()
        {
            Console.WriteLine("Выполнение 8...\n");
            await matrix_mult.Task08.ComputeTask08();
            Console.WriteLine("Нажмите любую клавишу, чтобы продолжить...");
            Console.ReadKey();
        }
    }
}
