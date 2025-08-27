using System.Collections.Generic;

namespace task1
{
    internal interface IStaff
    {
        List<JobVacancy> GetJobVacancies();
        List<Employee> GetEmployees();
        List<JobTitle> GetJobTitles();
        int AddJobTitle(JobTitle jobTitle);
        string PrintJobVacancies();
        bool DelJobTitle(int jobTitleId);
        JobVacancy OpenJobVacancy();
        bool CloseJobVacancy(int vacancyId);
        Employee Recruit(JobVacancy jobVacancy, Person person);
        bool Dismiss(int employeeId, Reason reason);
    }

    public class JobVacancy
    {
        public int VacancyId { get; set; }
        public string Title { get; set; }
        public bool IsOpen { get; set; }
    }

    public class Employee
    {
        public int EmployeeId { get; set; }
        public string Name { get; set; }
    }

    public class JobTitle
    {
        public int JobTitleId { get; set; }
        public string Title { get; set; }
    }

    public class Person
    {
        public string FirstName { get; set; }
        public string LastName { get; set; }
    }

    public class Reason
    {
        public string Description { get; set; }
    }
}
