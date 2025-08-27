using System;
using System.Collections.Generic;

namespace task1
{
    internal class Faculty
    {
        private int id;
        private string name;
        private string shortName;
        private string address;
        private DateTime timeStamp;
        private List<Department> departments = new List<Department>();

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

        public Faculty() { }

        public Faculty(string name, string shortName, string address)
        {
            this.name = name;
            this.shortName = shortName;
            this.address = address;
            this.timeStamp = DateTime.Now;
        }

        public Faculty(Faculty faculty)
        {
            this.id = faculty.id;
            this.name = faculty.name;
            this.shortName = faculty.shortName;
            this.address = faculty.address;
            this.timeStamp = faculty.timeStamp;
        }

        public int AddDepartment(Department department)
        {
            departments.Add(department);
            return departments.Count;
        }

        public bool DelDepartment(int id)
        {
            var department = departments.Find(d => d.Id == id);
            if (department != null)
            {
                departments.Remove(department);
                return true;
            }
            return false;
        }

        public List<Department> GetDepartments()
        {
            return departments;
        }

        public void PrintInfo()
        {
            Console.WriteLine(
                $" - Факультет: {name}, Короткое название: {shortName}, Адрес: {address}"
            );
            Console.WriteLine(" Отделы:");
            foreach (var department in departments)
            {
                department.PrintInfo();
            }
            Console.WriteLine();
        }
    }
}
