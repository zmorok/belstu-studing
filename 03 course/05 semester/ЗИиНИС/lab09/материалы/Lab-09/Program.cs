using System.Text;
namespace Lab09;

class Program
{
    const int mantis = 4;

    static void Main(string[] args)
    {
        Console.OutputEncoding = Encoding.UTF8;

        string input = "жукдмитрий";
        Console.WriteLine("Исходное сообщение: " + input);
        Console.WriteLine();

        var stats = BuildStatistics(input);

        Console.WriteLine("Вероятности символов:");
        foreach (var s in stats) Console.WriteLine($"{s.symbol} : {s.p:F4}");
        Console.WriteLine();

        var codes = BuildShannonFano(stats);

        Console.WriteLine("\nГотовые коды:");
        foreach (var kv in codes) Console.WriteLine($"{kv.Key} : {kv.Value}");

        Console.WriteLine();
        string encoded = Encode(input, codes);
        Console.WriteLine("Закодированная строка:\n" + encoded);
        Console.WriteLine();

        string decoded = Decode(encoded, codes);
        Console.WriteLine("Декодирование:\n" + decoded);
        Console.WriteLine();

        Console.WriteLine("Совпадает? " + (decoded == input ? "ДА" : "НЕТ"));

        int asciiBits = input.Length * 8;
        int shfBits = encoded.Length;
        double K = (double)asciiBits / shfBits;

        Console.WriteLine("\nЭффективность:");
        Console.WriteLine($"ASCII: {asciiBits}");
        Console.WriteLine($"Shannon–Fano: {shfBits}");
        Console.WriteLine($"Коэффициент K = {K:F3}");

        Console.ReadKey();
    }
    static List<(string symbol, float p)> BuildStatistics(string text)
    {
        Dictionary<char, int> freq = new Dictionary<char, int>();

        foreach (char c in text)
        {
            if (!freq.ContainsKey(c)) freq[c] = 0;
            freq[c]++;
        }

        int total = text.Length;

        var stats = freq
            .Select(x => (x.Key.ToString(), (float)Math.Round((double)x.Value / total, mantis)))
            .OrderByDescending(x => x.Item2)
            .ToList();

        float sum = stats.Sum(x => x.Item2);
        float diff = (float)Math.Round(1.0f - sum, mantis);
        stats[stats.Count - 1] = (stats.Last().Item1, stats.Last().Item2 + diff);

        return stats;
    }

    static Dictionary<string, string> BuildShannonFano(List<(string symbol, float p)> stats)
    {
        var codes = stats.ToDictionary(x => x.symbol, x => "");
        ProcessGroup(stats, codes, 0, stats.Count);
        return codes;
    }

    static void ProcessGroup(
        List<(string symbol, float p)> stats,
        Dictionary<string, string> codes,
        int start,
        int end)
    {
        if (end - start <= 1) return;

        Console.WriteLine("--------------------------------------------------");
        Console.WriteLine("Текущая группа:");

        float total = 0;
        for (int i = start; i < end; i++)
        {
            total = (float)Math.Round(total + stats[i].p, mantis);
            Console.WriteLine($"  {stats[i].symbol} : {stats[i].p:F4}");
        }

        float half = total / 2;
        Console.WriteLine($"Сумма = {total:F4}");
        Console.WriteLine($"Половина = {half:F4}\n");

        float accum = 0;
        int separator = start + 1;

        for (int i = start; i < end; i++)
        {
            accum += stats[i].p;

            if (accum >= half)
            {
                float diff1 = Math.Abs((total - accum) - accum);

                float prev = accum - stats[i].p;
                float diff2 = Math.Abs((total - prev) - prev);

                separator = (diff1 <= diff2) ? i + 1 : i;
                break;
            }
        }

        Console.WriteLine($"Разрез после {separator - start} символов\n");

        Console.WriteLine("Левая (1):");
        for (int i = start; i < separator; i++)
        {
            codes[stats[i].symbol] += "1";
            Console.WriteLine("  " + stats[i].symbol);
        }

        Console.WriteLine("\nПравая (0):");
        for (int i = separator; i < end; i++)
        {
            codes[stats[i].symbol] += "0";
            Console.WriteLine("  " + stats[i].symbol);
        }

        Console.WriteLine("\nКоды после шага:");
        foreach (var c in codes)
            Console.WriteLine($"  {c.Key} = {c.Value}");
        Console.WriteLine();

        ProcessGroup(stats, codes, start, separator);
        ProcessGroup(stats, codes, separator, end);
    }

    static string Encode(string input, Dictionary<string, string> codes)
    {
        StringBuilder sb = new StringBuilder();
        foreach (char c in input)
            sb.Append(codes[c.ToString()]);
        return sb.ToString();
    }

    static string Decode(string bits, Dictionary<string, string> codes)
    {
        var reverse = codes.ToDictionary(x => x.Value, x => x.Key);

        StringBuilder sb = new StringBuilder();
        string buffer = "";

        foreach (char b in bits)
        {
            buffer += b;
            if (reverse.ContainsKey(buffer))
            {
                sb.Append(reverse[buffer]);
                buffer = "";
            }
        }
        return sb.ToString();
    }
}