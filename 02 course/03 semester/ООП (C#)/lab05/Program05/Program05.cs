using System;
using Tecnologies;

namespace Program05
{
    internal class Program05
    {
        static void Main(string[] args)
        {
            LabController controller = new LabController();

            controller.AddTech(
                new PrintDevice(
                    "Принтер",
                    150,
                    "ХэПэ",
                    20,
                    TechType.PrintDevice,
                    new Warranty(3, DateTime.Now)
                )
            );
            controller.AddTech(
                new PrintDevice(
                    "Принтер",
                    2000,
                    "НоуХ",
                    100,
                    TechType.PrintDevice,
                    new Warranty(0, DateTime.Now)
                )
            );
            controller.AddTech(
                new PrintDevice(
                    "Принтер",
                    400,
                    "АксВВ",
                    40,
                    TechType.PrintDevice,
                    new Warranty(5, DateTime.Now)
                )
            );
            controller.AddTech(
                new Scaner(
                    "Сканер",
                    100,
                    "Канон",
                    600,
                    TechType.Scaner,
                    new Warranty(2, DateTime.Now)
                )
            );
            controller.AddTech(
                new Computer(
                    "ПК",
                    1200,
                    "Делл",
                    "I7",
                    16,
                    TechType.Computer,
                    new Warranty(5, DateTime.Now)
                )
            );
            controller.AddTech(
                new Tablet(
                    "Пад",
                    400,
                    "Сяоме",
                    10.2,
                    TechType.Tablet,
                    new Warranty(1, DateTime.Now)
                )
            );

            Console.WriteLine("\nВся техника:");
            controller.DisplayTechs();

            int y = 4;
            Console.WriteLine($"\nТехника старше {y}х лет:");
            controller.FindOldTech(y);

            Console.WriteLine("\nКоличество каждой техники:");
            controller.CountTechTypes();

            Console.WriteLine("\nОтсортированная техника по цене:");
            controller.SortAndDisplayByPrice();
        }
    }
}
