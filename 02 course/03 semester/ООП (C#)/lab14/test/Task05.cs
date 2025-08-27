using System;
using System.Threading;

namespace test
{
    internal partial class Program
    {
        private static Timer timer;

        private static void Task05()
        {
            TimerCallback callback = new(WhatTimeIsIt);
            timer = new(callback, null, 0, 1000);

            static void WhatTimeIsIt(object obj) =>
                Console.WriteLine(
                    $"ВРЕМЯ - {DateTime.Now.Hour}:{DateTime.Now.Minute}:{(DateTime.Now.Second >= 10 ? DateTime.Now.Second : "0" + DateTime.Now.Second)}"
                );
            Console.ReadLine();
            timer.Dispose();
        }
    }
}
