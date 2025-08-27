using System;
using System.Collections.Generic;

namespace task2
{
    internal class Faculty : Organization
    {
        public List<Department> Departments { get; set; } = new List<Department>();
        public Faculty() : base() { }
        public Faculty(string name, string shortName, string address)
            : base(name, shortName, address) { }
        public int AddDepartment(Department department)
        {
            Departments.Add(department);
            return Departments.Count;
        }
        public bool DelDepartment(int index)
        {
            if (index >= 0 && index < Departments.Count)
            {
                Departments.RemoveAt(index);
                return true;
            }
            return false;
        }
        public bool UpdDepartment(Department department)
        {
            for (int i = 0; i < Departments.Count; i++)
            {
                if (Departments[i].Id == department.Id)
                {
                    Departments[i] = department;
                    return true;
                }
            }
            return false;
        }
        public List<Department> GetDepartments()
        {
            return Departments;
        }
        public override void PrintInfo()
        {
            base.PrintInfo();
            Console.WriteLine("Кафедры:");
            foreach (var department in Departments)
            {
                department.PrintInfo();
            }
        }
    }
}
