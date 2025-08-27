using System;
using task3;

internal class Task3
{
    static void Main(string[] args)
    {
        University university = new University(
            "Московский государственный университет",
            "МГУ",
            "Москва"
        );

        Faculty faculty1 = new Faculty("Факультет математики", "ФМ", "Москва");
        Faculty faculty2 = new Faculty("Факультет физики", "ФФ", "Москва");

        Department department1 = new Department(1, "Кафедра алгебры");
        Department department2 = new Department(2, "Кафедра математического анализа");
        Department department3 = new Department(3, "Кафедра теоретической физики");

        faculty1.AddDepartment(department1);
        faculty1.AddDepartment(department2);
        faculty2.AddDepartment(department3);

        university.AddFaculty(faculty1);
        university.AddFaculty(faculty2);

        JobTitle jobTitle1 = new JobTitle(1, "Профессор");
        JobTitle jobTitle2 = new JobTitle(2, "Доцент");

        university.AddJobTitle(jobTitle1);
        university.AddJobTitle(jobTitle2);

        JobVacancy vacancy1 = new JobVacancy(
            1,
            jobTitle1,
            "Вакансия профессора на кафедру алгебры"
        );
        JobVacancy vacancy2 = new JobVacancy(
            2,
            jobTitle2,
            "Вакансия доцента на кафедру математического анализа"
        );
        university.OpenJobVacancy(vacancy1);
        university.OpenJobVacancy(vacancy2);

        Console.WriteLine(university.PrintJobVacancies());

        Person person = new Person(1, "Иван Иванов");

        Employee employee = university.Recruit(vacancy1, person);
        if (employee != null)
        {
            Console.WriteLine($"{employee.Name} нанят на должность {employee.Title.Name}");
        }

        university.Dismiss(employee.Id, "По собственному желанию");

        university.PrintInfo();
    }
}
