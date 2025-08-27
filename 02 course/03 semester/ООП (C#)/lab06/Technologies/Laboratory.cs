using System;
using System.Collections.Generic;
using Tecnologies;

public class Laboratory
{
    private readonly List<Technology> techList;

    public Laboratory()
    {
        techList = new List<Technology>();
    }

    public void AddTech(Technology tech)
    {
        techList.Add(tech);
    }

    public void RemoveTech(Technology tech)
    {
        techList.Remove(tech);
    }

    public Technology GetTech(int index) => techList[index];

    public List<Technology> GetAllTechs() => techList;

    public void DisplayTechs()
    {
        foreach (var tech in techList)
        {
            Console.WriteLine(tech.ToString());
        }
    }
}
