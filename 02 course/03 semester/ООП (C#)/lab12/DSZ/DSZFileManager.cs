using System;
using System.IO;
using System.IO.Compression;

namespace DSZ
{
    public class DSZFileManager
    {
        public static void InspectDirectory(string path, string drive)
        {
            try
            {
                string dirInspect = $"{path}\\DSZInspect";
                Directory.CreateDirectory(dirInspect);

                string logFilePath = $"{dirInspect}\\dsZdIrInfo.tXT";
                using (StreamWriter writer = new(logFilePath, false))
                {
                    foreach (var file in Directory.GetFiles(drive))
                        writer.WriteLine(file);
                }

                File.Copy(logFilePath, $"{dirInspect}\\dszdirinfo_copy.txt");
                File.Delete(logFilePath);
            }
            catch (Exception ex)
            {
                Console.WriteLine(
                    "Ошибка в классе DSZFileManager, метод InspectDirectory:\n\t" + ex.Message
                );
            }
        }

        public static void CopyFilesWithExtension(
            string sourceDir,
            string destDir,
            string extension
        )
        {
            try
            {
                Directory.CreateDirectory(destDir);

                foreach (var filePath in Directory.GetFiles(sourceDir, $"*{extension}"))
                {
                    string fileName = Path.GetFileName(filePath);
                    File.Copy(filePath, Path.Combine(destDir, fileName), true);
                }

                Directory.Move(destDir, $"{destDir}_Inspect");
            }
            catch (Exception ex)
            {
                Console.WriteLine(
                    "Ошибка в классе DSZFileManager, метод CopyFilesWithExtension:\n\t" + ex.Message
                );
            }
        }

        public static void ArchiveAndExtract(
            string sourceDir,
            string archivePath,
            string extractPath
        )
        {
            try
            {
                string archiveDir = Path.GetDirectoryName(archivePath);
                if (!Directory.Exists(archiveDir))
                {
                    Directory.CreateDirectory(archiveDir);
                }

                if (!File.Exists(archivePath))
                {
                    ZipFile.CreateFromDirectory(sourceDir, archivePath);
                }
               
                if (!Directory.Exists(extractPath))
                {
                    Directory.CreateDirectory(extractPath);
                }

                ZipFile.ExtractToDirectory(archivePath, extractPath);
            }
            catch (Exception ex)
            {
                Console.WriteLine(
                    "Ошибка в классе DSZFileManager, метод ArchiveAndExtract:\n\t" + ex.Message
                );
            }
        }
    }
}
