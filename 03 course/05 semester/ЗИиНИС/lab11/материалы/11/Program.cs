using System;
using System.Collections.Generic;
using System.Linq;
using System.Numerics;
using System.Text;

namespace lab11
{
    public class Program
    {
        static void Main(string[] args) => Lab11();
        
        static void Lab11()
        {
            string word1 = "песнетворчество";
            string word2 = "песнетворчествоэлектрифицированный";

            RunCase(word1);
            Console.WriteLine("\n\n");
            RunCase(word2);

            Console.ReadLine();
        }

        static void RunCase(string word)
        {
            int wordLength = word.Length;
            Compressor wordCompressor = new Compressor();

            Console.WriteLine($"Исходное слово: {word}");
            wordCompressor.Build(word);

            Console.WriteLine("Вероятности:");
            foreach (var n in wordCompressor.Nodes)
            {
                decimal p = (decimal)(n.HighCount - n.LowCount) / wordCompressor.Total;
                Console.WriteLine($"p({n.Symbol}) = {p}");
            }

            Console.WriteLine("\nИнтервалы:");
            foreach (var n in wordCompressor.Nodes)
            {
                var low = new DecimalCode(n.LowCount, 0).ToDecimalStringDivide(wordCompressor.Total, 15);
                var high = new DecimalCode(n.HighCount, 0).ToDecimalStringDivide(wordCompressor.Total, 15);
                Console.WriteLine($"{n.Symbol}  {low} - {high}");
            }

            Console.WriteLine();
            
            var compressResult = wordCompressor.Compress(word);
            Console.WriteLine("Сжатые данные:");
            Console.WriteLine(InfoString.Sb.ToString());

            var decompressResult = wordCompressor.Decompress(compressResult, wordLength);
            Console.WriteLine("Расжатые данные:");
            Console.WriteLine(InfoString.Sb.ToString());
        }
    }

    public class Compressor
    {
        public List<Node> Nodes { get; private set; } = new();
        public int Total { get; private set; }

        private Dictionary<char, Node> _map = new();

        public void Build(string source)
        {
            if (string.IsNullOrEmpty(source)) throw new ArgumentException("source is empty");

            Total = source.Length;

            var counts = new Dictionary<char, int>();
            foreach (var ch in source) if (!counts.TryAdd(ch, 1)) counts[ch]++;
            

            var ordered = counts.OrderBy(kv => kv.Value).ThenBy(kv => kv.Key).ToList();

            Nodes = new List<Node>(ordered.Count);
            _map = new Dictionary<char, Node>(ordered.Count);

            int sums = 0;
            foreach (var kv in ordered)
            {
                var n = new Node
                {
                    Symbol = kv.Key,
                    LowCount = sums,
                    HighCount = sums + kv.Value
                };
                sums += kv.Value;

                Nodes.Add(n);
                _map[n.Symbol] = n;
            }
        }

        public string Compress(string source)
        {
            InfoString.Sb = new StringBuilder();

            BigInteger low = 0;
            BigInteger high = 1;
            BigInteger den = 1;

            const int PRINT_DIGITS = 50;

            for (int i = 0; i < source.Length; i++)
            {
                char sym = source[i];
                if (!_map.TryGetValue(sym, out var node))
                    throw new InvalidOperationException($"Symbol '{sym}' not in model.");

                InfoString.Sb.AppendLine($"Шаг {i + 1}. Выбран символ: '{sym}'");
                InfoString.Sb.AppendLine($"Текущий интервал:\n   [{ToDecimalComma(low, den, PRINT_DIGITS)} ; {ToDecimalComma(high, den, PRINT_DIGITS)})");
                InfoString.Sb.AppendLine("Интервалы для всех символов:");

                BigInteger range = high - low;

                foreach (var n in Nodes)
                {
                    BigInteger candLow = low * Total + range * n.LowCount;
                    BigInteger candHigh = low * Total + range * n.HighCount;
                    BigInteger candDen = den * Total;

                    var gShow = BigInteger.GreatestCommonDivisor(BigInteger.GreatestCommonDivisor(candLow, candHigh), candDen);
                    if (gShow > 1)
                    {
                        candLow /= gShow;
                        candHigh /= gShow;
                        candDen /= gShow;
                    }

                    InfoString.Sb.AppendLine(
                        $"{n.Symbol}: [{ToDecimalComma(candLow, candDen, PRINT_DIGITS)} ; {ToDecimalComma(candHigh, candDen, PRINT_DIGITS)})"
                    );
                }

                BigInteger newDen = den * Total;
                BigInteger newLow = low * Total + range * node.LowCount;
                BigInteger newHigh = low * Total + range * node.HighCount;

                low = newLow;
                high = newHigh;
                den = newDen;

                var g = BigInteger.GreatestCommonDivisor(BigInteger.GreatestCommonDivisor(low, high), den);
                if (g > 1)
                {
                    low /= g;
                    high /= g;
                    den /= g;
                }

                InfoString.Sb.AppendLine($"После сужения:\n   [{ToDecimalComma(low, den, PRINT_DIGITS)} ; {ToDecimalComma(high, den, PRINT_DIGITS)})");
                InfoString.Sb.AppendLine(new string('-', 80));
            }

            PickDecimalInsideInterval(low, high, den, out BigInteger k, out int digits);
            var code = new DecimalCode(k, digits);

            InfoString.Sb.AppendLine($"Знаков после запятой нужно: {digits}");
            InfoString.Sb.AppendLine($"Результат: {code.ToCommaString()}");

            return code.ToCommaString();
        }


        public string Decompress(string codeComma, int length)
        {
            var code = DecimalCode.ParseCommaOrDot(codeComma);

            InfoString.Sb = new StringBuilder();
            InfoString.Sb.AppendLine($"Количество символов {length}");
            InfoString.Sb.AppendLine($"Код: {code.ToCommaString()}");
            InfoString.Sb.AppendLine();

            BigInteger vNum = code.Num;
            BigInteger vDen = code.Den;

            var sb = new StringBuilder();

            for (int i = 0; i < length; i++)
            {
                BigInteger scaled = vNum * Total;

                Node found = null!;
                foreach (var n in Nodes)
                {
                    BigInteger lo = (BigInteger)n.LowCount * vDen;
                    BigInteger hi = (BigInteger)n.HighCount * vDen;

                    if (scaled >= lo && scaled < hi)
                    {
                        found = n;
                        break;
                    }
                }

                if (found == null) throw new InvalidOperationException("Decoding failed: code out of range.");

                sb.Append(found.Symbol);

                // value = (value - Low/Total) / ((High-Low)/Total)
                BigInteger newNum = vNum * Total - (BigInteger)found.LowCount * vDen;
                BigInteger newDen = vDen * (found.HighCount - found.LowCount);

                var g = BigInteger.GreatestCommonDivisor(BigInteger.Abs(newNum), newDen);
                if (g > 1)
                {
                    newNum /= g;
                    newDen /= g;
                }

                vNum = newNum;
                vDen = newDen;

                InfoString.Sb.AppendLine($"на {i + 1} этапе получили {ToDecimalComma(vNum, vDen, 50)}\t-- {found.Symbol}");
            }

            return sb.ToString();
        }

        private static void PickDecimalInsideInterval(BigInteger low, BigInteger high, BigInteger den, out BigInteger k, out int digits)
        {
            BigInteger scale = 1; // 10^d
            digits = 0;

            while (true)
            {
                BigInteger kMin = CeilDiv(low * scale, den);
                BigInteger kMax = (high * scale - 1) / den; // строго < high/den

                if (kMin <= kMax)
                {
                    k = kMin;
                    return;
                }

                scale *= 10;
                digits++;

                if (digits > 5000)
                    throw new InvalidOperationException("Too many digits needed (unexpected).");
            }
        }

        private static BigInteger CeilDiv(BigInteger a, BigInteger b) => (a + b - 1) / b;
        

        static string ToDecimalComma(BigInteger num, BigInteger den, int fracDigits)
        {
            if (den.IsZero) throw new DivideByZeroException();
            if (fracDigits < 0) throw new ArgumentOutOfRangeException(nameof(fracDigits));

            bool neg = num.Sign < 0;
            if (neg) num = BigInteger.Abs(num);
            den = BigInteger.Abs(den);

            BigInteger intPart = num / den;
            BigInteger rem = num % den;

            var sb = new StringBuilder();
            if (neg) sb.Append('-');
            sb.Append(intPart);

            if (fracDigits == 0) return sb.ToString();

            sb.Append(',');

            for (int i = 0; i < fracDigits; i++)
            {
                rem *= 10;
                BigInteger digit = rem / den;
                rem %= den;
                sb.Append((char)('0' + (int)digit));
            }

            return sb.ToString();
        }

    }

    public class Node
    {
        public char Symbol { get; set; }
        public int LowCount { get; set; }
        public int HighCount { get; set; }
    }

    public static class InfoString
    {
        public static StringBuilder Sb { get; set; } = new StringBuilder();
    }

    public readonly struct DecimalCode
    {
        public BigInteger Num { get; }
        public int Digits { get; }
        public BigInteger Den { get; }

        public DecimalCode(BigInteger num, int digits)
        {
            if (digits < 0) throw new ArgumentOutOfRangeException(nameof(digits));
            if (num < 0) throw new ArgumentOutOfRangeException(nameof(num));

            Num = num;
            Digits = digits;
            Den = BigInteger.Pow(10, digits);
        }

        public string ToCommaString()
        {
            if (Digits == 0) return Num.ToString();

            var intPart = Num / Den;
            var fracPart = (Num % Den).ToString().PadLeft(Digits, '0');
            return intPart.ToString() + "," + fracPart;
        }

        public static DecimalCode ParseCommaOrDot(string s)
        {
            if (s == null) throw new ArgumentNullException(nameof(s));
            s = s.Trim().Replace('.', ',');

            if (!s.Contains(','))
            {
                var n = BigInteger.Parse(s);
                return new DecimalCode(n, 0);
            }

            var parts = s.Split(',');
            if (parts.Length != 2) throw new FormatException("Bad decimal format.");

            string intPart = parts[0].Length == 0 ? "0" : parts[0];
            string fracPart = parts[1];

            int digits = fracPart.Length;
            BigInteger den = BigInteger.Pow(10, digits);

            BigInteger i = BigInteger.Parse(intPart);
            BigInteger f = fracPart.Length == 0 ? BigInteger.Zero : BigInteger.Parse(fracPart);

            BigInteger num = i * den + f;
            return new DecimalCode(num, digits);
        }

        public string ToDecimalStringDivide(int divisor, int fracDigits)
        {
            if (divisor <= 0) throw new ArgumentOutOfRangeException(nameof(divisor));
            if (fracDigits < 0) throw new ArgumentOutOfRangeException(nameof(fracDigits));

            BigInteger scale = BigInteger.Pow(10, fracDigits);
            BigInteger q = (Num * scale) / divisor;

            string s = q.ToString();
            if (fracDigits == 0) return s;

            if (s.Length <= fracDigits)
                s = s.PadLeft(fracDigits + 1, '0');

            int cut = s.Length - fracDigits;
            return s.Substring(0, cut) + "," + s.Substring(cut);
        }
    }
}
