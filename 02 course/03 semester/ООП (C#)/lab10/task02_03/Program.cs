using System;
using System.Collections.Generic;
using System.Linq;

namespace task02_03
{
    internal class Program
    {
        #region ABITURS
        static readonly List<Abiturient> abiturients = new List<Abiturient>
        {
            new Abiturient("Иванов", "Иван", "Иванович", "Москва", "123456789", new[] { 8, 5, 3, 7 }),
            new Abiturient("Петров", "Пётр", "Петрович", "Санкт-Петербург", "987654321", new[] { 9, 10, 10, 9 }),
            new Abiturient("Сидоров", "Сидор", "Сидорович", "Новосибирск", "456789123", new[] { 6, 4, 5, 6 }),
            new Abiturient("Смирнов", "Андрей", "Андреевич", "Екатеринбург", "321654987", new[] { 7, 8, 9, 6 }),
            new Abiturient("Кузнецов", "Дмитрий", "Дмитриевич", "Нижний Новгород", "654321789", new[] { 10, 7, 8, 9 }),
            new Abiturient("Попов", "Алексей", "Алексеевич", "Челябинск", "159753468", new[] { 4, 3, 6, 5 }),
            new Abiturient("Новиков", "Виктор", "Викторович", "Казань", "753159486", new[] { 10, 9, 8, 9 }),
            new Abiturient("Васильев", "Олег", "Олегович", "Уфа", "951753852", new[] { 5, 6, 7, 6 }),
            new Abiturient("Морозов", "Николай", "Николаевич", "Самара", "852456963", new[] { 3, 10, 4, 5 }),
            new Abiturient("Фёдоров", "Игорь", "Игоревич", "Воронеж", "147258369", new[] { 7, 8, 6, 7 }),
            new Abiturient("Соловьёв", "Сергей", "Сергеевич", "Краснодар", "369258147", new[] { 9, 10, 8, 9 }),
            new Abiturient("Тихонов", "Артем", "Артёмович", "Астрахань", "258963147", new[] { 4, 5, 6, 7 }),
            new Abiturient("Борисов", "Юрий", "Юрьевич", "Вологда", "369147258", new[] { 9, 8, 7, 9 }),
            new Abiturient("Громов", "Владимир", "Владимирович", "Калуга", "147896325", new[] { 6, 10, 8, 7 }),
            new Abiturient("Семёнов", "Семён", "Семёнович", "Мурманск", "321789654", new[] { 10, 6, 7, 7 }),
            new Abiturient("Ковалёв", "Михаил", "Михайлович", "Тула", "654789321", new[] { 8, 9, 7, 6 })
        };
        #endregion

        static void Main(string[] args)
        {
            int choice = 0;
            while (true) 
            {
                Menu();
                int.TryParse(Console.ReadLine(), out choice);
                switch (choice)
                {
                    case 1:
                        MarksUnder4();
                        Console.ReadKey();
                        break;
                    case 2:
                        AbitursWithSumMoreEvex();
                        Console.ReadKey();
                        break;
                    case 3:
                        AbitursWith10From1Sub();
                        Console.ReadKey();
                        break;
                    case 4:
                        AbitursAlphabet();
                        Console.ReadKey();
                        break;
                    case 5:
                        AbitursLast4();
                        Console.ReadKey();
                        break;
                    case 0:
                        Console.ReadKey();
                        return;
                    default:
                        Console.WriteLine("неверная команда");
                        Console.ReadKey();
                        break;
                }
            }
        }

        static void Menu()
        {
            Console.Clear();
            Console.WriteLine("меню:");
            Console.WriteLine("1 - Абитуриенты с неудовлетворительными оценками.");
            Console.WriteLine("2 - Абитуриенты, у которых сумма оценок выше ...");
            Console.WriteLine("3 - Кол-во с десятками по первому предмету.");
            Console.WriteLine("4 - Абитуриенты в алфавитном порядке.");
            Console.WriteLine("5 - Последние 4 абитуриента с плохой успеваемостью.");
            Console.WriteLine("0 - выход.");
            Console.Write("\n\n команда -> ");
        }

        static void MarksUnder4()
        {
            var selectedAbiturs = abiturients.Where(a => a.Grades.Any(g => g < 4)).ToList();
            Console.WriteLine(" - - Абитуриенты с неудовлетворительными оценками:");
            foreach (var a in selectedAbiturs) Console.WriteLine(a);
        }

        static void AbitursWithSumMoreEvex()
        {
            int evex = 30;
            var selectedAbiturs = abiturients.Where(a => a.Grades.Sum() > evex).ToList();
            Console.WriteLine($" - - Абитуриенты, у которых сумма оценок выше {evex}:");
            foreach (var a in selectedAbiturs) Console.WriteLine(a);
        }

        static void AbitursWith10From1Sub()
        {
            int selectedAbiturs = abiturients.Count(a => a.Grades[0] == 10);
            Console.WriteLine(" - - Кол-во с десятками по первому предмету: {0}", selectedAbiturs);
        }

        static void AbitursAlphabet()
        {
            var selectedAbiturs = abiturients.OrderBy(a => a.Surname).ThenBy(a => a.Name).ToList();
            Console.WriteLine(" - - Абитуриенты в алфавитном порядке:");
            foreach (var a in selectedAbiturs) Console.WriteLine(a);
        }

        static void AbitursLast4()
        {
            var selectedAbiturs = abiturients.OrderBy(a => a.Grades.Sum()).Take(4).ToList();
            Console.WriteLine($" - - Последние 4 абитуриента с плохой успеваемостью:");
            foreach (var a in selectedAbiturs) Console.WriteLine(a);
        }
    }
}
