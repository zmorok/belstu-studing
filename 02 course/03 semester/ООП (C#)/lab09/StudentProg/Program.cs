using System;
using StudentLib;

namespace StudentProg
{
    internal class Program
    {
        static void Main(string[] args)
        {
            StudentCollection students = new StudentCollection();

            students.Add(new Student("Сергей Сергеевич Сергеев", "Программирование"));
            students.Add(new Student("Анастасия Владимировна Пасюк", "Математика"));
            students.Add(new Student("Бондан Владиславович Анисев", "Физика"));

            Console.WriteLine("\nВсе студенты:");
            students.PrintStudents();

            Console.WriteLine("\nПоиск студента с ID 2:");
            Student foundStudent = students.FindStudentById(2);
            Console.WriteLine(foundStudent != null ? foundStudent : "Студент не найден.");
            
            Console.WriteLine("\nУдаление студента:");
            students.Remove();

            Console.WriteLine("\nВсе студенты после удаления:");
            students.PrintStudents();
        }
    }
}
