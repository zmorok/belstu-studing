using System;
using System.Diagnostics;
using System.IO;
using Tecnologies;

public class Program06
{
    static void Main(string[] args)
    {
        try
        {
            Console.WriteLine("\tобработка исключений\n");

            // --неверная инициализации
            try
            {
                Task1();
            }
            catch (ExceptionDataInit ex)
            {
                HandleException(ex);
            }

            // выход индекса за границы
            try
            {
                Task2();
            }
            catch (IndexOutOfRangeException ex)
            {
                HandleException(ex);
            }

            // нулевой указателя
            try
            {
                Task3();
            }
            catch (NullReferenceException ex)
            {
                HandleException(ex);
            }

            // --деление на ноль
            try
            {
                Task4();
            }
            catch (ExceptionDivision ex)
            {
                HandleException(ex);
            }

            // --отсутствие файла
            try
            {
                Task5();
            }
            catch (ExceptionFile ex)
            {
                HandleException(ex);
            }
        }
        catch (Exception ex)
        {
            Console.WriteLine("[Глобальный обработчик исключений]\n");
            HandleException(ex);
        }
        finally
        {
            Console.WriteLine("Программа завершила выполнение (finally блок).\n");
        }
    }

    #region обработка исключения
    static void HandleException(Exception ex)
    {
        Console.WriteLine($"[Исключение поймано] Тип: {ex.GetType()}");
        Console.WriteLine($"Сообщение: {ex.Message}");
        Console.WriteLine($"Метод: {ex.TargetSite}");
        Console.WriteLine($"Стек вызовов: {ex.StackTrace}\n");
    }
    #endregion

    #region функции с ошибками
    static void Task1()
    {
        double price = -100;
        if (price < 0)
            throw new ExceptionDataInit("Цена товара должна быть положительным числом.");
        Scaner sc = new Scaner(
            "Сканер",
            price,
            "Канон",
            600,
            TechType.Scaner,
            new Warranty(2, DateTime.Now)
        );
    }

    static void Task2()
    {
        Product[] pr = new Product[2]
        {
            new Scaner("Сканер", 100, "Канон", 600, TechType.Scaner, new Warranty(2, DateTime.Now)),
            new Scaner("Сканер", 100, "Канон", 600, TechType.Scaner, new Warranty(2, DateTime.Now))
        };
        Product pr3 = pr[3];
    }

    static void Task3()
    {
        Product pr = null;
        pr.ToString();
    }

    static void Task4()
    {
        int a = 3;
        int b = 0;
        Debug.Assert(b != 0, "b не должен быть равен нулю.");
        if (b == 0)
            throw new ExceptionDivision("Попытка деления на ноль при передаче в параметр.");

        Product product = new Scaner(
            "name",
            a / b,
            "w",
            4,
            TechType.Scaner,
            new Warranty(2, DateTime.Now)
        );
    }
    
    static void Task5()
    {
        string db = "database.mdf";
        if (!File.Exists(db))
            throw new ExceptionFile($"Файла {db} не существует.");
    }
    #endregion
}
