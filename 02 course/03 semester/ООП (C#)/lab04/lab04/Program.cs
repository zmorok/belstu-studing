using System;

namespace lab04
{
    internal class Program
    {
        static void Main(string[] args)
        {
            Product[] products = new Product[]
            {
                new PrintDevice("Принтер", 150, "ХэПэ", 20),
                new Scaner("Сканер", 100, "Канон", 600),
                new Computer("ПК", 1200, "Делл", "I7", 16),
                new Tablet("Пад", 400, "Сяоме", 10.2)
            };

            Printer printer = new Printer();

            foreach (var product in products)
            {
                printer.IAmPrinting(product);
                Console.WriteLine();
            }

            foreach (var product in products)
            {
                if (product is PrintDevice)
                    Console.WriteLine($"{product.Name} - это PrintDevice.");

                var computer = product as Computer;
                if (computer is Computer)
                    Console.WriteLine($"У {computer.Name} есть ЦПУ: {computer.CPU}, ОЗУ: {computer.RAM} ГБ.");
            }

            Console.WriteLine("Вызов DoClone() у всех продуктов:");
            foreach (var product in products) { product.DoClone(); ((IProduct)product).DoClone(); } 

            Console.WriteLine(products[1].GetHashCode());
        }
    }
}
