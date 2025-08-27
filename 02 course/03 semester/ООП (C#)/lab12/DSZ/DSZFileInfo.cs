using System;
using System.IO;
using System.Linq;

namespace DSZ
{
    public class DSZFileInfo
    {
        public static void DisplayFileInfo(string filePath)
        {
            try
            {
                FileInfo fileInfo = new(filePath);

                if (fileInfo.Exists)
                {
                    Console.WriteLine(string.Join("", Enumerable.Repeat('+', 10)));
                    Console.WriteLine($"Полный путь: {fileInfo.FullName}");
                    Console.WriteLine($"Размер в байтах: {fileInfo.Length}");
                    Console.WriteLine($"Расширение: {fileInfo.Extension}");
                    Console.WriteLine($"Дата создания: {fileInfo.CreationTime}");
                    Console.WriteLine($"Последнее изменение: {fileInfo.LastWriteTime}");
                    Console.WriteLine(string.Join("", Enumerable.Repeat('+', 10)));
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine("Ошибка в классе DSZFileInfo: " + ex.Message);
            }
        }
    }
}
