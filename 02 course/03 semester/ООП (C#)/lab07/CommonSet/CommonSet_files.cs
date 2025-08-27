using System;
using System.Collections.Generic;
using System.IO;
using System.Text.Json;

namespace CommonSet
{
    public partial class CommonSet<T> : ISet<T>
        where T : IComparable<T>
    {
        #region work with files

        public void SaveToFile(string filePath)
        {
            try
            {
                var json = JsonSerializer.Serialize(
                    elements,
                    new JsonSerializerOptions { WriteIndented = true }
                );
                File.WriteAllText(filePath, json);
                Console.WriteLine($"Данные успешно сохранены в файл: {filePath}");
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Ошибка при сохранении в файл: {ex.Message}");
            }
        }

        public void LoadFromFile(string filePath)
        {
            try
            {
                if (!File.Exists(filePath))
                {
                    Console.WriteLine($"Файл не найден: {filePath}");
                    return;
                }

                var json = File.ReadAllText(filePath);
                var items = JsonSerializer.Deserialize<HashSet<T>>(json);
                elements.Clear();
                if (items != null)
                {
                    foreach (var item in items)
                    {
                        elements.Add(item);
                    }
                }
                Console.WriteLine($"Данные успешно загружены из файла: {filePath}");
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Ошибка при загрузке из файла: {ex.Message}");
            }
        }

        #endregion
    }
}
