using System;
using System.Collections.Generic;
using System.Globalization;
using System.Text;

namespace Lab8_BWT_Matrices
{
    internal class Program
    {
        private static readonly StringComparer RuComparer =
            StringComparer.Create(new CultureInfo("ru-RU"), ignoreCase: true);

        static void Main(string[] args)
        {
            Console.OutputEncoding = Encoding.UTF8;

            Console.Write("Введите имя: ");
            string name = Console.ReadLine() ?? string.Empty;
            if (string.IsNullOrWhiteSpace(name))
            {
                name = "дмитрий";
                Console.WriteLine($"Пустой ввод, используется пример: {name}");
            }

            Console.Write("Введите фамилию (без пробелов): ");
            string surname = Console.ReadLine() ?? string.Empty;
            if (string.IsNullOrWhiteSpace(surname))
            {
                surname = "жук";
                Console.WriteLine($"Пустой ввод, используется пример: {surname}");
            }

            Console.Write("Введите слово: ");
            string variantWord = Console.ReadLine() ?? string.Empty;
            if (string.IsNullOrWhiteSpace(variantWord))
            {
                variantWord = "песнетворчество";
                Console.WriteLine($"Пустой ввод, используется пример: {surname}");
            }

            Console.WriteLine();
            ProcessWord("Имя", name);
            ProcessWord("Фамилия", surname);
            ProcessWord("Слово по варианту", variantWord);

            Console.ReadLine();
        }

        private static void ProcessWord(string title, string word)
        {
            Console.WriteLine(new string('=', 70));
            Console.WriteLine($"{title}: \"{word}\"");
            Console.WriteLine($"Длина слова: {word.Length}");
            Console.WriteLine(new string('-', 70));

            var rotations = BuildRotations(word);
            Console.WriteLine("\nМатрица W1:");
            PrintMatrix(rotations);

            var (sorted, Mk, z) = BuildSortedMatrixAndMk(rotations, word);
            Console.WriteLine("\nМатрица W2:");
            PrintMatrix(sorted);

            Console.WriteLine($"\nПоследний столбец Mk: {Mk}");
            Console.WriteLine($"Номер строки z (где стоит исходное слово): {z}");

            // 2. Обратное преобразование: по Mk и z восстановить слово
            Console.WriteLine("\nОбратное преобразование:");
            string decoded = BwtDecodeWithSteps(Mk, z);

            Console.WriteLine($"\nВосстановленное слово: \"{decoded}\"");
            Console.WriteLine($"Совпадает с исходным: {string.Equals(word, decoded, StringComparison.Ordinal)}");
            Console.WriteLine();
        }

        private static List<string> BuildRotations(string s)
        {
            int n = s.Length;
            var list = new List<string>(n);
            for (int i = 0; i < n; i++)
            {
                string rotation = s.Substring(i) + s.Substring(0, i);
                list.Add(rotation);
            }
            return list;
        }

        private static (List<string> sorted, string Mk, int z) BuildSortedMatrixAndMk(
            List<string> rotations,
            string original)
        {
            int n = rotations.Count;
            var sorted = new List<string>(rotations);
            sorted.Sort(RuComparer);

            var sbMk = new StringBuilder(n);
            int z = -1;

            for (int i = 0; i < n; i++)
            {
                string row = sorted[i];
                sbMk.Append(row[n - 1]);

                if (z == -1 && RuComparer.Compare(row, original) == 0)
                {
                    z = i + 1;
                }
            }

            if (z == -1)
            {
                throw new InvalidOperationException("Исходная строка не найдена в W2.");
            }

            return (sorted, sbMk.ToString(), z);
        }

        private static void PrintMatrix(IReadOnlyList<string> rows)
        {
            for (int i = 0; i < rows.Count; i++)
            {
                Console.WriteLine($"{(i + 1).ToString().PadLeft(2)}: {rows[i]}");
            }
        }

        private static string BwtDecodeWithSteps(string Mk, int z)
        {
            int n = Mk.Length;
            var table = new string[n];

            for (int i = 0; i < n; i++)
            {
                table[i] = Mk[i].ToString();
            }

            Array.Sort(table, RuComparer);

            Console.WriteLine("\nШаг 1:");
            PrintMatrix(table);

            for (int step = 2; step <= n; step++)
            {
                for (int i = 0; i < n; i++)
                {
                    table[i] = Mk[i] + table[i];
                }

                Array.Sort(table, RuComparer);

                Console.WriteLine($"\nШаг {step}:");
                PrintMatrix(table);
            }

            return table[z - 1];
        }
    }
}