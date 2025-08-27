using System;
using System.Collections.Generic;

namespace task1
{
    internal class University
    {
        private int id;
        private string name;
        private string shortName;
        private string address;
        private DateTime timeStamp;
        private List<Faculty> faculties = new List<Faculty>();

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

        public University() { }

        public University(string name, string shortName, string address)
        {
            this.name = name;
            this.shortName = shortName;
            this.address = address;
            this.timeStamp = DateTime.Now;
        }

        public University(University university)
        {
            this.id = university.id;
            this.name = university.name;
            this.shortName = university.shortName;
            this.address = university.address;
            this.timeStamp = university.timeStamp;
        }

        public int AddFaculty(Faculty faculty)
        {
            faculties.Add(faculty);
            return faculties.Count;
        }

        public bool DelFaculty(int id)
        {
            var faculty = faculties.Find(f => f.Id == id);
            if (faculty != null)
            {
                faculties.Remove(faculty);
                return true;
            }
            return false;
        }

        public bool UpdFaculty(Faculty faculty)
        {
            var existingFaculty = faculties.Find(f => f.Id == faculty.Id);
            if (existingFaculty != null)
            {
                faculties.Remove(existingFaculty);
                faculties.Add(faculty);
                return true;
            }
            return false;
        }

        public List<Faculty> GetFaculties()
        {
            return faculties;
        }

        public void PrintInfo()
        {
            Console.WriteLine(
                $"\t\tУНИВЕРСИТЕТ: {name}\n\tКороткое название: {shortName}, Адрес: {address}\n\n"
            );
            Console.WriteLine("\t\t\t\tФакультеты:\n");
            foreach (var faculty in faculties)
            {
                faculty.PrintInfo();
            }
        }
    }
}
