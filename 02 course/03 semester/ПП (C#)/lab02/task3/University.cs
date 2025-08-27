using System;
using System.Collections.Generic;

namespace task3
{
    internal class University : Organization, IStaff
    {
        public List<Faculty> Faculties { get; set; } = new List<Faculty>();
        private List<JobVacancy> jobVacancies = new List<JobVacancy>();
        private List<Employee> employees = new List<Employee>();
        private List<JobTitle> jobTitles = new List<JobTitle>();

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

        public List<JobVacancy> GetJobVacancies()
        {
            return jobVacancies;
        }

        public List<Employee> GetEmployees()
        {
            return employees;
        }

        public List<JobTitle> GetJobTitles()
        {
            return jobTitles;
        }

        public int AddJobTitle(JobTitle jobTitle)
        {
            jobTitles.Add(jobTitle);
            return jobTitles.Count;
        }

        public string PrintJobVacancies()
        {
            string result = "Вакансии:\n";
            foreach (var vacancy in jobVacancies)
            {
                result += $"{vacancy.Title.Name} - {vacancy.Description}\n";
            }
            return result;
        }

        public bool DelJobTitle(int id)
        {
            var jobTitle = jobTitles.Find(jt => jt.Id == id);
            if (jobTitle != null)
            {
                jobTitles.Remove(jobTitle);
                return true;
            }
            return false;
        }

        public bool OpenJobVacancy(JobVacancy jobVacancy)
        {
            jobVacancies.Add(jobVacancy);
            return true;
        }

        public bool CloseJobVacancy(int id)
        {
            var jobVacancy = jobVacancies.Find(jv => jv.Id == id);
            if (jobVacancy != null)
            {
                jobVacancy.IsOpen = false;
                return true;
            }
            return false;
        }

        public Employee Recruit(JobVacancy jobVacancy, Person person)
        {
            if (jobVacancy.IsOpen)
            {
                var employee = new Employee(person.Id, person.Name, jobVacancy.Title);
                employees.Add(employee);
                CloseJobVacancy(jobVacancy.Id);
                return employee;
            }
            return null;
        }

        public bool Dismiss(int id, string reason)
        {
            var employee = employees.Find(e => e.Id == id);
            if (employee != null)
            {
                employees.Remove(employee);
                Console.WriteLine($"!! {employee.Name} уволен. Причина: {reason}");
                return true;
            }
            return false;
        }
    }
}
