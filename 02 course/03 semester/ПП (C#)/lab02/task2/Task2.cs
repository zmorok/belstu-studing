using task2;

class Task2
{
    static void Main()
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

        university.PrintInfo();
    }
}
