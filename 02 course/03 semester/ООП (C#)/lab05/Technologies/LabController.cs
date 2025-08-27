using System;
using System.Collections.Generic;
using System.Linq;
using Tecnologies;

#region ПЕРЕЧИСЛЕНИЕ И СТРУКТУРА
namespace Tecnologies
{
    public enum TechType
    {
        Computer,
        Tablet,
        Scaner,
        PrintDevice
    }

    public struct Warranty
    {
        public int Years;
        public DateTime StartDate;

        public Warranty(int years, DateTime startDate)
        {
            Years = years;
            StartDate = startDate;
        }

        public DateTime ExpirationDate => StartDate.AddYears(Years);
    }
}
#endregion

public class LabController
{
    private readonly Laboratory container;

    public LabController()
    {
        container = new Laboratory();
    }

    public void AddTech(Technology tech)
    {
        container.AddTech(tech);
    }

    public void DisplayTechs()
    {
        container.DisplayTechs();
    }

    public void FindOldTech(int years)
    {
        foreach (var tech in container.GetAllTechs())
        {
            int techYears = tech.Warranty.ExpirationDate.Year - tech.Warranty.StartDate.Year;
            if (techYears > years)
            {
                Console.WriteLine($" -- Старая техника (Срок службы: {techYears}):\n{tech}");
            }
        }
    }

    public void CountTechTypes()
    {
        int computerCount = 0;
        int tabletCount = 0;
        int scanerCount = 0;
        int printDeviceCount = 0;

        foreach (var tech in container.GetAllTechs())
        {
            switch (tech.Type)
            {
                case TechType.Computer:
                    computerCount++;
                    break;
                case TechType.Tablet:
                    tabletCount++;
                    break;
                case TechType.Scaner:
                    scanerCount++;
                    break;
                case TechType.PrintDevice:
                    printDeviceCount++;
                    break;
            }
        }

        Console.WriteLine(
            $" - Компьютеры: {computerCount} \n - Планшеты: {tabletCount} \n - Сканеры: {scanerCount} \n - Принтеры: {printDeviceCount}"
        );
    }

    public void SortAndDisplayByPrice()
    {
        var sortedTechs = container.GetAllTechs().OrderByDescending(t => t.Price).ToList();
        foreach (var tech in sortedTechs)
        {
            Console.WriteLine(tech.ToString());
        }
    }
}
