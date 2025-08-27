using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace arrays
{
    internal class Program
    {
        static void Main(string[] args)
        {
            // двумерный массив
            int[,] matrix =
            {
                { 1, 2, 6 },
                { 7, 8, 4 },
                { 1, 3, 5 },
                { 2, 4, 6 },
                { 3, 5, 7 },
            };
            Console.WriteLine("Двумерный массив:");
            for (int i = 0; i < matrix.GetLength(0); i++)
            {
                for (int j = 0; j < matrix.GetLength(1); j++)
                {
                    Console.Write($"[{matrix[i, j]}]");
                }
                Console.Write('\n');
            }

            // одномерный массив
            string[] array = { "12asd3das", "dadasd", "dasdad", "adsada" };
            Console.Write("\nОдномерный массив: ");
            foreach (string i in array)
            {
                Console.Write($"{i}, ");
            }
            Console.WriteLine($"\nДлинна массива array = {array.Length}");

            Console.Write("Выберите позицию элемента для замены: ");
            int elem = Convert.ToInt32(Console.ReadLine()) - 1;

            if (elem >= 0 && elem < array.Length)
            {
                Console.Write("Введите значение: ");
                string value = Console.ReadLine();

                array[elem] = value;

                Console.Write("Новый одномерный массив: ");
                for (int i = 0; i < array.Length; i++)
                {
                    if (i > 0)
                        Console.Write(", ");
                    Console.Write(array[i]);
                }
                Console.WriteLine("\n\n");
            }
            else
            {
                Console.WriteLine("Ошибка: Позиция элемента вне границ массива.");
            }

            // ступенчатый массив с 3 строками
            double[][] jaggedArray = new double[3][];

            for (int i = 0; i < 3; i++)
            {
                jaggedArray[i] = new double[i + 2];
            }

            // ввод значений с консоли
            for (int i = 0; i < jaggedArray.Length; i++)
            {
                for (int j = 0; j < jaggedArray[i].Length; j++)
                {
                    Console.Write($"Введите значение для элемента [{i}][{j}]: ");
                    jaggedArray[i][j] = Convert.ToDouble(Console.ReadLine());
                }
            }

            // вывод значений массива
            Console.WriteLine("\nВывод ступенчатого массива:");
            for (int i = 0; i < jaggedArray.Length; i++)
            {
                for (int j = 0; j < jaggedArray[i].Length; j++)
                {
                    Console.Write($"{jaggedArray[i][j]} ");
                }
                Console.WriteLine();
            }

            // неявнотипизированные переменные для строки и массива
            var untypedForStr = "strVar";
            var arrS = new[] { "dasd", "adsa", "asda" };
        }
    }
}
