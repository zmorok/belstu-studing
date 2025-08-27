using System;

namespace task2
{
    internal class Organization
    {
        public int Id { get; set; }
        public string Name { get; set; }
        public string ShortName { get; set; }
        public string Address { get; set; }
        public DateTime TimeStamp { get; set; }

        public Organization()
        {
            TimeStamp = DateTime.Now;
        }

        public Organization(string name, string shortName, string address)
        {
            Name = name;
            ShortName = shortName;
            Address = address;
            TimeStamp = DateTime.Now;
        }

        public virtual void PrintInfo()
        {
            Console.WriteLine($"Организация: {Name} ({ShortName})");
            Console.WriteLine($"Адрес: {Address}");
            Console.WriteLine($"Дата создания: {TimeStamp}");
        }
    }
}
