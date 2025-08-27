using System;
using System.Diagnostics;

namespace test
{
    internal partial class Program
    {
        private static void Task01()
        {
            var allProcesses = Process.GetProcesses();
            foreach (var process in allProcesses)
            {
                try {
                    Console.WriteLine("ID: {0},\tИмя: {1,-50}" +
                                  $"Приоритет: {process.BasePriority},\t" +
                                  $"Время начала: {process.StartTime},\t" +
                                  "Время обработки: {2, -20}" +
                                  $"Состояние: {process.Responding}", process.Id, process.ProcessName, process.TotalProcessorTime + ",");
                } catch {
                   
                }
            }
        }
    }
}
