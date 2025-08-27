using System;

namespace task1
{
    internal class Organization
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

        public Organization() { }

        public Organization(string name, string shortName, string address)
        {
            this.name = name;
            this.shortName = shortName;
            this.address = address;
            this.timeStamp = DateTime.Now;
        }

        public Organization(Organization organization)
        {
            this.id = organization.id;
            this.name = organization.name;
            this.shortName = organization.shortName;
            this.address = organization.address;
            this.timeStamp = organization.timeStamp;
        }

        public void PrintInfo()
        {
            Console.WriteLine(
                $"Название: {name}, шортИмя: {shortName}, Адрес: {address}, Временная отметка: {timeStamp}"
            );
        }
    }
}
