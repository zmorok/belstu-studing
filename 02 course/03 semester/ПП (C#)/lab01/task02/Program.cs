using System;

namespace task02
{
    internal class Program
    {
        public interface IStudent
        {
            // свойства
            string Name { get; set; }
            double GPA { get; set; }

            // методы
            void DisplayInfo();
        }

        public class Teacher : IStudent
        {
            // Константы
            private const double MIN_GPA = 0.0;
            public const string CLASS_NAME = "Teacher Class";
            protected const int MAX_AGE = 120;

            // Поля
            private int _id;
            public string Name { get; set; }
            public double GPA { get; set; }
            protected int Age;
            protected string Subject;
            private string PrivateNotes { get; set; }

            // Конструктор по умолчанию
            public Teacher()
            {
                _id = 0;
                Name = "Имя по умолчанию";
                Age = 30;
                GPA = MIN_GPA;
                Subject = "Не определено";
                PrivateNotes = "Нет записей";
            }

            // Конструктор с параметрами
            public Teacher(int id, string name, int age, double gpa, string subject, string notes = "Нет записей")
            {
                _id = id;
                Name = name;
                Age = age;
                GPA = gpa;
                Subject = subject;
                PrivateNotes = notes;
            }

            // Конструктор копирующий
            public Teacher(Teacher other)
            {
                _id = other._id;
                Name = other.Name;
                Age = other.Age;
                GPA = other.GPA;
                Subject = other.Subject;
                PrivateNotes = other.PrivateNotes;
            }

            // Реализация метода из интерфейса
            public void DisplayInfo()
            {
                Console.WriteLine($"ID: {_id}");
                Console.WriteLine($"Имя: {Name}");
                Console.WriteLine($"Возраст: {Age}");
                Console.WriteLine($"Средняя оценка: {(GPA == 0.0 ? "Не нужно" : GPA.ToString())}");
                Console.WriteLine($"Предмет: {Subject}");
                Console.WriteLine($"Записи: {PrivateNotes}");
            }

            // Приватный метод
            private void AddPrivateNotes(string notes)
            {
                PrivateNotes = notes;
                Console.WriteLine("Личная запись добавлена");
            }

            // Защищённый метод
            protected void UpdateSubject(string newSubject)
            {
                Subject = newSubject;
                Console.WriteLine("Предмет обновлён");
            }
        }

        static void Main(string[] args)
        {
            string fl = "------------------------------";
            Console.WriteLine("Объект через конструктор по умолчанию:\n" + fl);
            Teacher teacher1 = new Teacher();
            teacher1.DisplayInfo();
            Console.WriteLine(fl + "\n\n");

            Console.WriteLine("Объект через конструктор с параметрами:\n" + fl);
            Teacher teacher2 = new Teacher(
                1,
                "Грейс",
                41,
                0,
                "Компьютерная наука",
                "Старший преподаватель"
            );
            teacher2.DisplayInfo();
            Console.WriteLine(fl + "\n\n");

            Console.WriteLine("Объект через копирующий конструктор:\n" + fl);
            Teacher teacher3 = new Teacher(teacher2);
            teacher3.DisplayInfo();
            Console.WriteLine(fl);
        }
    }
}
