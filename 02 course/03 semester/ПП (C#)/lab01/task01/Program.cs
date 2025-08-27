using System;

namespace task01
{
    internal class Program
    {
        public class Student
        {
            // константы
            private const double MIN_GPA = 0.0;
            public const string CLASS_NAME = "Student Class";
            protected const int MAX_AGE = 120;

            // поля
            private int _id;
            public string Name;
            protected int Age;

            // свойства
            private string PrivateNotes { get; set; }
            public double GPA { get; set; }
            protected string Major { get; set; }

            // конструктор по умолчанию
            public Student()
            {
                _id = 0;
                Name = "Имя по умолчанию";
                Age = 18;
                GPA = MIN_GPA;
                Major = "Не назначено";
                PrivateNotes = "Нет записей";
            }

            // конструктор с параметрами
            public Student(
                int id,
                string name,
                int age,
                double gpa,
                string major,
                string notes = "Нет записей"
            )
            {
                _id = id;
                Name = name;
                Age = age;
                GPA = gpa;
                Major = major;
                PrivateNotes = notes;
            }

            // конструктор копирующий
            public Student(Student other)
            {
                _id = other._id;
                Name = other.Name;
                Age = other.Age;
                GPA = other.GPA;
                Major = other.Major;
                PrivateNotes = other.PrivateNotes;
            }

            // методы
            private void AddPrivateNotes(string notes)
            {
                PrivateNotes = notes;
                Console.WriteLine("Приватная запись добавлена");
            }

            public void DisplayInfo()
            {
                Console.WriteLine($"ID: {_id}");
                Console.WriteLine($"Имя: {Name}");
                Console.WriteLine($"Возраст: {Age}");
                Console.WriteLine($"Средняя оценка: {GPA}");
                Console.WriteLine($"Направление: {Major}");
                Console.WriteLine($"Записи: {PrivateNotes}");
            }

            protected void UpdateMajor(string newMajor)
            {
                Major = newMajor;
                Console.WriteLine("Направление обновлено.");
            }
        }

        static void Main(string[] args)
        {
            string fl = "------------------------------";
            Console.WriteLine("Объект через конструктор по умолчанию:\n" + fl);
            Student student1 = new Student();
            student1.DisplayInfo();
            Console.WriteLine(fl + "\n\n");

            Console.WriteLine("Объект через конструктор с параметрами:\n" + fl);
            Student student2 = new Student(
                1,
                "Алиса",
                20,
                3.8,
                "Компьютерная наушка",
                "Студент на бюджете"
            );
            student2.DisplayInfo();
            Console.WriteLine(fl + "\n\n");

            Console.WriteLine("Объект через копирующий конструктор:\n" + fl);
            Student student3 = new Student(student2);
            student3.DisplayInfo();
            Console.WriteLine(fl);
        }
    }
}
