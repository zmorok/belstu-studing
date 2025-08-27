using System;
using System.Collections.Generic;
using System.Linq;

namespace DifferentCollections
{
    internal class Program
    {
        static void Main(string[] args)
        {
            // a. вывод первой коллекции
            Queue<int> queue = new Queue<int>();
            queue.Enqueue(10);
            queue.Enqueue(20);
            queue.Enqueue(30);
            queue.Enqueue(40);

            Console.WriteLine("Первая коллекция (Queue):");
            foreach (int item in queue) Console.Write(item + " ");

            // b. удаление n последовательных элементов 
            Console.Write($"\n\nВведите кол-во начальных последовательных элементов для удаления (макс: {queue.Count}): ");
            if (int.TryParse(Console.ReadLine(), out int n)) for (int i = 0; i < n; i++) queue.Dequeue();
            else Console.WriteLine("Неверное значение. Пропуск задания.");

            // c. добавление новых элементов
            queue.Enqueue(50);
            queue.Enqueue(60);

            Console.WriteLine("\nПервая коллекция после удаления и добавления:");
            foreach (int item in queue) Console.Write(item + " ");
            
            // d. cоздание второй коллекции
            Dictionary<int, int> dictionary = new Dictionary<int, int>();
            int key = 0;
            foreach (int item in queue) dictionary.Add(++key, item);

            // e. вывод второй коллекции
            Console.WriteLine("\n\nВторая коллекция (Dictionary):");
            foreach (var item in dictionary) Console.Write(item.Value + " ");

            // f. поиск значения во второй коллекции
            Console.Write("\nВведите значение для поиска во второй коллекции: ");
            if (int.TryParse(Console.ReadLine(), out int searchValue))
            {
                if (dictionary.ContainsValue(searchValue))
                {
                    Console.WriteLine($"\nЗначение {searchValue} найдено в второй коллекции.");
                }
                else
                {
                    Console.WriteLine($"\nЗначение {searchValue} не найдено в второй коллекции.");
                }
            }
            else Console.WriteLine("\nНеверное значение. Пропуск задания.");
        }
    }
}
