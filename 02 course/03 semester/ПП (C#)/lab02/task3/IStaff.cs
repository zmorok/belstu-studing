using System.Collections.Generic;

namespace task3
{
    internal interface IStaff
    {
        List<JobVacancy> GetJobVacancies();
        List<Employee> GetEmployees();
        List<JobTitle> GetJobTitles();
        int AddJobTitle(JobTitle jobTitle);
        string PrintJobVacancies();
        bool DelJobTitle(int id);
        bool OpenJobVacancy(JobVacancy jobVacancy);
        bool CloseJobVacancy(int id);
        Employee Recruit(JobVacancy jobVacancy, Person person);
        bool Dismiss(int id, string reason);
    }

    public class JobTitle
    {
        public int Id { get; set; }
        public string Name { get; set; }

        public JobTitle(int id, string name)
        {
            Id = id;
            Name = name;
        }
    }

    public class Employee
    {
        public int Id { get; set; }
        public string Name { get; set; }
        public JobTitle Title { get; set; }

        public Employee(int id, string name, JobTitle title)
        {
            Id = id;
            Name = name;
            Title = title;
        }
    }

    public class JobVacancy
    {
        public int Id { get; set; }
        public JobTitle Title { get; set; }
        public string Description { get; set; }
        public bool IsOpen { get; set; }

        public JobVacancy(int id, JobTitle title, string description)
        {
            Id = id;
            Title = title;
            Description = description;
            IsOpen = true;
        }
    }

    public class Person
    {
        public int Id { get; set; }
        public string Name { get; set; }

        public Person(int id, string name)
        {
            Id = id;
            Name = name;
        }
    }
}
