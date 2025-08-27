using System;
using System.Threading;

namespace test
{
    internal partial class Program
    {
        static void Main(string[] args)
        {
            while (true)
            {
                Console.Clear();
                Console.WriteLine("Меню:");
                Console.WriteLine("1. Task01");
                Console.WriteLine("2. Task02");
                Console.WriteLine("3. Task03");
                Console.WriteLine("4. Task04 (Четные и нечетные числа)");
                Console.WriteLine("5. Task05");
                Console.WriteLine("0. Выход");
                Console.Write("Выберите вариант: ");

                string choice = Console.ReadLine();

                switch (choice)
                {
                    case "1":
                        Task01();
                        break;
                    case "2":
                        Task02();
                        break;
                    case "3":
                        Task03();
                        break;
                    case "4":
                        Console.Write("-- сначала Чётные, потом Нечётные - 1\n-- последовательно - 2\nВыбор -> ");
                        int.TryParse(Console.ReadLine(), out Nums.param);
                        if (Nums.param == 1 || Nums.param == 2)
                        {
                            Task04();
                            Nums.number = 0;
                        }
                        else
                        {
                            Console.WriteLine("Ошибка");
                        }
                        break;
                    case "5":
                        Task05();
                        break;
                    case "0":
                        return;
                    default:
                        Console.WriteLine("Неверный выбор, попробуйте снова.");
                        break;
                }

                Console.WriteLine("Нажмите любую клавишу для продолжения...");
                Console.ReadKey();
            }
        }
    }
}