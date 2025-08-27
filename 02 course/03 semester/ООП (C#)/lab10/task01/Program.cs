using System;
using System.Collections.Generic;
using System.Linq;

namespace task01
{
    internal class Program
    {
        static readonly string[] months =
            {
                "January",
                "February",
                "March",
                "April",
                "May",
                "June",
                "July",
                "August",
                "September",
                "October",
                "November",
                "December"
            };

        static void Main(string[] args)
        {
            Task01();
        }

        #region TASK 01
        static void Task01()
        {
            MonthWithLenN();
            Console.WriteLine("\n");
            MonthSummerOrWinter();
            Console.WriteLine("\n");
            MonthsAscending();
            Console.WriteLine("\n");
            MonthsWithU4();
            Console.WriteLine("\n");           
        }

        static void MonthWithLenN()
        {
            Console.Write("len - ? : ");
            int.TryParse(Console.ReadLine(), out int len);

            IEnumerable<string> monthsWithN =
                from month in months
                where month.Length == len
                select month;

            Console.Write(" - - Месяцы с длинной {0} :\n - - ", len);
            foreach (string month in monthsWithN) Console.Write(month + " ");
        }

        static void MonthSummerOrWinter()
        {
            var monthsSorW = months.Where( m => m == "June" || m == "July" || m == "August" ||
            m == "December" || m == "January" || m == "February").Select(m => m.ToString());

            Console.Write(" - - Месяцы ЛЕТА или ЗИМЫ:\n - - ");
            foreach (string month in monthsSorW) Console.Write(month + " ");

        }

        static void MonthsAscending()
        {
            var monthsAsc = 
                from month in months
                orderby month
                select month;

            Console.Write(" - - Месяцы в алфавитном порядке:\n - - ");
            foreach (string month in monthsAsc) Console.Write(month + " ");
        }

        static void MonthsWithU4()
        {
            var monthsU4 =
                from month in months
                where month.Contains("u") && month.Length > 3
                select month;

            Console.WriteLine($" - - Кол-во месяцев с буквой u и длинной >= 4 : {monthsU4.Count()}");
            //foreach (string m in monthsU4) Console.Write(m + " ");
        }

        #endregion
    }
}
