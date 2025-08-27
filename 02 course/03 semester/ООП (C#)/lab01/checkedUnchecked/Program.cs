using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace checkedUnchecked
{
    internal class Program
    {
        static void Main(string[] args)
        {
            // функция с блоком checked
            void CheckedOperation()
            {
                try
                {
                    checked
                    {
                        int maxInt = int.MaxValue;
                        Console.WriteLine($"Максимально значение (checked): {maxInt}");
                        maxInt += 1;  // Попытка переполнения
                        Console.WriteLine($"После переполнения (checked): {maxInt}\n\n");
                    }
                }
                catch (OverflowException ex)
                {
                    Console.WriteLine("Checked block: Исключение переполнения обнаружено.\n\n");
                }
            }

            // функция с блоком unchecked
            void UncheckedOperation()
            {
                try
                {
                    unchecked
                    {
                        int maxInt = int.MaxValue;
                        Console.WriteLine($"Максимальное значение (unchecked): {maxInt}");
                        maxInt += 1;
                        Console.WriteLine($"После переполнения (unchecked): {maxInt}\n\n");
                    }
                }
                catch (OverflowException ex)
                {
                    Console.WriteLine("Unchecked block: Исключение переполнения обнаружено.\n\n");
                }
            }

            CheckedOperation();
            UncheckedOperation();
        }
    }
}
