using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

class Program
{
    static void Main()
    {
        while (true)
        {
            Console.WriteLine("\nВыберите опцию:");
            Console.WriteLine("1. Инициализация и вывод примитивных типов");
            Console.WriteLine("2. Явное и неявное приведение типов");
            Console.WriteLine("3. Упаковка и распаковка значимых типов");
            Console.WriteLine("4. Работа с неявно типизированной переменной");
            Console.WriteLine("5. Работа с Nullable переменной");
            Console.WriteLine("6. Ошибка при присвоении значения другого типа переменной var");
            Console.WriteLine("0. Выход");

            string choice = Console.ReadLine();
            Console.WriteLine("\n");
            switch (choice)
            {
                case "1":
                    InitAndDisplayPrimitives();
                    break;
                case "2":
                    TypeConversions();
                    break;
                case "3":
                    BoxingAndUnboxing();
                    break;
                case "4":
                    ImplicitlyTypedVariable();
                    break;
                case "5":
                    NullableVariable();
                    break;
                case "6":
                    VarTypeError();
                    break;
                case "0":
                    return;
                default:
                    Console.WriteLine("Неверный выбор, попробуйте еще раз.");
                    break;
            }
            Console.WriteLine("\nДля продолжения нажмите любую клавишу...");
            Console.ReadKey();
            Console.Clear();
        }
    }

    static void InitAndDisplayPrimitives()
    {
        bool boolVar = true;
        byte byteVar = 255;
        sbyte sbyteVar = -128;
        char charVar = 'A';
        decimal decimalVar = decimal.MaxValue / 2;
        double doubleVar = 3.14159;
        float floatVar = 3.14f;
        int intVar = 214748;
        uint uintVar = 4267295;
        long longVar = 9223207;
        ulong ulongVar = 184615;
        short shortVar = -368;
        ushort ushortVar = 655;
        string stringVar = "Hello, World!";

        Console.WriteLine("Примитивные типы и их значения:");
        Console.WriteLine($"bool: {boolVar}");
        Console.WriteLine($"byte: {byteVar}");
        Console.WriteLine($"sbyte: {sbyteVar}");
        Console.WriteLine($"char: {charVar}");
        Console.WriteLine($"decimal: {decimalVar}");
        Console.WriteLine($"double: {doubleVar}");
        Console.WriteLine($"float: {floatVar}");
        Console.WriteLine($"int: {intVar}");
        Console.WriteLine($"uint: {uintVar}");
        Console.WriteLine($"long: {longVar}");
        Console.WriteLine($"ulong: {ulongVar}");
        Console.WriteLine($"short: {shortVar}");
        Console.WriteLine($"ushort: {ushortVar}");
        Console.WriteLine($"string: {stringVar}");
    }

    static void TypeConversions()
    {
        double doubleVal = 123.45;
        int intVal = (int)doubleVal;
        float floatVal = (float)doubleVal;
        byte byteVal = (byte)intVal;
        long longVal = (long)floatVal;
        short shortVal = (short)byteVal;

        Console.WriteLine("Явное приведение:");
        Console.WriteLine($"double to int: {intVal}");
        Console.WriteLine($"double to float: {floatVal}");
        Console.WriteLine($"int to byte: {byteVal}");
        Console.WriteLine($"float to long: {longVal}");
        Console.WriteLine($"byte to short: {shortVal}");

        intVal = 123;
        doubleVal = intVal;
        floatVal = intVal;
        longVal = intVal;
        decimal decimalVal = intVal;

        Console.WriteLine("Неявное приведение:");
        Console.WriteLine($"int to double: {doubleVal}");
        Console.WriteLine($"int to float: {floatVal}");
        Console.WriteLine($"int to long: {longVal}");
        Console.WriteLine($"int to decimal: {decimalVal}");

        string strVal = "12345";
        int convertedInt = Convert.ToInt32(strVal);
        double convertedDouble = Convert.ToDouble(strVal);
        bool convertedBool = Convert.ToBoolean(1);
        decimal convertedDecimal = Convert.ToDecimal(doubleVal);
        byte convertedByte = Convert.ToByte(intVal);

        Console.WriteLine("Использование класса Convert:");
        Console.WriteLine($"string to int: {convertedInt}");
        Console.WriteLine($"string to double: {convertedDouble}");
        Console.WriteLine($"int to bool: {convertedBool}");
        Console.WriteLine($"double to decimal: {convertedDecimal}");
        Console.WriteLine($"int to byte: {convertedByte}");
    }

    static void BoxingAndUnboxing()
    {
        int intValue = 123;
        object boxedValue = intValue;
        int unboxedValue = (int)boxedValue;

        Console.WriteLine("Упаковка и распаковка:");
        Console.WriteLine($"Упакованное значение: {boxedValue}");
        Console.WriteLine($"Распакованное значение: {unboxedValue}");
    }

    static void ImplicitlyTypedVariable()
    {
        var number = 42.23;
        Console.WriteLine("Неявно типизированная переменная:");
        Console.WriteLine($"Значение: {number}");
        Console.WriteLine($"Тип: {number.GetType()}");
    }

    static void NullableVariable()
    {
        int? nullableInt = null;
        Console.WriteLine("Nullable переменная:");
        Console.WriteLine($"Значение: {nullableInt}");
        Console.WriteLine($"Имеет значение? {nullableInt.HasValue}");

        nullableInt = 10;
        Console.WriteLine($"Новое значение: {nullableInt}");
        Console.WriteLine($"Имеет значение? {nullableInt.HasValue} {nullableInt}");
    }

    static void VarTypeError()
    {
        var variable = 123;
        Console.WriteLine("Переменная var:");
        Console.WriteLine($"Значение: {variable}");
        Console.WriteLine($"Тип: {variable.GetType()}");

        // для ошибки компиляции
        // variable = "Hello";
    }
}
