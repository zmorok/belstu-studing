using System;
using System.Linq;

class Program
{
    static void Main(string[] args)
    {
        string str = "        Пример строки/  С лишними   пробелами.           ";

        Func<string, string> process = s => s.Replace(",", " ").Replace("/", " ");
        process += s => s.ToUpper();
        process += s => s.Trim();
        process += s => s.Replace(" ", "_");
        process += s => "Prefix_" + s;

        foreach (Func<string, string> func in process.GetInvocationList())
        {
            Console.WriteLine("input before: " + str);
            str = func(str);
            Console.WriteLine($"input after: " + str);
            Console.WriteLine(string.Join("", Enumerable.Repeat("#", 10)));
        }

        Console.WriteLine($"\n\tРезультат обработки: {str}\n");
    }
}
