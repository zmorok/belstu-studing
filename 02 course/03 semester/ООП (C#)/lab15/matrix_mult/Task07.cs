using System;
using System.Collections.Concurrent;
using System.Threading;
using System.Threading.Tasks;

namespace matrix_mult
{
    public class Task07
    {
        public static void ComputeTask07()
        {
            var strore = new BlockingCollection<string>(20);

            Task[] mfrs = new Task[5];
            for (int i = 0; i < mfrs.Length; i++)
            {
                int mfrId = i + 1;
                mfrs[i] = Task.Run(() => Mfr(mfrId, strore));
            }

            Task[] custs = new Task[10];
            for (int i = 0; i < custs.Length; i++)
            {
                Thread.Sleep(2000);
                int custId = i + 1;
                custs[i] = Task.Run(() => Cust(custId, strore));
            }

            Task.WaitAll(mfrs);
            Thread.Sleep(5000); // время на завершение
            Console.WriteLine("Все поставщики закончили работу, покупателей больше нет.");
        }

        static void Mfr(int mfrId, BlockingCollection<string> store)
        {
            Random random = new Random();
            string[] products = { "товар_1", "товар_2", "товар_3", "товар_4", "товар_5" };

            for (int i = 0; i < 15; i++)
            {
                Thread.Sleep(random.Next(100, 2000)); // разная скорость завоза
                string product = $"{products[random.Next(products.Length)]} от поставщика {mfrId}";

                if (store.TryAdd(product))
                {
                    Console.WriteLine($" + Поставщик {mfrId} завез: {product}");
                    StoreState(store);
                }
                else
                {
                    Console.WriteLine($"Склад заполнен! Поставщик {mfrId} ожидает...");
                    while (!store.TryAdd(product))
                    {
                        Thread.Sleep(100);
                    }
                    Console.WriteLine($"Поставщик {mfrId} добавил: {product} после ожидания.");
                    StoreState(store);
                }
            }
        }

        static void Cust(int custId, BlockingCollection<string> store)
        {
            Random random = new Random();

            while (true)
            {
                Thread.Sleep(random.Next(100, 500)); // разная скорость закупки

                if (store.TryTake(out string product))
                {
                    Console.WriteLine($"\t - Покупатель {custId} купил: {product}");
                    StoreState(store);
                }
                else
                {
                    Console.WriteLine($"Покупатель {custId} ушел, так как товаров нет.");
                    break;
                }
            }
        }

        static void StoreState(BlockingCollection<string> store)
        {
            Console.WriteLine($"Состояние склада: [{string.Join(", ", store.ToArray())}]");
        }
    }
}
