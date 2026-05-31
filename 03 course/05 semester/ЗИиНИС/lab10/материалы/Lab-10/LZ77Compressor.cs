using System.Diagnostics;
using System.Text;

namespace Lempel_Ziv_method;

// Реализация алгоритма сжатия LZ77
class LZ77Compressor
{
    private int _dictionarySize;
    private int _bufferSize;
    private int _numeralBase;
    private int _positionDigits;
    private int _lengthDigits;

    public void Run()
    {
        Console.WriteLine("Алгоритм сжатия LZ77");
        Console.WriteLine("====================");
        Console.WriteLine();

        string sourceText = InputHandler.LoadSourceText();
        if (string.IsNullOrEmpty(sourceText))
        {
            Console.WriteLine("Ошибка: не удалось загрузить исходный текст.");
            return;
        }

        DisplaySourceText(sourceText);
        RequestCompressionParameters();

        var encodedTokens = Compress(sourceText);
        var decompressedText = Decompress(encodedTokens);

        VerifyResult(sourceText, decompressedText);
        
        Console.WriteLine("\nНажмите любую клавишу для выхода...");
        Console.ReadKey();
    }

    private void DisplaySourceText(string text)
    {
        Console.WriteLine("\nИсходное сообщение:");
        Console.WriteLine(text);
        Console.WriteLine($"Длина: {text.Length} символов\n");
    }

    private void RequestCompressionParameters()
    {
        Console.Write("Длина словаря: ");
        _dictionarySize = InputHandler.ReadPositiveInteger();

        Console.Write("Длина буфера: ");
        _bufferSize = InputHandler.ReadPositiveInteger();

        Console.Write("Система счисления (2-16): ");
        _numeralBase = InputHandler.ReadIntegerInRange(2, 16);

        CalculateDigitCounts();
        DisplayParameters();
    }

    private void CalculateDigitCounts()
    {
        _positionDigits = NumberConverter.CalculateRequiredDigits(_dictionarySize, _numeralBase);
        _lengthDigits = NumberConverter.CalculateRequiredDigits(_bufferSize, _numeralBase);
    }

    private void DisplayParameters()
    {
        Console.WriteLine($"\nПараметры кодирования:");
        Console.WriteLine($"  Разрядность позиции (P): {_positionDigits}");
        Console.WriteLine($"  Разрядность длины (Q): {_lengthDigits}");
        Console.WriteLine($"  Основание системы: {_numeralBase}");
        Console.WriteLine();
    }

    private List<EncodedToken> Compress(string sourceText)
    {
        Console.WriteLine("=== Процесс сжатия ===");
        
        var tokens = new List<EncodedToken>();
        var dictionary = InitializeDictionary();
        var lookAheadBuffer = InitializeLookAheadBuffer(sourceText);
        var remainingText = sourceText.Substring(Math.Min(_bufferSize, sourceText.Length));

        var stopwatch = Stopwatch.StartNew();

        while (lookAheadBuffer.Length > 0)
        {
            var match = FindLongestMatch(dictionary, lookAheadBuffer);
            var token = CreateToken(match, lookAheadBuffer);
            tokens.Add(token);

            DisplayCompressionStep(dictionary, lookAheadBuffer, token);

            UpdateSlidingWindow(ref dictionary, ref lookAheadBuffer, ref remainingText, match);
        }

        stopwatch.Stop();
        Console.WriteLine($"\nВремя сжатия: {stopwatch.ElapsedMilliseconds} мс");
        Console.WriteLine($"Создано токенов: {tokens.Count}");

        return tokens;
    }

    private string InitializeDictionary()
    {
        return new string('0', _dictionarySize);
    }

    private string InitializeLookAheadBuffer(string text)
    {
        int initialSize = Math.Min(_bufferSize, text.Length);
        return text.Substring(0, initialSize);
    }

    private MatchResult FindLongestMatch(string dictionary, string buffer)
    {
        int bestPosition = 0;
        int bestLength = 0;
        int searchLength = 1;

        while (searchLength <= buffer.Length)
        {
            string pattern = buffer.Substring(0, searchLength);
            int position = dictionary.IndexOf(pattern);

            if (position >= 0)
            {
                bestPosition = position + 1; // 1-based для алгоритма
                bestLength = searchLength;
                searchLength++;
            }
            else
            {
                break;
            }
        }

        return new MatchResult
        {
            Position = bestPosition,
            Length = bestLength
        };
    }

    private EncodedToken CreateToken(MatchResult match, string buffer)
    {
        string positionStr = NumberConverter.ConvertToBaseString(match.Position, _numeralBase, _positionDigits);
        string lengthStr = NumberConverter.ConvertToBaseString(match.Length, _numeralBase, _lengthDigits);
        
        string nextChar = match.Length < buffer.Length 
            ? buffer[match.Length].ToString() 
            : string.Empty;

        return new EncodedToken
        {
            Position = positionStr,
            Length = lengthStr,
            NextChar = nextChar
        };
    }

    private void DisplayCompressionStep(string dictionary, string buffer, EncodedToken token)
    {
        string tokenStr = token.Position + token.Length + token.NextChar;
        Console.WriteLine($"{dictionary}|{buffer}| -> триада: {tokenStr}");
    }

    private void UpdateSlidingWindow(ref string dictionary, ref string buffer, ref string remaining, MatchResult match)
    {
        int step = match.Length + (string.IsNullOrEmpty(buffer) || match.Length >= buffer.Length ? 0 : 1);
        step = Math.Min(step, buffer.Length);

        if (step > 0)
        {
            string processed = buffer.Substring(0, step);
            
            dictionary = dictionary.Remove(0, step) + processed;
            buffer = buffer.Remove(0, step);

            int takeFromRemaining = Math.Min(step, remaining.Length);
            if (takeFromRemaining > 0)
            {
                buffer += remaining.Substring(0, takeFromRemaining);
                remaining = remaining.Remove(0, takeFromRemaining);
            }
        }
    }

    private string Decompress(List<EncodedToken> tokens)
    {
        Console.WriteLine("\n=== Процесс распаковки ===");
        
        var decompressed = new StringBuilder();
        var dictionary = InitializeDictionary();

        var stopwatch = Stopwatch.StartNew();

        foreach (var token in tokens)
        {
            string decoded = ProcessToken(token, dictionary);
            decompressed.Append(decoded);
            UpdateDictionary(ref dictionary, decoded);

            Console.WriteLine($"{decompressed}|{dictionary}|{token.Position}{token.Length}{token.NextChar}");
        }

        stopwatch.Stop();
        Console.WriteLine($"\nВремя распаковки: {stopwatch.ElapsedMilliseconds} мс");

        return decompressed.ToString();
    }

    private string ProcessToken(EncodedToken token, string dictionary)
    {
        int position = NumberConverter.ConvertFromBaseString(token.Position, _numeralBase);
        int length = NumberConverter.ConvertFromBaseString(token.Length, _numeralBase);

        string result;

        if (position == 0)
        {
            if (string.IsNullOrEmpty(token.NextChar))
            {
                result = dictionary.Substring(0, length);
            }
            else
            {
                result = token.NextChar;
            }
        }
        else
        {
            int startIndex = position - 1;
            string matched = dictionary.Substring(startIndex, length);
            result = matched + (string.IsNullOrEmpty(token.NextChar) ? string.Empty : token.NextChar);
        }

        return result;
    }

    private void UpdateDictionary(ref string dictionary, string newContent)
    {
        int step = newContent.Length;
        dictionary = dictionary.Remove(0, step) + newContent;
    }

    private void VerifyResult(string original, string decompressed)
    {
        Console.WriteLine("\n=== Результат ===");
        Console.WriteLine("Распакованное сообщение:");
        Console.WriteLine(decompressed);
        Console.WriteLine();

        bool isCorrect = original.Equals(decompressed, StringComparison.Ordinal);
        if (isCorrect)
        {
            Console.WriteLine("✓ Проверка пройдена: исходный и распакованный тексты совпадают");
        }
        else
        {
            Console.WriteLine("✗ Ошибка: тексты не совпадают!");
            Console.WriteLine($"Оригинал: {original.Length} символов");
            Console.WriteLine($"Распаковано: {decompressed.Length} символов");
        }
    }
}