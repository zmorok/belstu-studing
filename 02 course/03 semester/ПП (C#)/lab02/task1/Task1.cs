using task1;

public class Task1
{
    public static void Main()
    {
        University university = new University(
            "Национальный университет",
            "НУ",
            "ул. Кальвар, 123"
        );

        Faculty scienceFaculty = new Faculty("Факультет ИТ", "ФИТ", "ул. Кальвар, 123-1");
        Faculty artsFaculty = new Faculty("Факультет Искусства", "ИЗО", "ул. Кальвар, 123-2");

        university.AddFaculty(scienceFaculty);
        university.AddFaculty(artsFaculty);

        Department physicsDepartment = new Department(
            "Отдел технологий",
            "ОТ",
            "Здание технологий"
        );
        Department networkDepartment = new Department("Отдел сетей", "ОС", "Здание сетей");

        scienceFaculty.AddDepartment(physicsDepartment);
        scienceFaculty.AddDepartment(networkDepartment);

        Department artDepartment = new Department("Отдел живописи", "Ож", "Здание живописи");

        artsFaculty.AddDepartment(artDepartment);

        university.PrintInfo();
    }
}
