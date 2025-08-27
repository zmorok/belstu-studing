using System;

namespace task3
{
    internal class Department
    {
        public int Id { get; set; }
        public string Name { get; set; }
        public Department(int id, string name)
        {
            Id = id;
            Name = name;
        }
        public void PrintInfo()
        {
            Console.WriteLine($"\t -Кафедра: {Name} (ID: {Id})");
        }
    }
}
