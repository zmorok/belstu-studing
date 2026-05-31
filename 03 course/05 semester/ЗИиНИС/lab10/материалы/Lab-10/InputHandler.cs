using System.Text;
namespace Lempel_Ziv_method;

// Класс для обработки пользовательского ввода
static class InputHandler
{
    public static string LoadSourceText()
    {
        Console.Write("Введите путь к файлу с исходным сообщением: ");
        string? filePath = Console.ReadLine();

        if (string.IsNullOrWhiteSpace(filePath))
        {
            filePath = Path.Combine(Directory.GetCurrentDirectory(), "text.txt");
        }

        if (!File.Exists(filePath))
        {
            Console.WriteLine($"Файл не найден: {filePath}");
            Console.Write("Введите текст вручную: ");
            return Console.ReadLine() ?? string.Empty;
        }

        try
        {
            return File.ReadAllText(filePath, Encoding.UTF8);
        }
        catch (Exception ex)
        {
            Console.WriteLine($"Ошибка чтения файла: {ex.Message}");
            return string.Empty;
        }
    }

    public static int ReadPositiveInteger()
    {
        while (true)
        {
            string? input = Console.ReadLine();
            if (int.TryParse(input, out int value) && value > 0)
                return value;
            Console.Write("Введите положительное число: ");
        }
    }

    public static int ReadIntegerInRange(int min, int max)
    {
        while (true)
        {
            string? input = Console.ReadLine();
            if (int.TryParse(input, out int value) && value >= min && value <= max)
                return value;
            Console.Write($"Введите число от {min} до {max}: ");
        }
    }
}