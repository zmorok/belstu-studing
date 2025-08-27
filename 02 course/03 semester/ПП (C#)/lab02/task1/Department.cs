using System;

namespace task1
{
    internal class Department
    {
        private int id;
        private string name;
        private string shortName;
        private string address;
        private DateTime timeStamp;

        public int Id
        {
            get { return id; }
            set { id = value; }
        }

        public string Name
        {
            get { return name; }
            set { name = value; }
        }

        public string ShortName
        {
            get { return shortName; }
            set { shortName = value; }
        }

        public string Address
        {
            get { return address; }
            set { address = value; }
        }

        public DateTime TimeStamp
        {
            get { return timeStamp; }
            set { timeStamp = value; }
        }

        public Department() { }

        public Department(string name, string shortName, string address)
        {
            this.name = name;
            this.shortName = shortName;
            this.address = address;
            this.timeStamp = DateTime.Now;
        }

        public Department(Department department)
        {
            this.id = department.id;
            this.name = department.name;
            this.shortName = department.shortName;
            this.address = department.address;
            this.timeStamp = department.timeStamp;
        }

        public void PrintInfo()
        {
            Console.WriteLine(
                $"\t-- Название отдела: {name}, Короткое название: {shortName}, Адрес: {address}"
            );
        }
    }
}
