using System;
using System.Collections;
using System.Collections.Generic;

namespace StudentLib
{
    public class StudentCollection : IEnumerable<Student>
    {
        private readonly Queue<Student> students = new Queue<Student>();

        public void Add(Student student)
        {
            students.Enqueue(student);
            Console.WriteLine($"{student.Name} добавлен в конец.");
        }

        public void Remove()
        {
            if (students.Count > 0)
            {
                Student removedStudent = students.Dequeue();
                Console.WriteLine($"{removedStudent.Name} удалён из начала.");
            }
            else
            {
                Console.WriteLine("Очередь пуста.");
            }
        }

        public Student FindStudentById(int id)
        {
            foreach (var student in students)
            {
                if (student.Id == id)
                {
                    return student;
                }
            }
            return null;
        }

        public void PrintStudents()
        {
            if (students.Count == 0)
            {
                Console.WriteLine("Очередь пуста.");
            }
            else
            {
                foreach (var student in students)
                {
                    Console.WriteLine(student);
                }
            }
        }

        public IEnumerator<Student> GetEnumerator()
        {
            return students.GetEnumerator();
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            return GetEnumerator();
        }
    }
}
