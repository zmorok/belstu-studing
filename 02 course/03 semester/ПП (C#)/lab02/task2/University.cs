using System;
using System.Collections.Generic;

namespace task2
{
    internal class University : Organization
    {
        public List<Faculty> Faculties { get; set; } = new List<Faculty>();

        public University()
            : base() { }

        public University(string name, string shortName, string address)
            : base(name, shortName, address) { }

        public int AddFaculty(Faculty faculty)
        {
            Faculties.Add(faculty);
            return Faculties.Count;
        }

        public bool DelFaculty(int index)
        {
            if (index >= 0 && index < Faculties.Count)
            {
                Faculties.RemoveAt(index);
                return true;
            }
            return false;
        }

        public bool UpdFaculty(Faculty faculty)
        {
            for (int i = 0; i < Faculties.Count; i++)
            {
                if (Faculties[i].Id == faculty.Id)
                {
                    Faculties[i] = faculty;
                    return true;
                }
            }
            return false;
        }

        public List<Faculty> GetFaculties()
        {
            return Faculties;
        }

        public override void PrintInfo()
        {
            base.PrintInfo();
            Console.WriteLine("Факультеты:");
            foreach (var faculty in Faculties)
            {
                faculty.PrintInfo();
            }
        }
    }
}
