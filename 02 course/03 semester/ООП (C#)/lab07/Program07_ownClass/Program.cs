using System;
using System.Collections.Generic;
using CommonSet;
using lab04;

namespace Program07_ownClass
{
    internal class Program
    {
        static void Main(string[] args)
        {
            var productSet = new CommonSet<Technology>();
            productSet.Add(new Computer("компуктер", 120.32d, "хп", "мощное", 16));
            productSet.Add(new Tablet("планшет", 120.32d, "хп2", 160));
            productSet.Add(new PrintDevice("принтер", 120.32d, "хп52", 52));

            List<Technology> list =
                (List<Technology>)productSet.Find(x => x.GetType().Name == "Computer");
            Console.WriteLine(string.Join(", ", list));

            Console.WriteLine("ВСЯ ТЕХНИКА:"); 
            Console.WriteLine(productSet);
        }
    }
}
