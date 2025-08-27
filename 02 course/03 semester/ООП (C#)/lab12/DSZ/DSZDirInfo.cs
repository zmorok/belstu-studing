using System;
using System.IO;
using System.Linq;


namespace DSZ
{
    public class DSZDirInfo
    {
        public static void DisplayDirectoryInfo(string dirPath)
        {
            try
            {
                DirectoryInfo dirInfo = new(dirPath);

                if (dirInfo.Exists)
                {
                    Console.WriteLine(string.Join("", Enumerable.Repeat('*', 10)));
                    Console.WriteLine($"Количество файлов: {dirInfo.GetFiles().Length}");
                    Console.WriteLine($"Время создания: {dirInfo.CreationTime}");
                    Console.WriteLine($"Количество поддиректориев: {dirInfo.GetDirectories().Length}");

                    Console.WriteLine("Список родительских директориев:");
                    DirectoryInfo parentDir = dirInfo.Parent;
                    while (parentDir != null)
                    {
                        Console.WriteLine($"\tРодительский каталог: {parentDir.FullName}");
                        parentDir = parentDir.Parent;
                    }
                    Console.WriteLine(string.Join("", Enumerable.Repeat('*', 10)));
                }
            }
            catch (Exception ex) {
            Console.WriteLine("Ошибка в классе DSZDirInfo: " + ex.Message );
            }
            
        }
    }
}
