using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Reflection;

namespace ReflectorSpace
{
    public static class Reflector
    {
        
        public static void Research<T>()
        {
            Type type = typeof(T);

            Console.WriteLine($"Имя сборки: {GetAssemblyName(type)}");
            Console.WriteLine($"Есть публичные конструкторы?: {HasPublicConstructors(type)}");

            Console.WriteLine("\nПубличные методы:");
            foreach (var method in GetPublicMethods(type)) Console.WriteLine(method);
            
            Console.WriteLine("\nПоля и свойства:");
            foreach (var item in GetFieldsAndProperties(type)) Console.WriteLine(item);

            Console.WriteLine("\nРеализованные интерфейсы:");
            foreach (var iFace in GetImplementedInterfaces(type)) Console.WriteLine(iFace);

            Console.WriteLine("\nМетоды, принимающие параметры типа string:");
            foreach (var methodName in GetMethodsByParameterString(type))
            {
                Console.WriteLine(methodName);
            }
        }


        // a. имя сборки, в которой определен класс
        private static string GetAssemblyName(Type type)
        {   
            if (type == null)
                return "Класс не найден.";
            return type.Assembly.FullName;
        }

        // b. есть ли публичные конструкторы?
        private static bool HasPublicConstructors(Type type)
        {
            if (type == null)
                return false;
            return type.GetConstructors(BindingFlags.Public).Any();
        }

        // c. все общедоступные публичные методы класса
        private static IEnumerable<string> GetPublicMethods(Type type)
        {
            if (type is null)
                return Enumerable.Empty<string>();

            return type.GetMethods(BindingFlags.Public | BindingFlags.Instance | BindingFlags.Static).Select(m => $"Публичный метод {m.Name}");
        }

        // d. информация о полях и свойствах класса
        private static IEnumerable<string> GetFieldsAndProperties(Type type)
        {
            if (type is null)
                return Enumerable.Empty<string>();

            var fields = type.GetFields(BindingFlags.Public | BindingFlags.Instance | BindingFlags.Static).Select(f => $"поле: {f.Name}");

            var properties = type.GetProperties(BindingFlags.Public | BindingFlags.Instance | BindingFlags.Static).Select(p => $"свойство: {p.Name}");

            return fields.Concat(properties);
        }

        // e. все реализованные классом интерфейсы
        private static IEnumerable<string> GetImplementedInterfaces(Type type)
        {
            if (type is null)
                return Enumerable.Empty<string>();
            return type.GetInterfaces().Select(i => i.Name);
        }

        // f. имена методов, которые содержат заданный тип параметра
        private static IEnumerable<string> GetMethodsByParameterString(Type type)
        {
            if (type == null)
                return Enumerable.Empty<string>();

            return type.GetMethods()
                .Where(m => m.GetParameters().Any(p => p.ParameterType == typeof(string)))
                .Select(m => m.Name);
        }

        // g. Invoke для вызова метода с аргументами
        public static object InvokeMethod(object obj, string methodName, object[] pae)
        {
            Type type = obj.GetType();
            MethodInfo method = type.GetMethod(methodName);
            return method == null
                ? throw new ArgumentException($"Метод {methodName} не обнаружен в типе {type.Name}.")
                : method.Invoke(obj, pae);
        }

        // запись инфы в файл
        public static void WriteInfoToFile<T>(string filePath)
        {
            Type type = typeof(T);
            using (StreamWriter writer = new StreamWriter(filePath, false))
            {
                writer.WriteLine($"Имя сборки: {GetAssemblyName(type)}");
                writer.WriteLine($"Есть публичные конструкторы?: {HasPublicConstructors(type)}");

                writer.WriteLine("\nПУБЛИЧНЫЕ МЕТОДЫ:");
                foreach (var method in GetPublicMethods(type)) writer.WriteLine($"+ {method}");

                writer.WriteLine("\nПОЛЯ И СВОЙСТВА:");
                foreach (var item in GetFieldsAndProperties(type)) writer.WriteLine($"- {item}");

                writer.WriteLine("\nРЕАЛИЗОВАННЫЕ ИНТЕРФЕЙСЫ:");
                foreach (var iFace in GetImplementedInterfaces(type)) writer.WriteLine($"- {iFace}");

                writer.Close();
            }

        }

        public static T Create<T>() where T : new()
        {
            return new T();
        }
    }
}
