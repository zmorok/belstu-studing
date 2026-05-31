using System.Text;
namespace Lempel_Ziv_method;

// Утилиты для конвертации чисел в различные системы счисления
static class NumberConverter
{
    public static string ConvertToBaseString(int number, int numeralBase, int requiredDigits)
    {
        if (number == 0)
            return new string('0', requiredDigits);

        var result = new StringBuilder();
        int current = number;

        while (current > 0)
        {
            int remainder = current % numeralBase;
            result.Insert(0, remainder.ToString());
            current /= numeralBase;
        }

        while (result.Length < requiredDigits)
        {
            result.Insert(0, '0');
        }

        return result.ToString();
    }
    public static int ConvertFromBaseString(string numberStr, int numeralBase)
    {
        int result = 0;
        int power = 1;

        for (int i = numberStr.Length - 1; i >= 0; i--)
        {
            int digit = int.Parse(numberStr[i].ToString());
            result += digit * power;
            power *= numeralBase;
        }

        return result;
    }
    public static int CalculateRequiredDigits(int maxValue, int numeralBase)
    {
        if (maxValue <= 0) return 1;
        return (int)Math.Ceiling(Math.Log(maxValue + 1, numeralBase));
    }
}