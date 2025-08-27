using System;
using System.IO;
using System.Linq;

namespace DSZ
{
    public static class DSZLog
    {
        private static readonly string logFilePath = "DSZlogfile.txt";

        public static void LogAction(string action, string details)
        {
            using StreamWriter writer = new(logFilePath, true);
            writer.WriteLine($"{DateTime.Now} - Действие: {action}, Подробности: {details}");
        }

        public static string[] ReadLog() => File.ReadAllLines(logFilePath);

        public static string[] SearchLog(string keyword) => File.ReadAllLines(logFilePath).Where(line => line.Contains(keyword)).ToArray();
        
        //***************
        public static string[] SearchByDate(DateTime startDate)
        {
            DateTime endDate = DateTime.Now;

            return File.ReadAllLines(logFilePath)
                .Where(line =>
                    DateTime.TryParse(line.Substring(0, 19), out DateTime logDate)
                    && logDate >= startDate
                    && logDate <= endDate
                )
                .ToArray();
        }
    }
}
