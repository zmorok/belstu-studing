using System;
using System.IO;
using System.Linq;

namespace DSZ
{
    public class DSZDiskInfo
    {
        public static void DisplayDiskInfo()
        {
            try
            {
                var drives = DriveInfo.GetDrives();
                foreach (var drive in drives)
                {
                    if (drive.IsReady)
                    {
                        Console.WriteLine(string.Join("", Enumerable.Repeat('-', 10)));
                        Console.WriteLine($"Имя диска: {drive.Name}");
                        Console.WriteLine($"Файловая система: {drive.DriveFormat}");
                        Console.WriteLine($"Доступное место: {drive.AvailableFreeSpace / (1024 * 1024 * 1024)} GB.");
                        Console.WriteLine($"Полный объём: {drive.TotalSize / (1024 * 1024 * 1024)} GB.");
                        Console.WriteLine($"Метка тома: {drive.VolumeLabel}");
                        Console.WriteLine(string.Join("", Enumerable.Repeat('-', 10)));
                    }
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine("Ошибка в классе DSZDiskInfo: " + ex.Message);
            }
        }
    }
}
