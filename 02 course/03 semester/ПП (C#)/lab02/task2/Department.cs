using System;

namespace task2
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
            Console.WriteLine($"\tКафедра: {Name} (ID: {Id})");
        }
    }
}
