using System;
using System.Linq;

namespace var05
{
    public class Program
    {
        public static void Main()
        {
            var stud1 = new Abiturient(
                "Иванов",
                "Иван",
                "Иванович",
                "Адрес1",
                "3242343242",
                new[] { 5, 4, 3 }
            );
            var stud2 = new Abiturient(
                "Алексеев",
                "Алексей",
                "Алексеевич",
                "Адрес2",
                "363631345452",
                new[] { 2, 3, 4 }
            );

            Console.WriteLine(stud1);
            Console.WriteLine(
                $"|||\tСредняя оценка студента student1: {stud1.CalculateAverageGrade()}"
            );

            Console.WriteLine($"|||\tstudent1 соответствует student2: {stud1.Equals(stud2)}");

            Abiturient.PrintClassInfo();

            var studs = new[] { stud1, stud2 };

            Console.WriteLine("Студенты с неудовлетворительными оценками:");
            foreach (var stud in studs)
            {
                bool und = false;
                foreach (int mark in stud.Grades)
                {
                    if (mark < 3)
                    {
                        und = true;
                        break;
                    }
                }
                if (und)
                {
                    Console.WriteLine(stud);
                }
            }

            int downMark = 10;
            Console.WriteLine($"Студенты с общей оценкой больше, чем {downMark}:");
            foreach (var stud in studs)
            {
                if (stud.Grades.Sum() > downMark)
                {
                    Console.WriteLine(stud);
                }
            }

            var anonStud = new
            {
                stud1.Surname,
                stud1.Name,
                stud1.Address
            };
            Console.WriteLine(
                $"Аноним: Фамилия = {anonStud.Surname}, Имя = {anonStud.Name}, Адрес = {anonStud.Address}"
            );
        }
    }
}
