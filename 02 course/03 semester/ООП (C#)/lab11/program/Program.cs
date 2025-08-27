using System;
using System.Linq;
using System.IO;
using System.Reflection;


using ReflectorSpace;
using Tecnologies;
using var05;

class Program
{
    static void Main()
    {
        string pathName = typeof(Abiturient).Name + "Class.txt";

        Console.WriteLine("инфа о Abiturient");
        Reflector.WriteInfoToFile<Abiturient>("Abitur.txt");
        Reflector.Research<Abiturient>();
        Console.WriteLine(string.Join("", Enumerable.Repeat('#', 100)));

        Console.WriteLine("\n\nинфа о Product");
        Reflector.WriteInfoToFile<Product>("Product.txt");
        Reflector.Research<Product>();
        Console.WriteLine(string.Join("", Enumerable.Repeat('#', 100)));


        Console.WriteLine("\n\nинфа о String");
        Reflector.WriteInfoToFile<String>("String.txt");
        Reflector.Research<String>();
        Console.WriteLine(string.Join("", Enumerable.Repeat('#', 100)));
        
        // считывание имени класса, метода класса и его параметров с файла
        // а также вызов Invoke
        StreamReader sr = new StreamReader(pathName, false);

        string name = sr.ReadLine();
        string method = sr.ReadLine();
        string param = sr.ReadLine();
        object[] Params = new object[]{ param };

        Assembly currentAssembly = Assembly.GetExecutingAssembly();
        foreach (Type type in currentAssembly.GetTypes())
        {
            if (type.Name == name)
            {
                object obj = Activator.CreateInstance(type);
                Console.WriteLine(Reflector.InvokeMethod(obj, method, Params));
                sr.Close();
                break;
            }
        }
        
    }
}
