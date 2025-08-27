using System;
using System.Collections.Generic;
using System.Linq;

namespace task04_05
{
    internal class Program
    {
        #region ABITURS
        static readonly List<Abiturient> abiturients = new List<Abiturient>
        {
            new Abiturient(
                "Иванов",
                "Иван",
                "Иванович",
                "Москва",
                "123456789",
                new[] { 8, 5, 3, 7 }
            ),
            new Abiturient(
                "Петров",
                "Пётр",
                "Петрович",
                "Санкт-Петербург",
                "987654321",
                new[] { 9, 10, 10, 9 }
            ),
            new Abiturient(
                "Сидоров",
                "Сидор",
                "Сидорович",
                "Новосибирск",
                "456789123",
                new[] { 6, 4, 5, 6 }
            ),
            new Abiturient(
                "Смирнов",
                "Андрей",
                "Андреевич",
                "Екатеринбург",
                "321654987",
                new[] { 7, 8, 9, 6 }
            ),
            new Abiturient(
                "Кузнецов",
                "Дмитрий",
                "Дмитриевич",
                "Нижний Новгород",
                "654321789",
                new[] { 10, 7, 8, 9 }
            ),
            new Abiturient(
                "Попов",
                "Алексей",
                "Алексеевич",
                "Челябинск",
                "159753468",
                new[] { 4, 3, 6, 5 }
            ),
            new Abiturient(
                "Новиков",
                "Виктор",
                "Викторович",
                "Казань",
                "753159486",
                new[] { 10, 9, 8, 9 }
            ),
            new Abiturient(
                "Васильев",
                "Олег",
                "Олегович",
                "Уфа",
                "951753852",
                new[] { 5, 6, 7, 6 }
            ),
            new Abiturient(
                "Морозов",
                "Николай",
                "Николаевич",
                "Самара",
                "852456963",
                new[] { 3, 10, 4, 5 }
            ),
            new Abiturient(
                "Фёдоров",
                "Игорь",
                "Игоревич",
                "Воронеж",
                "147258369",
                new[] { 7, 8, 6, 7 }
            ),
            new Abiturient(
                "Соловьёв",
                "Сергей",
                "Сергеевич",
                "Краснодар",
                "369258147",
                new[] { 9, 10, 8, 9 }
            ),
            new Abiturient(
                "Тихонов",
                "Артем",
                "Артёмович",
                "Астрахань",
                "258963147",
                new[] { 4, 5, 6, 7 }
            ),
            new Abiturient(
                "Борисов",
                "Юрий",
                "Юрьевич",
                "Вологда",
                "369147258",
                new[] { 9, 8, 7, 9 }
            ),
            new Abiturient(
                "Громов",
                "Владимир",
                "Владимирович",
                "Калуга",
                "147896325",
                new[] { 6, 10, 8, 7 }
            ),
            new Abiturient(
                "Семёнов",
                "Семён",
                "Семёнович",
                "Мурманск",
                "321789654",
                new[] { 10, 6, 7, 7 }
            ),
            new Abiturient(
                "Ковалёв",
                "Михаил",
                "Михайлович",
                "Тула",
                "654789321",
                new[] { 8, 9, 7, 6 }
            )
        };
        #endregion

        #region SUBJECTS
        static readonly List<Subject> subjects = new List<Subject>
        {
            new Subject("Иванов", "Физика", 9),
            new Subject("Петров", "Математика", 10),
            new Subject("Сидоров", "Химия", 8),
            new Subject("Смирнов", "Биология", 9),
            new Subject("Кузнецов", "География", 10),
            new Subject("Попов", "История", 6),
            new Subject("Новиков", "Литература", 7),
            new Subject("Соловьёв", "Информатика", 10),
            new Subject("Тихонов", "Экономика", 5),
            new Subject("Громов", "Физика", 10),
            new Subject("Семёнов", "Математика", 8),
            new Subject("Ковалёв", "Химия", 9)
        };
        #endregion

        static void Main(string[] args)
        {
            // 5 операторов разных типов
            var selectedAbiturs = abiturients
                .Where(a => a.Grades.Sum() > 30)
                .GroupBy(a => a.Address)
                .Select(g => new
                {
                    Город = g.Key,
                    Кол_во = g.Count(),
                    avg = g.Average(a => a.Grades.Average())
                })
                .OrderByDescending(a => a.avg)
                .ToList();

            foreach (var a in selectedAbiturs)
                Console.WriteLine(a);

            // join
            var joinedAbiturs =
                from a in abiturients
                join sb in subjects on a.Surname equals sb.AbiturientSurname
                select new
                {
                    AbiturientName = $"{a.Surname} {a.Name}",
                    sb.SubjectName,
                    sb.Grade
                };

            Console.WriteLine("\nАбитуриенты и их оценки по предметам:");
            foreach (var item in joinedAbiturs)
                Console.WriteLine($"{item.AbiturientName} - {item.SubjectName}: {item.Grade}");
        }
    }
}
