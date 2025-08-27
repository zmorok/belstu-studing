using System;
using System.IO;
using DSZ;

class Program
{
    static void Main()
    {
        string path = Directory.GetCurrentDirectory() + "\\INSPECTION";
        try
        {
            if (Directory.Exists(path))
            {
                Console.WriteLine("Директория INSPECTION существует, проверьте её.");
                return;
            }

            Directory.CreateDirectory(path);
            Console.WriteLine("Директория INSPECTION создана.");
        }
        catch (Exception e)
        {
            Console.WriteLine("Процесс закончился с ошибкой: {0}", e.ToString());
        }
        finally 
        {
            Directory.SetCurrentDirectory(path);
            // DSZLog
            Console.WriteLine("=== Логирование действий ===");
            DSZLog.LogAction("Запуск программы", "Начальная инициализация программы");
            DSZLog.LogAction("Выполнение теста", "Запущены тестовые методы для проверки работы");

            Console.WriteLine("=== Чтение всех логов ===");
            string[] allLogs = DSZLog.ReadLog();
            foreach (var log in allLogs) Console.WriteLine(log);

            Console.WriteLine("\n=== Поиск по ключевому слову 'тест' ===");
            string[] keywordLogs = DSZLog.SearchLog("тест");
            foreach (var log in keywordLogs) Console.WriteLine(log);

            Console.WriteLine("\n=== Поиск логов за диапазон дат ===");
            DateTime startDate = DateTime.Today.AddDays(-1);
            string[] dateRangeLogs = DSZLog.SearchByDate(startDate);
            foreach (var log in dateRangeLogs) Console.WriteLine(log);

            Console.WriteLine($"\nКоличество записей в логе: {allLogs.Length}");

            // записи за текущий час
            Console.WriteLine("\n=== Оставляем записи только за текущий час ===");
            DateTime currentHour = DateTime.Now.Date.AddHours(DateTime.Now.Hour);
            var currentHourLogs = DSZLog.SearchByDate(currentHour);
            File.WriteAllLines("DSZlogfile.txt", currentHourLogs);
            Console.WriteLine("Лог обновлен, оставлены записи только за текущий час.\n");
            
            // DSZDirInfo
            Console.WriteLine("=== Информация о каталоге ===");
            string pathParrent = Directory.GetParent(Directory.GetCurrentDirectory()).FullName;
            DSZDirInfo.DisplayDirectoryInfo(pathParrent);

            // DSZDiskInfo
            Console.WriteLine("\n=== Информация о дисках ===");
            DSZDiskInfo.DisplayDiskInfo();

            // DSZFileInfo
            Console.WriteLine("\n=== Информация о файле ===");
            DSZFileInfo.DisplayFileInfo("dSzLoGfiLe.tXt");

            // DSZFileManager
            Console.WriteLine("\n=== Обзор каталога ===");
            DSZFileManager.InspectDirectory(path, "D:\\");

            string source = Path.GetFullPath(Path.Combine(Directory.GetCurrentDirectory(), @"..\..\..\..\DSZ"));
            string root = Path.GetFullPath(Path.Combine(Directory.GetCurrentDirectory(), @"..\..\..\..\..\..\старис"));

            Console.WriteLine("\n=== Копирование файлов с расширением ===");
            DSZFileManager.CopyFilesWithExtension(
                source,
                path + "\\CS_FILES",
                ".cs"
            );

            Console.WriteLine("\n=== Архивирование и распаковка ===");
            DSZFileManager.ArchiveAndExtract(
                root,
                path + "\\Archive\\Archive.rar",
                path + "\\Archive\\ArchiveFiles\\"
            );

            Console.Write("\nНажмите на любую клавишу...");
            Console.ReadKey();
        }
    }
}