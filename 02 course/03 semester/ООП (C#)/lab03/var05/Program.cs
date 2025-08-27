using System;
using System.Collections.Generic;
using System.Linq;

namespace var05
{
    public class Set
    {
        private HashSet<string> elements;
        public HashSet<string> Elements
        {
            get => elements;
            set { }
        }

        public override string ToString() 
        { 
            return string.Join(", ", this.SortSet()); 
        }

        public Set()
        {
            elements = new HashSet<string>();
        }

        public Set(IEnumerable<string> initialElements)
        {
            elements = new HashSet<string>(initialElements);
        }

        public class Production
        {
            private int _id;
            private string _name;
            public int Id { get => _id; set => _id = value; }
            public string OrganizationName { get => _name; set => _name = value; }

            public Production(int id, string organizationName)
            {
                Id = id;
                OrganizationName = organizationName;
            }

            public override string ToString() => 
                $"----------\nID: {Id}\nНазвание организации: {OrganizationName}\n----------\n";
            
        }

        public class Developer
        {
            private int _id;
            private string _name;
            private string _department;
            public string FullName { get => _name; set => _name = value; }
            public int Id { get => _id; set => _id = value; }
            public string Department { get => _department; set => _department = value; }

            public Developer(string fullName, int id, string department)
            {
                FullName = fullName;
                Id = id;
                Department = department;
            }

            public override string ToString() => 
                $"----------\nID: {Id}\nИмя: {FullName}\nДолжность: {Department}\n----------\n";
        }

        public string this[int index]
        {
            get => elements.ElementAt(index);
            set
            {
                if (index >= 0 && index < elements.Count)
                {
                    elements.Remove(elements.ElementAt(index));
                    elements.Add(value);
                }
            }
        }

        public static Set operator <<(Set set, string item)
        {
            set.elements.Add(item);
            return set;
        }

        public static Set operator >>(Set set, string item)
        {
            set.elements.Remove(item);
            return set;
        }

        public static bool operator >(Set set1, Set set2) =>
            set2.elements.IsSubsetOf(set1.elements);

        public static bool operator <(Set set1, Set set2) =>
            !set2.elements.IsSubsetOf(set1.elements);

        public static bool operator ==(Set set1, Set set2) =>
            set1.elements.SetEquals(set2.elements);

        public static bool operator !=(Set set1, Set set2) =>
            !set1.elements.SetEquals(set2.elements);

        public static Set operator %(Set set1, Set set2) =>
            new(set1.elements.Intersect(set2.elements));

        public static class StatisticOperation
        {
            public static int Count(Set set) => set.elements.Count;

            public static int Difference(Set set) =>
                set.elements.Count > 0
                    ? set.elements.Max(s => s.Length) - set.elements.Min(s => s.Length)
                    : 0;

            public static int Sum(Set set) => set.elements.Count;
        }
    }

    public static class SetExtensions
    {
        public static string ShortestWord(this Set set) =>
            set.Elements.OrderBy(s => s.Length).FirstOrDefault();

        public static IEnumerable<string> SortSet(this Set set) => set.Elements.OrderBy(s => s);
    }

    public static class StringExtensions
    {
        public static string Reversed(this string str)
        {
            char[] arr = str.ToCharArray();
            Array.Reverse(arr);
            return new string(arr);
        }
    }

    public class Program
    {
        public static void Main(string[] args)
        {
            Set mySet = new Set();
            _ = mySet << "banana" << "cherry" << "apple" << "d";

            var production = new Set.Production(1, "Орг.");
            Console.WriteLine(production);

            var developer = new Set.Developer("Вовчик Старый", 123, "Разработчик");
            Console.WriteLine(developer);

            mySet <<= "Абоба";
            mySet >>= "banana";
            Console.WriteLine($"Содержимое множества: {mySet}");
            Console.WriteLine($"Короткое слово: {mySet.ShortestWord()}");
            Console.WriteLine($"Количество элементов: {Set.StatisticOperation.Count(mySet)}");

            var anotherSet = new Set(["banana", "date", "Абоба" ]);
            var intersection = mySet % anotherSet;
            Console.WriteLine($"Пересечение: {intersection}");
            Console.WriteLine(Set.StatisticOperation.Difference(mySet));

            Console.WriteLine(mySet < intersection);
            Console.WriteLine(mySet.ShortestWord());

            Console.WriteLine(mySet[0].Reversed());
        }
    }
}
